#include "v4l2_util.h"

#include <base/log.h>
#include <libv4l2.h>
#include <string.h>
#include <sys/ioctl.h>

namespace uvc {

bool disable_libv4l2 = false;

/*
 * set ioctl retries to 4
 */
#define IOCTL_RETRY 4

/*
 * ioctl with a number of retries in the case of I/O failure
 * args:
 *   fd - device descriptor
 *   IOCTL_X - ioctl reference
 *   arg - pointer to ioctl data
 *
 * asserts:
 *   none
 *
 * returns - ioctl result
 */
int xioctl(int fd, int IOCTL_X, void *arg) {
    int ret = 0;
    int tries = IOCTL_RETRY;
    do {
        if (!disable_libv4l2) {
            ret = v4l2_ioctl(fd, IOCTL_X, arg);
        } else {
            ret = ioctl(fd, IOCTL_X, arg);
        }
    } while (ret && tries-- && ((errno == EINTR) || (errno == EAGAIN) || (errno == ETIMEDOUT)));

    if (ret && (tries <= 0))
        base::LogError() << "ioctl (" << IOCTL_X << ") retried " << IOCTL_RETRY
                         << " times - giving up:" << strerror(errno);

    return (ret);
}

}  // namespace uvc