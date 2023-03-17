# wayland-global-ptt
A temporary solution for Wayland push-to-talk globally. Listens for defined key event via `evdev` and then toggles the volume of the default capture device via `alsa-lib`.

> NOTE: by default the first mouse side button (Forward, Mouse 4) key is used for push to talk. In order to use a different key, change values for `PTT_EV_KEY_CODE` in file `push-to-talk.cpp`.

> NOTE: By default, we target the default ALSA input card and the  element called `Capture`. Modify `card` and `selem_name` to suit your needs.

## Note on permissions

Your `$USER` should at least have read access to the device. 
**This won't work with root privileges!** 
This is because the root user (typically) has no context for ALSA. Modify according to your needs.

## Security
Usually, you can give access to the device by adding the user to the group `input`. Do note that on most distributions, this not very secure, since now effectively any program can read all of your inputs (via libevdev or similar).

### Use a mouse!
If you are using a mouse for PTT, you should instead use udev rules to allow access to the specified mice. This way, no extra permissions are given to your keyboard. See [example here](https://wiki.archlinux.org/title/Udev#Allowing_regular_users_to_use_devices).

# Installation & Usage

## Requirements

- C++ compiler & Make
- libevdev
- alsa-lib

```
make
./push-to-talk /dev/input/by-id/<device-name> &
```

# Todo
- [ ] Include an utility to pick the correct input device
- [ ] Include an utility to pick the correct capture device & element
- [ ] Introduce a "releasy delay", eg. wait for x ms before muting microphone
