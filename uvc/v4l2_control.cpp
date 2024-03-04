#include "v4l2_control.h"

#include <assert.h>
#include <base/log.h>
#include <libintl.h>
#include <libv4l2.h>
#include <string.h>

#include "v4l2_util.h"

namespace uvc {

/*
 * don't use xioctl for control query when using V4L2_CTRL_FLAG_NEXT_CTRL
 * returns: error code
 */
static int query_ioctl(V4L2Context *context, int current_ctrl, struct v4l2_queryctrl *ctrl) {
    int ret = 0;
    int tries = 4;
    do {
        if (ret != 0) {
            ctrl->id = current_ctrl | V4L2_CTRL_FLAG_NEXT_CTRL;
        }
        ret = v4l2_ioctl(context->fd, VIDIOC_QUERYCTRL, ctrl);
    } while (ret && tries-- && ((errno == EIO || errno == EPIPE || errno == ETIMEDOUT)));

    return ret;
};

/*
 * subscribe for v4l2 control events
 * args:
 *  context - pointer to V4L2Context
 *  control_id - id of control to subscribe events for
 *
 * return: none
 */
void v4l2_subscribe_control_events(V4L2Context *context, unsigned int control_id) {
    context->evsub.type = V4L2_EVENT_CTRL;
    context->evsub.id = control_id;

    int ret = xioctl(context->fd, VIDIOC_SUBSCRIBE_EVENT, &context->evsub);

    if (ret != 0) {
        base::LogError() << "V4L2_CORE: failed to subscribe events for control " << control_id
                         << " :" << strerror(errno);
    }
}

/*
 * add control to control list
 * args:
 *   context - pointer to V4L2Context
 *   queryctrl - pointer to v4l2_queryctrl data
 */
static bool add_control(V4L2Context *context, struct v4l2_queryctrl *queryctrl) {
    int menu_entries = 0;

    V4L2ControlData control_data;
    struct v4l2_querymenu *menu = NULL;      //menu list
    struct v4l2_querymenu *old_menu = menu;  //temp menu list pointer

    if (queryctrl->flags & V4L2_CTRL_FLAG_DISABLED) {
        base::LogWarn() << "Control " << queryctrl->id
                        << " is disabled: remove it from control list";
        return false;
    }

    //check menu items if needed
    if (queryctrl->type == V4L2_CTRL_TYPE_MENU || queryctrl->type == V4L2_CTRL_TYPE_INTEGER_MENU) {
        int i = 0;
        struct v4l2_querymenu querymenu = {0};

        for (querymenu.index = queryctrl->minimum; querymenu.index <= queryctrl->maximum;
             querymenu.index++) {
            querymenu.id = queryctrl->id;
            if (xioctl(context->fd, VIDIOC_QUERYMENU, &querymenu) < 0) {
                continue;
            }

            old_menu = menu;

            if (menu == nullptr) {
                menu = (struct v4l2_querymenu *)calloc(i + 1, sizeof(struct v4l2_querymenu));
            } else {
                menu =
                    (struct v4l2_querymenu *)realloc(menu, (i + 1) * sizeof(struct v4l2_querymenu));
            }

            if (menu == NULL) {
                /*since we exit on failure there was no need to free any previous */
                /* menu allocation (realloc), but silence cppcheck anyway */
                if (old_menu != NULL) {
                    free(old_menu);
                }

                base::LogError() << "V4L2_CORE: FATAL memory allocation failure (add_control): "
                                 << strerror(errno);
                exit(-1);
            }

            memcpy(&(menu[i]), &querymenu, sizeof(struct v4l2_querymenu));
            i++;
        }

        old_menu = menu;

        /*last entry (NULL name)*/
        if (menu != NULL) {
            menu = (struct v4l2_querymenu *)calloc(i + 1, sizeof(struct v4l2_querymenu));
        } else {
            menu = (struct v4l2_querymenu *)realloc(menu, (i + 1) * sizeof(struct v4l2_querymenu));
        }

        if (menu == NULL) {
            /*since we exit on failure there was no need to free any previous */
            /* menu allocation (realloc), but silence cppcheck anyway */
            if (old_menu != NULL) {
                free(old_menu);
            }

            base::LogError() << "V4L2_CORE: FATAL memory allocation failure (add_control): "
                             << strerror(errno);
            exit(-1);
        }

        menu[i].id = querymenu.id;
        menu[i].index = queryctrl->maximum + 1;
        if (queryctrl->type == V4L2_CTRL_TYPE_MENU) {
            menu[i].name[0] = 0;
        }
        menu_entries = i;
    }

    /*check for focus control to enable software autofocus*/
    if (queryctrl->id == V4L2_CID_FOCUS_LOGITECH || queryctrl->id == V4L2_CID_FOCUS_ABSOLUTE) {
        context->has_focus_control_id = queryctrl->id;
    }
    /*check for pan/tilt control*/
    else if (queryctrl->id == V4L2_CID_TILT_RELATIVE || queryctrl->id == V4L2_CID_PAN_RELATIVE) {
        /*get unit id for logitech pan_tilt V3 if any*/
        context->has_pantilt_control_id = 1;
        // context->pantilt_unit_id = get_logitech_peripheral_unit_id(context);
    }
    // Add the control to the linked list
    V4L2ControlData *control = (V4L2ControlData *)calloc(1, sizeof(V4L2ControlData));
    if (control == NULL) {
        base::LogError() << "FATAL memory allocation failure (add_control): " << strerror(errno);
        exit(-1);
    }
    memcpy(&(control->control), queryctrl, sizeof(struct v4l2_queryctrl));
    control->cclass = V4L2_CTRL_ID2CLASS(control->control.id);
    control->name = strdup(dgettext(GETTEXT_PACKAGE_V4L2CORE, (char *)control->control.name));
    //add the menu adress (NULL if not a menu)
    control->menu = menu;
    if (control->menu != NULL && control->control.type == V4L2_CTRL_TYPE_MENU) {
        control->menu_entry = (char **)calloc(menu_entries, sizeof(char *));
        if (control->menu_entry == NULL) {
            base::LogError() << "FATAL memory allocation failure (add_control): "
                             << strerror(errno);
            exit(-1);
        }
        for (int i = 0; i < menu_entries; i++) {
            control->menu_entry[i] =
                strdup(dgettext(GETTEXT_PACKAGE_V4L2CORE, (char *)control->menu[i].name));
        }
        control->menu_entries = menu_entries;
    } else {
        control->menu_entries = 0;
        control->menu_entry = NULL;
    }
    //allocate a string with max size if needed
    if (control->control.type == V4L2_CTRL_TYPE_STRING) {
        control->string = (char *)calloc(control->control.maximum + 1, sizeof(char));
        if (control->string == NULL) {
            base::LogError() << "FATAL memory allocation failure (add_control): "
                             << strerror(errno);
            exit(-1);
        }
    } else {
        control->string = NULL;
    }

    context->list_device_controls.push_back(control);

    //subscribe control events
    v4l2_subscribe_control_events(context, queryctrl->id);
    return true;
}

int V4L2Control::enumerate_control(V4L2Context *context) {
    assert(context != NULL);
    assert(context->fd > 0);
    assert(context->list_device_controls.size() == 0);

    int ret = 0;
    int control_count = 0;
    int currentctrl = 0;
    struct v4l2_queryctrl queryctrl = {0};
    queryctrl.id = V4L2_CTRL_FLAG_NEXT_CTRL;

    /*try the next_flag method first*/
    while ((ret = query_ioctl(context, currentctrl, &queryctrl)) == 0) {
        if (add_control(context, &queryctrl)) {
            control_count++;
        }

        currentctrl = queryctrl.id;
        queryctrl.id |= V4L2_CTRL_FLAG_NEXT_CTRL;
    }

    return 0;
}

}  // namespace uvc