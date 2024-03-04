#pragma once

#include <string>

#include "v4l2_context.h"

namespace uvc {

class V4L2Control final {
public:
    /*
    * enumerate device (read/write) controls
    */
    int enumerate_control(V4L2Context *context);
};

}  // namespace uvc