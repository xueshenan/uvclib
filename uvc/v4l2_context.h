#pragma once

#include <linux/videodev2.h>

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

struct V4L2Context {
    int fd;

    struct v4l2_event_subscription evsub;  // v4l2 event subscription struct

    int has_focus_control_id;  //it's set to control id if a focus control is available (enables software autofocus)
    int has_pantilt_control_id;  //it's set to 1 if a pan/tilt control is available
    uint8_t pantilt_unit_id;     //logitech peripheral V3 unit id (if any)

    std::vector<V4L2ControlData *> list_device_controls;  //list of available device controls
};

}  // namespace uvc