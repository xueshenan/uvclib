#pragma once

#include <linux/videodev2.h>
#include <stdint.h>

#include <string>
#include <vector>

namespace uvc {

/*
 * LOGITECH Dynamic controls defs
 */

#define V4L2_CID_BASE_EXTCTR 0x0A046D01
#define V4L2_CID_BASE_LOGITECH V4L2_CID_BASE_EXTCTR
//#define V4L2_CID_PAN_RELATIVE_LOGITECH		V4L2_CID_BASE_LOGITECH
//#define V4L2_CID_TILT_RELATIVE_LOGITECH		V4L2_CID_BASE_LOGITECH+1
#define V4L2_CID_PANTILT_RESET_LOGITECH V4L2_CID_BASE_LOGITECH + 2

/*this should realy be replaced by V4L2_CID_FOCUS_ABSOLUTE in libwebcam*/
#define V4L2_CID_FOCUS_LOGITECH V4L2_CID_BASE_LOGITECH + 3
#define V4L2_CID_LED1_MODE_LOGITECH V4L2_CID_BASE_LOGITECH + 4
#define V4L2_CID_LED1_FREQUENCY_LOGITECH V4L2_CID_BASE_LOGITECH + 5
#define V4L2_CID_DISABLE_PROCESSING_LOGITECH V4L2_CID_BASE_LOGITECH + 0x70
#define V4L2_CID_RAW_BITS_PER_PIXEL_LOGITECH V4L2_CID_BASE_LOGITECH + 0x71
#define V4L2_CID_LAST_EXTCTR V4L2_CID_RAW_BITS_PER_PIXEL_LOGITECH

/* gview_v4l2core */
#define GETTEXT_PACKAGE_V4L2CORE "gview_v4l2core"
#define PACKAGE_LOCALE_DIR "/home/tbago/work/local/"  // TODO: need change

/*
 * IO methods
 */
#define IO_MMAP 1
#define IO_READ 2

/*
 * stream status codes
 */
#define STRM_STOP (0)
#define STRM_REQ_STOP (1)
#define STRM_OK (2)

static int frame_queue_size = 1; /*just one frame in queue (enough for a single thread)*/

/*
 * v4l2 control data
 */
struct V4L2ControlData {
    struct v4l2_queryctrl control;
    struct v4l2_querymenu *menu;
    int32_t cclass;  //don't use 'class' to avoid issues with c++
    int32_t value;   //also used for string max size
    int64_t value64;
    char *string;

    /*localization*/
    std::string name; /*gettext translated name*/
    int menu_entries;
    char **menu_entry; /*gettext translated menu entry name*/
};

struct V4L2Rational {
    int32_t numerator;
    int32_t denominator;
};
/*
 * v4l2 stream capability data
 */
struct V4l2StreamCapability {
    int32_t width;                         //width
    int32_t height;                        //height
    std::vector<V4L2Rational> framerates;  //list of framerates, should be 1 in almost all cases
};

/*
 * v4l2 stream format data
 */
struct V4L2StreamFormat {
    uint8_t dec_support;   //decoder support (1-supported; 0-not supported)
    int pixel_format;      //v4l2 pixel format
    char fourcc[5];        //corresponding fourcc (mode)
    char description[32];  //format description
    std::vector<V4l2StreamCapability> list_stream_cap;  //list of stream capabilities for format
};

struct V4L2Context {
    int fd;
    std::string videodevice;  // video device string (e.g. "/dev/video0")

    int cap_meth;                                  // capture method: IO_READ or IO_MMAP
    std::vector<V4L2StreamFormat> stream_formats;  //list of available stream formats

    struct v4l2_capability cap;            // v4l2 capability struct
    struct v4l2_format format;             // v4l2 format struct
    struct v4l2_buffer buf;                // v4l2 buffer struct
    struct v4l2_requestbuffers rb;         // v4l2 request buffers struct
    struct v4l2_streamparm streamparm;     // v4l2 stream parameters struct
    struct v4l2_event_subscription evsub;  // v4l2 event subscription struct

    uint8_t streaming;  // flag device stream : STRM_STOP ; STRM_REQ_STOP; STRM_OK
    int has_focus_control_id;  //it's set to control id if a focus control is available (enables software autofocus)
    int has_pantilt_control_id;  //it's set to 1 if a pan/tilt control is available
    uint8_t pantilt_unit_id;     //logitech peripheral V3 unit id (if any)

    int fps_num;    //fps numerator
    int fps_denom;  //fps denominator

    double real_fps;  //real fps (calculated from number of captured frames)

    int frame_queue_size;  //size of frame queue (in frames)

    uint8_t h264_unit_id;  // uvc h264 unit id, if <= 0 then uvc h264 is not supported
    uint8_t
        h264_no_probe_default;  // flag core to use the preset h264_config_probe_req data (don't reset to default before commit)
    // uvcx_video_config_probe_commit_t h264_config_probe_req;  //probe commit struct for h264 streams
    uint8_t *h264_last_IDR;  // last IDR frame retrieved from uvc h264 stream
    int h264_last_IDR_size;  // last IDR frame size
    uint8_t *h264_SPS;       // h264 SPS info
    uint16_t h264_SPS_size;  // SPS size
    uint8_t *h264_PPS;       // h264 PPS info
    uint16_t h264_PPS_size;  // PPS size

    int pan_step;   //pan step for relative pan tilt controls (logitech sphere/orbit/BCC950)
    int tilt_step;  //tilt step for relative pan tilt controls (logitech sphere/orbit/BCC950)

    std::vector<V4L2ControlData *> list_device_controls;  //list of available device controls
};

}  // namespace uvc