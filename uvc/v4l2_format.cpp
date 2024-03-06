#include "v4l2_format.h"

#include <base/log.h>
#include <string.h>

#include "v4l2_define.h"
#include "v4l2_util.h"

namespace uvc {

static uint32_t decoder_supported_formats[] = {
    V4L2_PIX_FMT_YUYV, V4L2_PIX_FMT_MJPEG, V4L2_PIX_FMT_JPEG, V4L2_PIX_FMT_H264, V4L2_PIX_FMT_YVYU,
    V4L2_PIX_FMT_UYVY, V4L2_PIX_FMT_VYUY, V4L2_PIX_FMT_YYUV, V4L2_PIX_FMT_YUV444,
    V4L2_PIX_FMT_YUV555, V4L2_PIX_FMT_YUV565, V4L2_PIX_FMT_YUV32, V4L2_PIX_FMT_Y41P,
    V4L2_PIX_FMT_GREY, V4L2_PIX_FMT_Y10BPACK, V4L2_PIX_FMT_Y16,
#ifdef V4L2_PIX_FMT_Y16_BE
    V4L2_PIX_FMT_Y16_BE,
#endif
    V4L2_PIX_FMT_YUV420, V4L2_PIX_FMT_YUV422P, V4L2_PIX_FMT_YVU420, V4L2_PIX_FMT_NV12,
    V4L2_PIX_FMT_NV21, V4L2_PIX_FMT_NV16, V4L2_PIX_FMT_NV61, V4L2_PIX_FMT_NV24, V4L2_PIX_FMT_NV42,
    V4L2_PIX_FMT_SPCA501, V4L2_PIX_FMT_SPCA505, V4L2_PIX_FMT_SPCA508, V4L2_PIX_FMT_SGBRG8,
    V4L2_PIX_FMT_SGRBG8, V4L2_PIX_FMT_SBGGR8, V4L2_PIX_FMT_SRGGB8, V4L2_PIX_FMT_RGB24,
    V4L2_PIX_FMT_BGR24, V4L2_PIX_FMT_RGB332, V4L2_PIX_FMT_RGB565, V4L2_PIX_FMT_RGB565X,
    V4L2_PIX_FMT_RGB444, V4L2_PIX_FMT_RGB555, V4L2_PIX_FMT_RGB555X, V4L2_PIX_FMT_BGR666,
    V4L2_PIX_FMT_BGR32, V4L2_PIX_FMT_RGB32,
#ifdef V4L2_PIX_FMT_ARGB444
    V4L2_PIX_FMT_ARGB444, V4L2_PIX_FMT_XRGB444,
#endif
#ifdef V4L2_PIX_FMT_ARGB555
    V4L2_PIX_FMT_ARGB555, V4L2_PIX_FMT_XRGB555,
#endif
#ifdef V4L2_PIX_FMT_ARGB555X
    V4L2_PIX_FMT_ARGB555X, V4L2_PIX_FMT_XRGB555X,
#endif
#ifdef V4L2_PIX_FMT_ABGR32
    V4L2_PIX_FMT_ABGR32, V4L2_PIX_FMT_XBGR32,
#endif
#ifdef V4L2_PIX_FMT_ARGB32
    V4L2_PIX_FMT_ARGB32, V4L2_PIX_FMT_XRGB32,
#endif
    /*last one (zero terminated)*/
    0};

/*
 * check pixelformat against decoder support formats
 * args:
 *    pixelformat - v4l2 pixelformat
 *
 * asserts:
 *    none
 *
 * returns: TRUE(1) if format is supported
 *          FALSE(0) if not
 */
uint8_t can_decode_format(uint32_t pixelformat) {
    int i = 0;
    uint32_t sup_fmt = 0;

    do {
        sup_fmt = decoder_supported_formats[i];

        if (pixelformat == sup_fmt) {
            return TRUE;
        }
        i++;
    } while (sup_fmt); /*last format is always 0*/
    return FALSE;
}

/*
 * enumerate frame framerate
 * args:
 *   context - pointer to V4L2Context
 *   pixfmt - v4l2 pixel format that we want to list framerate for
 *   stream_capability - stream capability that we want to list framerate for
 *
 * returns 0 if enumeration succeded or errno otherwise
 */
static int enum_frame_framerate(V4L2Context *context, uint32_t pixfmt,
                                V4l2StreamCapability &stream_capability) {
    stream_capability.framerates.clear();

    struct v4l2_frmivalenum frame_ivalue_enum;
    memset(&frame_ivalue_enum, 0, sizeof(frame_ivalue_enum));
    frame_ivalue_enum.index = 0;
    frame_ivalue_enum.pixel_format = pixfmt;
    frame_ivalue_enum.width = stream_capability.width;
    frame_ivalue_enum.height = stream_capability.height;

    base::LogDebug() << "\tTime interval between frame: ";
    int ret = 0;
    while ((ret = xioctl(context->fd, VIDIOC_ENUM_FRAMEINTERVALS, &frame_ivalue_enum)) == 0) {
        frame_ivalue_enum.index++;
        if (frame_ivalue_enum.type == V4L2_FRMIVAL_TYPE_DISCRETE) {
            base::LogDebug() << "\t\t" << frame_ivalue_enum.discrete.numerator << "/"
                             << frame_ivalue_enum.discrete.denominator;
            V4L2Rational framerate;
            framerate.numerator = frame_ivalue_enum.discrete.numerator;
            framerate.denominator = frame_ivalue_enum.discrete.denominator;
            stream_capability.framerates.push_back(framerate);
        } else if (frame_ivalue_enum.type == V4L2_FRMIVAL_TYPE_CONTINUOUS) {
            base::LogDebug() << "\t\t{min { " << frame_ivalue_enum.stepwise.min.numerator << "/"
                             << frame_ivalue_enum.stepwise.min.numerator << " } .. max { "
                             << frame_ivalue_enum.stepwise.max.denominator << "/"
                             << frame_ivalue_enum.stepwise.max.denominator << " }";
            break;
        } else if (frame_ivalue_enum.type == V4L2_FRMIVAL_TYPE_STEPWISE) {
            base::LogDebug() << "\t\t{min { " << frame_ivalue_enum.stepwise.min.numerator << "/"
                             << frame_ivalue_enum.stepwise.min.denominator << "} .. max { "
                             << frame_ivalue_enum.stepwise.max.numerator << "/"
                             << frame_ivalue_enum.stepwise.max.denominator << " "
                             << frame_ivalue_enum.stepwise.step.numerator << "/"
                             << frame_ivalue_enum.stepwise.step.denominator << "}";
            break;
        }
    }

    if (ret != 0 && errno != EINVAL) {
        base::LogError() << "(VIDIOC_ENUM_FRAMEINTERVALS) Error enumerating frame intervals";
        return errno;
    }
    return 0;
}

/*
 * enumerate frame sizes (width and height)
 * args:
 *   context - pointer to V4L2Context
 *   pixfmt - v4l2 pixel format that we want to list frame sizes for
 *   format_index - current index of format list
 *
 * returns 0 if enumeration succeded or errno otherwise
 */
static int enum_frame_sizes(V4L2Context *context, uint32_t pixfmt, int format_index) {
    int frame_size_index = 0; /*index for supported sizes*/
    context->stream_formats[format_index].list_stream_cap.clear();

    struct v4l2_frmsizeenum frame_size_enum;
    memset(&frame_size_enum, 0, sizeof(frame_size_enum));
    frame_size_enum.pixel_format = pixfmt;

    int ret = 0;
    while ((ret = xioctl(context->fd, VIDIOC_ENUM_FRAMESIZES, &frame_size_enum)) == 0) {
        frame_size_enum.index++;
        if (frame_size_enum.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
            base::LogDebug() << "{ discrete: width = " << frame_size_enum.discrete.width
                             << ", height = " << frame_size_enum.discrete.height << "}";

            V4l2StreamCapability stream_capability;

            stream_capability.width = frame_size_enum.discrete.width;
            stream_capability.height = frame_size_enum.discrete.height;

            ret = enum_frame_framerate(context, pixfmt, stream_capability);
            context->stream_formats[format_index].list_stream_cap.emplace_back(stream_capability);
            if (ret != 0) {
                base::LogError() << "Unable to enumerate framerate " << strerror(ret);
            }
        } else if (frame_size_enum.type == V4L2_FRMSIZE_TYPE_CONTINUOUS ||
                   frame_size_enum.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
            if (frame_size_enum.type == V4L2_FRMSIZE_TYPE_CONTINUOUS) {
                base::LogDebug() << "{ continuous: min { width = "
                                 << frame_size_enum.stepwise.min_width
                                 << ", height = " << frame_size_enum.stepwise.min_height << "}"
                                 << " .. max { width = " << frame_size_enum.stepwise.max_width
                                 << ", height = " << frame_size_enum.stepwise.max_height << " } }";
            } else {
                base::LogDebug() << "{ stepwise: min { width = "
                                 << frame_size_enum.stepwise.min_width
                                 << ", height = " << frame_size_enum.stepwise.min_height << " } .. "
                                 << "max { width = " << frame_size_enum.stepwise.max_width
                                 << ", height = " << frame_size_enum.stepwise.max_height << "} .. "
                                 << "stepsize { width = " << frame_size_enum.stepwise.step_width
                                 << ", height = " << frame_size_enum.stepwise.step_height << " } }";
            }

            V4l2StreamCapability stream_capability;

            stream_capability.width = frame_size_enum.stepwise.min_width;
            stream_capability.height = frame_size_enum.stepwise.min_height;

            ret = enum_frame_framerate(context, pixfmt, stream_capability);
            context->stream_formats[format_index].list_stream_cap.emplace_back(stream_capability);
            if (ret != 0) {
                base::LogError() << "Unable to enumerate framerate " << strerror(ret);
            }

            stream_capability.width = frame_size_enum.stepwise.max_width;
            stream_capability.height = frame_size_enum.stepwise.max_height;

            ret = enum_frame_framerate(context, pixfmt, stream_capability);

            if (ret != 0) {
                base::LogError() << "Unable to enumerate frame sizes " << strerror(ret);
            }
        } else {
            base::LogError() << "V4L2_CORE: fsize.type not supported: " << frame_size_enum.type;
        }
    }

    if (ret != 0 && errno != EINVAL) {
        base::LogError() << "(VIDIOC_ENUM_FRAMESIZES) - Error enumerating frame sizes";
        return errno;
    }
    return 0;
}

/*
 * enumerate frame formats (pixelformats, resolutions and fps)
 * and creates list in vd->list_stream_formats
 * args:
 *   context - pointer to V4L2Context
 *
 * returns: 0 (E_OK) if enumeration succeded or error otherwise
 */
int enum_frame_formats(V4L2Context *context) {
    int valid_formats = 0; /*number of valid formats found (with valid frame sizes)*/

    struct v4l2_fmtdesc format_description;
    memset(&format_description, 0, sizeof(format_description));
    format_description.index = 0;
    format_description.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    int ret = 0;
    while ((ret = xioctl(context->fd, VIDIOC_ENUM_FMT, &format_description)) == 0) {
        V4L2StreamFormat new_stream_format;
        uint8_t dec_support = can_decode_format(format_description.pixelformat);
        uint32_t pix_format = format_description.pixelformat;

        format_description.index++;
        if ((format_description.pixelformat & (1 << 31)) != 0) {
            pix_format &= ~(1 << 31);  //need to fix fourcc string
            base::LogDebug() << " valid pixel format " << format_description.description;
        } else {
            base::LogDebug() << " valid pixel format " << format_description.description;
        }

        if (!dec_support) {
            base::LogWarn() << "- FORMAT NOT SUPPORTED BY DECODER -";
        }

        new_stream_format.dec_support = dec_support;
        new_stream_format.pixel_format = format_description.pixelformat;
        if ((format_description.pixelformat & (1 << 31)) != 0) {  //be format flag
            pix_format &= ~(1 << 31);                             //need to fix fourcc string
        }
        snprintf(new_stream_format.fourcc, 5, "%c%c%c%c", pix_format & 0xFF,
                 (pix_format >> 8) & 0xFF, (pix_format >> 16) & 0xFF, (pix_format >> 24) & 0xFF);
        strncpy(new_stream_format.description, (char *)format_description.description, 31);
        context->stream_formats.push_back(new_stream_format);
        //enumerate frame sizes
        ret = enum_frame_sizes(context, format_description.pixelformat,
                               context->stream_formats.size() - 1);
        if (ret != E_OK) {
            base::LogError() << "Unable to enumerate frame sizes : " << ret;
        }
        if (dec_support && !ret) {
            valid_formats++; /*the format can be decoded and it has valid frame sizes*/
        }
    }

    if (errno != EINVAL) {
        base::LogError() << "(VIDIOC_ENUM_FMT) - Error enumerating frame formats: "
                         << strerror(errno);
    }

    if (valid_formats > 0) {
        return E_OK;
    } else {
        return E_DEVICE_ERR;
    }
}

}  // namespace uvc