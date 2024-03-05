#pragma once

#include "v4l2_context.h"

namespace uvc {

/*
 * enumerate frame formats (pixelformats, resolutions and fps)
 * and creates list in vd->list_stream_formats
 * args:
 *   context - pointer to V4L2Context
 *
 * returns: 0 (E_OK) if enumeration succeded or error otherwise
 */
int enum_frame_formats(V4L2Context *context);

}  // namespace uvc