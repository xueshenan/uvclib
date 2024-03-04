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
    * Initiate the device list (with udev monitoring)
    */
    void init_device_list();
    int32_t get_device_count() const { return _dev_sys_datas.size(); };
    /*
    * get the device sys data for index
    */
    V4L2DeviceSysData get_device_sys_data(int index);
private:
    /*
    * enumerate available v4l2 devices and creates list 
    * return: number of valid devices
    */
    int enum_devices();
private:
    struct udev *_udev;                  // pointer to a udev struct (libudev)
    struct udev_monitor *_udev_monitor;  // udev monitor
    int _udev_fd;                        // udev monitor file descriptor
    std::vector<V4L2DeviceSysData> _dev_sys_datas;
};

}  // namespace uvc