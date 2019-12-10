#!/bin/bash
echo "[d]efault, or [m]odified? : "
read camera_type

if [[ "$camera_type" == "d" ]]; then
    for camera in "$@"
    do
        v4l2-ctl -d $camera -c brightness=0                     # (int)    : min=-64 max=64 step=1 default=0 value=0
        v4l2-ctl -d $camera -c contrast=32                      # (int)    : min=0 max=95 step=1 default=32 value=32
        v4l2-ctl -d $camera -c saturation=55                    # (int)    : min=0 max=100 step=1 default=55 value=55
        v4l2-ctl -d $camera -c hue=0                            # (int)    : min=-2000 max=2000 step=1 default=0 value=0
        v4l2-ctl -d $camera -c white_balance_temperature_auto=1 # (bool)   : default=1 value=1
        v4l2-ctl -d $camera -c gamma=165                        # (int)    : min=100 max=300 step=1 default=165 value=165
        v4l2-ctl -d $camera -c power_line_frequency=1           # (menu)   : min=0 max=2 default=1 value=1
        #v4l2-ctl -d $camera -c white_balance_temperature=4600   # (int)    : min=2800 max=6500 step=10 default=4600 value=4600 flags=inactive
        v4l2-ctl -d $camera -c sharpness=2                      # (int)    : min=1 max=7 step=1 default=2 value=2
        v4l2-ctl -d $camera -c backlight_compensation=0         # (int)    : min=0 max=1 step=1 default=0 value=0
        v4l2-ctl -d $camera -c exposure_auto=3                  # (menu)   : min=0 max=3 default=3 value=3
        #v4l2-ctl -d $camera -c exposure_absolute=166            # (int)    : min=50 max=10000 step=1 default=166 value=166 flags=inactive
    done
fi

if [[ "$camera_type" == "m" ]]; then
    for camera in "$@"
    do
        v4l2-ctl -d $camera -c brightness=-64                   # (int)    : min=-64 max=64 step=1 default=0 value=0
        v4l2-ctl -d $camera -c contrast=95                      # (int)    : min=0 max=95 step=1 default=32 value=32
        v4l2-ctl -d $camera -c saturation=55                    # (int)    : min=0 max=100 step=1 default=55 value=55
        v4l2-ctl -d $camera -c hue=0                            # (int)    : min=-2000 max=2000 step=1 default=0 value=0
        v4l2-ctl -d $camera -c white_balance_temperature_auto=1 # (bool)   : default=1 value=1
        v4l2-ctl -d $camera -c gamma=100                        # (int)    : min=100 max=300 step=1 default=165 value=165
        v4l2-ctl -d $camera -c power_line_frequency=1           # (menu)   : min=0 max=2 default=1 value=1
        #v4l2-ctl -d $camera -c white_balance_temperature=4600   # (int)    : min=2800 max=6500 step=10 default=4600 value=4600 flags=inactive
        v4l2-ctl -d $camera -c sharpness=0                      # (int)    : min=1 max=7 step=1 default=2 value=2
        v4l2-ctl -d $camera -c backlight_compensation=0         # (int)    : min=0 max=1 step=1 default=0 value=0
        v4l2-ctl -d $camera -c exposure_auto=3                  # (menu)   : min=0 max=3 default=3 value=3
        #v4l2-ctl -d $camera -c exposure_absolute=166            # (int)    : min=50 max=10000 step=1 default=166 value=166 flags=inactive
    done
fi
