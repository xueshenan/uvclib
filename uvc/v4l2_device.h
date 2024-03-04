#pragma once

#include <string>
#include <vector>
extern "C" {
#include <libudev.h>
}

namespace uvc {

/*
 * v4l2 device system data
 */
struct V4L2DeviceSysData {
    std::string device;
    std::string name;
    std::string driver;
    std::string location;
    uint32_t vendor;
    uint32_t product;
    int32_t valid;
    int32_t current;
    uint64_t busnum;
    uint64_t devnum;
};

class V4l2Device final {
public:
    /*
    * enumerate available v4l2 devices and creates list 
    * return: number of valid devices
    */
    int enum_devices();
private:
    struct udev *_udev;  // pointer to a udev struct (libudev)
    std::vector<V4L2DeviceSysData> _dev_sys_datas;
};

}  // namespace uvc