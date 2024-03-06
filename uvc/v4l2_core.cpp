#include "v4l2_core.h"

#include <base/log.h>
#include <fcntl.h>
#include <libintl.h>
#include <libv4l2.h>
#include <string.h>
#include <sys/mman.h>

#include "v4l2_define.h"
#include "v4l2_format.h"
#include "v4l2_util.h"

namespace uvc {

/*
 * clean video context data allocation
 * args:
 *   vd - pointer to v4l2 device handler
 *
 * returns: void
 */
static void clean_v4l2_dev(V4L2Context *context) {
    // if (vd->has_focus_control_id) v4l2core_soft_autofocus_close();

    // if (vd->list_device_controls) free_v4l2_control_list(vd);

    // if (vd->list_stream_formats) free_frame_formats(vd);

    // if (vd->frame_queue) free(vd->frame_queue);

    /*close descriptor*/
    if (context->fd > 0) {
        v4l2_close(context->fd);
        context->fd = 0;
    }

    free(context);
}

/*
 * Query video device capabilities and supported formats
 * args:
 *   vd - pointer to v4l2 device handler
 *
 * returns: error code  (E_OK)
 */
static int check_v4l2_dev(V4L2Context *context) {
    memset(&context->cap, 0, sizeof(struct v4l2_capability));

    if (xioctl(context->fd, VIDIOC_QUERYCAP, &context->cap) < 0) {
        base::LogError() << "(VIDIOC_QUERYCAP) error: " << strerror(errno);
        return E_QUERYCAP_ERR;
    }

    if ((context->cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) {
        base::LogError() << "Error opening device " << context->videodevice
                         << " : video capture not supported.";
        return E_QUERYCAP_ERR;
    }
    if (!(context->cap.capabilities & V4L2_CAP_STREAMING)) {
        base::LogError() << "V4L2_CORE: " << context->videodevice
                         << " does not support streaming i/o";
        return E_QUERYCAP_ERR;
    }

    if (context->cap_meth == IO_READ) {
        // vd->mem[vd->buf.index] = NULL;
        if (!(context->cap.capabilities & V4L2_CAP_READWRITE)) {
            base::LogError() << context->videodevice << " does not support read, try with mmap";
            return E_READ_ERR;
        }
    }
    base::LogDebug() << "Init. " << context->cap.card << "location: " << context->cap.bus_info;

    /*enumerate frame formats supported by device*/
    int ret = enum_frame_formats(context);
    if (ret != E_OK) {
        base::LogError() << "No valid frame formats (with valid sizes) found for device";
        return ret;
    }

    // /*add h264 (uvc muxed) to format list if supported by device*/
    // add_h264_format(vd);

    // /*enumerate device controls*/
    // enumerate_v4l2_control(vd);
    // /*gets the current control values and sets their flags*/
    // get_v4l2_control_values(vd);

    // /*if we have a focus control initiate the software autofocus*/
    // if (vd->has_focus_control_id) {
    //     if (v4l2core_soft_autofocus_init(vd) != E_OK) vd->has_focus_control_id = 0;
    // }

    return E_OK;
}

/*
 * Initiate video device handler with default values
 * args:
 *   device - device name (e.g: "/dev/video0")
 *
 * returns: pointer to V4L2Context handler (or NULL on error)
 */
V4L2Context *v4l2core_init_dev(const char *device) {
    /*localization*/
    char *lc_all = setlocale(LC_ALL, "");
    char *lc_dir = bindtextdomain(GETTEXT_PACKAGE_V4L2CORE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE_V4L2CORE, "UTF-8");
    base::LogDebug() << "language catalog=> dir:" << lc_dir << "type:" << lc_all
                     << " cat:" << GETTEXT_PACKAGE_V4L2CORE << ".mo";

    /*alloc the device data*/
    V4L2Context *context = (V4L2Context *)calloc(1, sizeof(V4L2Context));

    /*MMAP by default*/
    context->cap_meth = IO_MMAP;

    context->videodevice = device;

    base::LogDebug() << "capture method mmap " << context->cap_meth;
    base::LogDebug() << "video device: " << context->videodevice;
    context->frame_queue_size = frame_queue_size;
    /*alloc frame buffer queue*/
    // vd->frame_queue = calloc(vd->frame_queue_size, sizeof(v4l2_frame_buff_t));

    context->h264_no_probe_default = 0;
    context->h264_SPS = NULL;
    context->h264_SPS_size = 0;
    context->h264_PPS = NULL;
    context->h264_PPS_size = 0;
    context->h264_last_IDR = NULL;
    context->h264_last_IDR_size = 0;

    /*set some defaults*/
    context->fps_num = 1;
    context->fps_denom = 25;

    context->pan_step = 128;
    context->tilt_step = 128;

    /*open device*/
    if ((context->fd = v4l2_open(context->videodevice.c_str(), O_RDWR | O_NONBLOCK, 0)) < 0) {
        base::LogError() << "V4L2_CORE: ERROR opening V4L interface: " << strerror(errno);
        // clean_v4l2_dev(vd);
        return NULL;
    }

    /*try to map known xu controls (we could/should leave this for libwebcam)*/
    // init_xu_ctrls(vd);

    /*zero structs*/
    memset(&context->cap, 0, sizeof(struct v4l2_capability));
    memset(&context->format, 0, sizeof(struct v4l2_format));
    memset(&context->buf, 0, sizeof(struct v4l2_buffer));
    memset(&context->rb, 0, sizeof(struct v4l2_requestbuffers));
    memset(&context->streamparm, 0, sizeof(struct v4l2_streamparm));
    memset(&context->evsub, 0, sizeof(struct v4l2_event_subscription));

    if (check_v4l2_dev(context) != E_OK) {
        clean_v4l2_dev(context);
        return (NULL);
    }

    return context;
}

/*
 * Start video stream
 * args:
 *   vd - pointer to V4L2Context
 *
 * returns: VIDIOC_STREAMON ioctl result (E_OK or E_STREAMON_ERR)
*/
int v4l2core_start_stream(V4L2Context *context) {
    if (context->streaming == STRM_OK) {
        base::LogWarn() << "Stream already started) stream_status = STRM_OK\n";
        return E_OK;
    }

    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret = E_OK;
    switch (context->cap_meth) {
        case IO_READ:
            //do nothing
            break;
        case IO_MMAP:
        default:
            ret = xioctl(context->fd, VIDIOC_STREAMON, &type);
            if (ret < 0) {
                base::LogError() << "(VIDIOC_STREAMON) Unable to start stream: " << strerror(errno);
                return E_STREAMON_ERR;
            }
            break;
    }

    context->streaming = STRM_OK;

    base::LogDebug() << "(VIDIOC_STREAMON) stream_status = STRM_OK";

    return ret;
}

/*
 * Stops the video stream
 * args:
 *   context - pointer to V4L2Context
 *
 * returns: VIDIOC_STREAMON ioctl result (E_OK)
*/
int v4l2core_stop_stream(V4L2Context *context) {
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret = E_OK;
    switch (context->cap_meth) {
        case IO_READ:
        case IO_MMAP:
        default:
            ret = xioctl(context->fd, VIDIOC_STREAMOFF, &type);
            if (ret < 0) {
                if (errno == 9) { /* stream allready stoped*/
                    context->streaming = STRM_STOP;
                }
                base::LogError() << "(VIDIOC_STREAMOFF) Unable to stop stream: " << strerror(errno);
                return E_STREAMOFF_ERR;
            }
            break;
    }

    context->streaming = STRM_STOP;

    base::LogDebug() << "(VIDIOC_STREAMOFF) stream_status = STRM_STOP";

    return ret;
}

/*
 * maps v4l2 buffers
 * args:
 *   context - pointer to V4L2Context
 *
 * returns: error code  (0- E_OK)
 */
static int map_buff(V4L2Context *context) {
    base::LogDebug() << "V4L2_CORE: mapping v4l2 buffers";

    // map new buffer
    for (int i = 0; i < NB_BUFFER; i++) {
        context->mem[i] = v4l2_mmap(NULL,  // start anywhere
                                    context->buff_length[i], PROT_READ | PROT_WRITE, MAP_SHARED,
                                    context->fd, context->buff_offset[i]);
        if (context->mem[i] == MAP_FAILED) {
            base::LogError() << "V4L2_CORE: Unable to map buffer: " << strerror(errno);
            return E_MMAP_ERR;
        }
        base::LogError() << "V4L2_CORE: mapped buffer[" << i << "] with length "
                         << context->buff_length[i] << " to pos " << context->mem[i];
    }

    return (E_OK);
}

/*
 * unmaps v4l2 buffers
 * args:
 *   context - pointer to V4L2Context
 *
 * returns: error code  (0- E_OK)
 */
static int unmap_buff(V4L2Context *context) {
    base::LogDebug() << "V4L2_CORE: unmapping v4l2 buffers";
    int ret = E_OK;

    switch (context->cap_meth) {
        case IO_READ:
            break;

        case IO_MMAP:
            for (int i = 0; i < NB_BUFFER; i++) {
                // unmap old buffer
                if ((context->mem[i] != MAP_FAILED) && context->buff_length[i])
                    if ((ret = v4l2_munmap(context->mem[i], context->buff_length[i])) < 0) {
                        base::LogError() << "V4L2_CORE: couldn't unmap buff: " << strerror(errno);
                    }
            }
    }
    return ret;
}

/*
 * Query and map buffers
 * args:
 *   context - pointer to V4L2Context
 *
 * returns: error code  (0- E_OK)
 */
static int query_buff(V4L2Context *context) {
    base::LogDebug() << "query v4l2 buffers";

    int ret = E_OK;
    switch (context->cap_meth) {
        case IO_READ:
            break;

        case IO_MMAP:
            for (int i = 0; i < NB_BUFFER; i++) {
                memset(&context->buf, 0, sizeof(struct v4l2_buffer));
                context->buf.index = i;
                context->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                // vd->buf.flags = V4L2_BUF_FLAG_TIMECODE;
                // vd->buf.timecode = vd->timecode;
                // vd->buf.timestamp.tv_sec = 0;
                // vd->buf.timestamp.tv_usec = 0;
                context->buf.memory = V4L2_MEMORY_MMAP;
                ret = xioctl(context->fd, VIDIOC_QUERYBUF, &context->buf);

                if (ret < 0) {
                    base::LogError() << "V4L2_CORE: (VIDIOC_QUERYBUF) Unable to query buffer "
                                     << strerror(errno);
                    base::LogError() << "         try with read method instead";
                    return E_QUERYBUF_ERR;
                }

                if (context->buf.length <= 0)
                    base::LogError() << "V4L2_CORE: (VIDIOC_QUERYBUF) - buffer length is "
                                     << context->buf.length;

                context->buff_length[i] = context->buf.length;
                context->buff_offset[i] = context->buf.m.offset;
            }
            // map the new buffers
            if (map_buff(context) != 0) {
                ret = E_MMAP_ERR;
            }
            break;
    }
    // for (int i = 0; i < context->frame_queue_size; ++i) {
    //     context->frame_queue[i].raw_frame_max_size = context->buf.length;
    // }

    return ret;
}

/*
 * Queue Buffers
 * args:
 *   context - pointer to V4L2Context
 *
 * returns: error code  (0- E_OK)
 */
static int queue_buff(V4L2Context *context) {
    base::LogDebug() << "query v4l2 buffers";

    int ret = E_OK;
    switch (context->cap_meth) {
        case IO_READ:
            break;

        case IO_MMAP:
        default:
            for (int i = 0; i < NB_BUFFER; ++i) {
                memset(&context->buf, 0, sizeof(struct v4l2_buffer));
                context->buf.index = i;
                context->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                // context->buf.flags = V4L2_BUF_FLAG_TIMECODE;
                // context->buf.timecode = context->timecode;
                // context->buf.timestamp.tv_sec = 0;
                // context->buf.timestamp.tv_usec = 0;
                context->buf.memory = V4L2_MEMORY_MMAP;
                ret = xioctl(context->fd, VIDIOC_QBUF, &context->buf);
                if (ret < 0) {
                    base::LogError() << "(VIDIOC_QBUF) Unable to queue buffer: " << strerror(errno);
                    return E_QBUF_ERR;
                }
            }
            context->buf.index = 0; /*reset index*/
    }
    return ret;
}

/*
 * Set device video stream format
 * args:
 *   context - pointer to v4l2 context
 *   width - requested video frame width
 *   height - requested video frame height
 *   pixelformat - requested v4l2 pixelformat
 *
 * returns: error code ( E_OK)
 */
int set_video_stream_format(V4L2Context *context, int32_t width, int32_t height, int pixelformat) {
    int ret = E_OK;

    int old_format = context->requested_fmt;

    context->requested_fmt = pixelformat;

    uint8_t stream_status = context->streaming;

    if (stream_status == STRM_OK) {
        v4l2core_stop_stream(context);
    }

    if (context->requested_fmt == V4L2_PIX_FMT_H264) {
        base::LogDebug() << "Requested H264 stream is supported through muxed MJPG";
        pixelformat = V4L2_PIX_FMT_MJPEG;
    }

    context->format.fmt.pix.pixelformat = pixelformat;
    context->format.fmt.pix.width = width;
    context->format.fmt.pix.height = height;

    /*override field and type entries*/
    context->format.fmt.pix.field = V4L2_FIELD_ANY;
    context->format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    ret = xioctl(context->fd, VIDIOC_S_FMT, &context->format);

    if (ret != 0) {
        base::LogError() << "(VIDIOC_S_FORMAT) Unable to set format: " << strerror(errno);
        // reset to old format
        context->requested_fmt = old_format;
        return E_FORMAT_ERR;
    }

    if ((context->format.fmt.pix.width != width) || (context->format.fmt.pix.height != height)) {
        base::LogError() << "Requested resolution unavailable: got width "
                         << context->format.fmt.pix.width << " height "
                         << context->format.fmt.pix.height;
    }

    // /*
    //  * try to alloc frame buffers based on requested format
    // */
    // ret = alloc_v4l2_frames(vd);
    // if (ret != E_OK) {
    //     fprintf(stderr, "V4L2_CORE: Frame allocation returned error (%i)\n", ret);
    //     return E_ALLOC_ERR;
    // }

    switch (context->cap_meth) {
        case IO_READ: /*allocate buffer for read*/
            memset(&context->buf, 0, sizeof(struct v4l2_buffer));
            context->buf.length = (context->format.fmt.pix.width) *
                                  (context->format.fmt.pix.height) * 3;  // worst case (rgb)
            context->mem[context->buf.index] = calloc(context->buf.length, sizeof(uint8_t));
            if (context->mem[context->buf.index] == NULL) {
                base::LogError() << "FATAL memory allocation failure (try_video_stream_format): "
                                 << strerror(errno);
                exit(-1);
            }
            break;

        case IO_MMAP:
        default:
            /* request buffers */
            memset(&context->rb, 0, sizeof(struct v4l2_requestbuffers));
            context->rb.count = NB_BUFFER;
            context->rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            context->rb.memory = V4L2_MEMORY_MMAP;

            ret = xioctl(context->fd, VIDIOC_REQBUFS, &context->rb);

            if (ret < 0) {
                base::LogError() << "(VIDIOC_REQBUFS) Unable to allocate buffers: "
                                 << strerror(errno);
                return E_REQBUFS_ERR;
            }
            /* map the buffers */
            if (query_buff(context)) {
                base::LogError() << "(VIDIOC_QBUFS) Unable to query buffers: " << strerror(errno);
                /*
                * delete requested buffers
                 * no need to unmap as mmap failed for sure
                */
                base::LogDebug() << "V4L2_CORE: cleaning requestbuffers";
                memset(&context->rb, 0, sizeof(struct v4l2_requestbuffers));
                context->rb.count = 0;
                context->rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                context->rb.memory = V4L2_MEMORY_MMAP;
                if (xioctl(context->fd, VIDIOC_REQBUFS, &context->rb) < 0) {
                    base::LogError() << "V4L2_CORE: (VIDIOC_REQBUFS) Unable to delete buffers: "
                                     << strerror(errno);
                }

                return E_QUERYBUF_ERR;
            }

            /* Queue the buffers */
            if (queue_buff(context)) {
                base::LogError() << "V4L2_CORE: (VIDIOC_QBUFS) Unable to queue buffers: "
                                 << strerror(errno);
                /*delete requested buffers */
                base::LogDebug() << "V4L2_CORE: cleaning requestbuffers";
                unmap_buff(context);
                memset(&context->rb, 0, sizeof(struct v4l2_requestbuffers));
                context->rb.count = 0;
                context->rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                context->rb.memory = V4L2_MEMORY_MMAP;
                if (xioctl(context->fd, VIDIOC_REQBUFS, &context->rb) < 0)
                    base::LogError() << "V4L2_CORE: (VIDIOC_REQBUFS) Unable to delete buffers: "
                                     << strerror(errno);
                return E_QBUF_ERR;
            }
    }

    // /*this locks the mutex (can't be called while the mutex is being locked)*/
    // v4l2core_request_framerate_update(vd);

    // if (stream_status == STRM_OK) v4l2core_start_stream(vd);

    // /*update the current framerate for the device*/
    // v4l2core_get_framerate(vd);

    return E_OK;
}

}  // namespace uvc