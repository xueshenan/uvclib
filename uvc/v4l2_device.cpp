#include "v4l2_device.h"

#include <base/log.h>
#include <fcntl.h>
#include <libv4l2.h>
#include <linux/videodev2.h>
#include <string.h>

#include "v4l2_util.h"

namespace uvc {

void V4l2Device::init_device_list() {
    /* Create a udev object */
    _udev = udev_new();
    /*start udev device monitoring*/
    /* Set up a monitor to monitor v4l2 devices */
    if (_udev != NULL) {
        _udev_monitor = udev_monitor_new_from_netlink(_udev, "udev");
        udev_monitor_filter_add_match_subsystem_devtype(_udev_monitor, "video4linux", NULL);
        udev_monitor_enable_receiving(_udev_monitor);
        /* Get the file descriptor (fd) for the monitor */
        _udev_fd = udev_monitor_get_fd(_udev_monitor);

        enum_devices();
    } else {
        base::LogError() << "Create new udev failed";
    }
}

V4L2DeviceSysData V4l2Device::get_device_sys_data(int index) {
    V4L2DeviceSysData sys_data;
    if (index < 0 || index >= _dev_sys_datas.size()) {
        base::LogError() << "Invalid index " << index;
        return sys_data;
    }

    return _dev_sys_datas[index];
}

void V4l2Device::free_device_list() {
    _dev_sys_datas.clear();

    if (_udev != NULL) {
        udev_unref(_udev);
    }
}

int V4l2Device::enum_devices() {
    _dev_sys_datas.clear();
    /* Create a list of the devices in the 'v4l2' subsystem. */
    struct udev_enumerate *enumerate = udev_enumerate_new(_udev);
    udev_enumerate_add_match_subsystem(enumerate, "video4linux");
    udev_enumerate_scan_devices(enumerate);
    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);

    /*
     * For each item enumerated, print out its information.
     * udev_list_entry_foreach is a macro which expands to
     * a loop. The loop will be executed for each member in
     * devices, setting dev_list_entry to a list entry
     * which contains the device's path in /sys.
     */
    struct udev_list_entry *dev_list_entry;
    udev_list_entry_foreach(dev_list_entry, devices) {
        /*
         * Get the filename of the /sys entry for the device
         * and create a udev_device object (dev) representing it
         */
        const char *path = udev_list_entry_get_name(dev_list_entry);
        struct udev_device *dev = udev_device_new_from_syspath(_udev, path);

        /* usb_device_get_devnode() returns the path to the device node
            itself in /dev. */
        const char *v4l2_device = udev_device_get_devnode(dev);
        base::LogDebug() << " Found device node path: " << v4l2_device;

        int fd = 0;
        /* open the device and query the capabilities */
        if ((fd = v4l2_open(v4l2_device, O_RDWR | O_NONBLOCK, 0)) < 0) {
            base::LogError() << "Error opening V4L2 interface for " << v4l2_device;
            v4l2_close(fd);
            continue; /*next dir entry*/
        }

        struct v4l2_capability v4l2_cap;
        if (xioctl(fd, VIDIOC_QUERYCAP, &v4l2_cap) < 0) {
            base::LogError() << "VIDIOC_QUERYCAP error: " << strerror(errno);
            base::LogError() << "Couldn't query device " << v4l2_device;
            v4l2_close(fd);
            continue; /*next dir entry*/
        }
        v4l2_close(fd);

        uint32_t caps;
        if (v4l2_cap.capabilities & V4L2_CAP_DEVICE_CAPS) {
            caps = v4l2_cap.device_caps;
        } else {
            caps = v4l2_cap.capabilities;
        }

        if (!(caps & V4L2_CAP_VIDEO_CAPTURE)) {
            base::LogWarn() << "Device " << v4l2_device << " not support video capture";
            continue;
        }

        V4L2DeviceSysData new_device;

        new_device.device = v4l2_device;
        new_device.name = reinterpret_cast<char *>(v4l2_cap.card);
        new_device.driver = reinterpret_cast<char *>(v4l2_cap.driver);
        new_device.location = reinterpret_cast<char *>(v4l2_cap.bus_info);
        new_device.valid = 1;
        new_device.current = 0;

        /* The device pointed to by dev contains information about
            the v4l2 device. In order to get information about the
            USB device, get the parent device with the
            subsystem/devtype pair of "usb"/"usb_device". This will
            be several levels up the tree, but the function will find
            it.*/
        dev = udev_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device");
        if (dev == NULL) {
            base::LogError() << "Unable to find parent usb device.";
            continue;
        }

        /* From here, we can call get_sysattr_value() for each file
            in the device's /sys entry. The strings passed into these
            functions (idProduct, idVendor, serial, etc.) correspond
            directly to the files in the directory which represents
            the USB device. Note that USB strings are Unicode, UCS2
            encoded, but the strings returned from
            udev_device_get_sysattr_value() are UTF-8 encoded. */
        base::LogDebug() << "\tVID: " << udev_device_get_sysattr_value(dev, "idVendor");
        base::LogDebug() << "\tPID: " << udev_device_get_sysattr_value(dev, "idProduct");
        base::LogDebug() << "\tmanufacturer: "
                         << udev_device_get_sysattr_value(dev, "manufacturer");
        base::LogDebug() << "\tproduct:" << udev_device_get_sysattr_value(dev, "product");
        base::LogDebug() << "\tserial: " << udev_device_get_sysattr_value(dev, "serial");
        base::LogDebug() << "\tbusnum: " << udev_device_get_sysattr_value(dev, "busnum");
        base::LogDebug() << "\tdevnum: ", udev_device_get_sysattr_value(dev, "devnum");

        new_device.vendor = strtoull(udev_device_get_sysattr_value(dev, "idVendor"), NULL, 16);
        new_device.product = strtoull(udev_device_get_sysattr_value(dev, "idProduct"), NULL, 16);
        new_device.busnum = strtoull(udev_device_get_sysattr_value(dev, "busnum"), NULL, 10);
        new_device.devnum = strtoull(udev_device_get_sysattr_value(dev, "devnum"), NULL, 10);

        _dev_sys_datas.emplace_back(new_device);

        udev_device_unref(dev);
    }

    /* Free the enumerator object */
    udev_enumerate_unref(enumerate);

    return _dev_sys_datas.size();
}

}  // namespace uvc