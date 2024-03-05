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
 * enumerate frame formats (pixelformats, resolutions and fps)
 * and creates list in vd->list_stream_formats
 * args:
 *   context - pointer to V4L2Context
 *
 * returns: 0 (E_OK) if enumeration succeded or error otherwise
 */
int enum_frame_formats(V4L2Context *context) {
    int ret = E_OK;

    int fmtind = 0;
    int valid_formats = 0; /*number of valid formats found (with valid frame sizes)*/
    struct v4l2_fmtdesc fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.index = 0;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    while ((ret = xioctl(context->fd, VIDIOC_ENUM_FMT, &fmt)) == 0) {
        V4L2StreamFormat *stream_format = (V4L2StreamFormat *)calloc(1, sizeof(V4L2StreamFormat));
        if (stream_format == NULL) {
            base::LogError() << "FATAL memory allocation failure (enum_frame_formats): ",
                strerror(errno);
            exit(-1);
        }

        uint8_t dec_support = can_decode_format(fmt.pixelformat);

        uint32_t pix_format = fmt.pixelformat;

        fmt.index++;
        if ((fmt.pixelformat & (1 << 31)) != 0) {
            pix_format &= ~(1 << 31);  //need to fix fourcc string
            base::LogDebug() << " valid pixel format " << fmt.description;
        } else {
            base::LogDebug() << " valid pixel format " << fmt.description;
        }

        if (!dec_support) {
            base::LogWarn() << "    - FORMAT NOT SUPPORTED BY DECODER -";
        }

        stream_format->dec_support = dec_support;
        stream_format->format = fmt.pixelformat;
        context->stream_formats.push_back(stream_format);
        if ((fmt.pixelformat & (1 << 31)) != 0) {  //be format flag
            pix_format &= ~(1 << 31);              //need to fix fourcc string
        }
        snprintf(stream_format->fourcc, 5, "%c%c%c%c", pix_format & 0xFF, (pix_format >> 8) & 0xFF,
                 (pix_format >> 16) & 0xFF, (pix_format >> 24) & 0xFF);
        strncpy(stream_format->description, (char *)fmt.description, 31);
        // //enumerate frame sizes
        // ret = enum_frame_sizes(vd, fmt.pixelformat, fmtind);
        // if (ret != 0)
        //     fprintf(stderr, "v4L2_CORE: Unable to enumerate frame sizes :%s\n", strerror(ret));

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