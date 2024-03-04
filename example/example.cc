#include <uvc/v4l2_device.h>

#include <iostream>

int main(int argc, const char *argv[]) {
    uvc::V4l2Device device;
    std::cout << device.enum_devices() << std::endl;
    return 0;
}