#include <base/log.h>

#include <thread>

#include "uvc/v4l2_core.h"
#include "uvc/v4l2_device.h"

int main(int argc, const char *argv[]) {
    uvc::V4l2Device device;
    device.init_device_list();

    if (device.get_device_count() > 0) {
        auto device_sys_data = device.get_device_sys_data(0);
        base::LogDebug() << "begin open device " << device_sys_data.device;
        auto context = uvc::v4l2core_init_dev(device_sys_data.device.c_str());

        auto ret = v4l2core_stop_stream(context);
        ret = v4l2core_start_stream(context);
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    device.free_device_list();
    return 0;
}