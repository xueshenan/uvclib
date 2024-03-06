#pragma once

#include "v4l2_context.h"

namespace uvc {

/*
 * Initiate video device handler with default values
 * args:
 *   device - device name (e.g: "/dev/video0")
 *
 * returns: pointer to V4L2Context handler (or NULL on error)
 */
V4L2Context *v4l2core_init_dev(const char *device);

/*
 * Start video stream
 * args:
 *   vd - pointer to V4L2Context
 *
 * returns: VIDIOC_STREAMON ioctl result (E_OK or E_STREAMON_ERR)
*/
int v4l2core_start_stream(V4L2Context *context);

/*
 * Stops the video stream
 * args:
 *   context - pointer to v4l2 context
 *
 * returns: VIDIOC_STREAMON ioctl result (E_OK)
*/
int v4l2core_stop_stream(V4L2Context *context);

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
int set_video_stream_format(V4L2Context *context, int32_t width, int32_t height, int pixelformat);
}  // namespace uvc