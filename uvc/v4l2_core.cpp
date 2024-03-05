#include "v4l2_core.h"

#include <base/log.h>
#include <fcntl.h>
#include <libintl.h>
#include <libv4l2.h>
#include <string.h>

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

}  // namespace uvc