#include <uvc/v4l2_device.h>

#include <iostream>

int main(int argc, const char *argv[]) {
    uvc::V4l2Device device;
    device.init_device_list();
    return 0;
}