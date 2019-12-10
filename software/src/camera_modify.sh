#!/bin/bash
camera_type=$1

if [[ "$camera_type" == "d" ]]; then
    for ((i = 2; i <= $#; i++ )); do
        if [[ -e "${!i}" ]]; then
            v4l2-ctl -d ${!i} -c brightness=0                     # (int)    : min=-64 max=64 step=1 default=0 value=0
            v4l2-ctl -d ${!i} -c contrast=32                      # (int)    : min=0 max=95 step=1 default=32 value=32
            v4l2-ctl -d ${!i} -c saturation=55                    # (int)    : min=0 max=100 step=1 default=55 value=55
            v4l2-ctl -d ${!i} -c hue=0                            # (int)    : min=-2000 max=2000 step=1 default=0 value=0
            v4l2-ctl -d ${!i} -c white_balance_temperature_auto=1 # (bool)   : default=1 value=1
            v4l2-ctl -d ${!i} -c gamma=165                        # (int)    : min=100 max=300 step=1 default=165 value=165
            v4l2-ctl -d ${!i} -c power_line_frequency=1           # (menu)   : min=0 max=2 default=1 value=1
            #v4l2-ctl -d ${!i} -c white_balance_temperature=4600   # (int)    : min=2800 max=6500 step=10 default=4600 value=4600 flags=inactive
            v4l2-ctl -d ${!i} -c sharpness=2                      # (int)    : min=1 max=7 step=1 default=2 value=2
            v4l2-ctl -d ${!i} -c backlight_compensation=0         # (int)    : min=0 max=1 step=1 default=0 value=0
            v4l2-ctl -d ${!i} -c exposure_auto=3                  # (menu)   : min=0 max=3 default=3 value=3
            #v4l2-ctl -d ${!i} -c exposure_absolute=166            # (int)    : min=50 max=10000 step=1 default=166 value=166 flags=inactive
            echo "Successfully set '${!i}' to 'default'"
        else
            echo "Could not find '${!i}'!"
        fi
    done
elif [[ "$camera_type" == "m" ]]; then
    for ((i = 2; i <= $#; i++ )); do
        if [[ -e "${!i}" ]]; then
            v4l2-ctl -d ${!i} -c brightness=-64                   # (int)    : min=-64 max=64 step=1 default=0 value=0
            v4l2-ctl -d ${!i} -c contrast=95                      # (int)    : min=0 max=95 step=1 default=32 value=32
            v4l2-ctl -d ${!i} -c saturation=55                    # (int)    : min=0 max=100 step=1 default=55 value=55
            v4l2-ctl -d ${!i} -c hue=0                            # (int)    : min=-2000 max=2000 step=1 default=0 value=0
            v4l2-ctl -d ${!i} -c white_balance_temperature_auto=1 # (bool)   : default=1 value=1
            v4l2-ctl -d ${!i} -c gamma=100                        # (int)    : min=100 max=300 step=1 default=165 value=165
            v4l2-ctl -d ${!i} -c power_line_frequency=1           # (menu)   : min=0 max=2 default=1 value=1
            #v4l2-ctl -d ${!i} -c white_balance_temperature=4600   # (int)    : min=2800 max=6500 step=10 default=4600 value=4600 flags=inactive
            v4l2-ctl -d ${!i} -c sharpness=0                      # (int)    : min=1 max=7 step=1 default=2 value=2
            v4l2-ctl -d ${!i} -c backlight_compensation=0         # (int)    : min=0 max=1 step=1 default=0 value=0
            v4l2-ctl -d ${!i} -c exposure_auto=3                  # (menu)   : min=0 max=3 default=3 value=3
            #v4l2-ctl -d ${!i} -c exposure_absolute=166            # (int)    : min=50 max=10000 step=1 default=166 value=166 flags=inactive
            echo "Successfully set '${!i}' to 'modified'"
        else
            echo "Could not find '${!i}'!"
        fi
    done
else
    echo "Invalid Syntax! : '$1' is not valid option type!"
    echo "m = [m]odified"
    echo "d = [d]efault"
fi
