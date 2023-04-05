#include <stdio.h>
#include <fcntl.h>
#include <libevdev/libevdev.h>
#include <alsa/asoundlib.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "setvolume.h"
#include <thread>

/* See https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h */
#define PTT_EV_KEY_CODE BTN_EXTRA

// variable to store if we are pressing the button
bool button_pressed = false;

// variable to control the mute delay length
int delay = 0.4 * 1000000; // 1 second

void delay_mute(int delay)
{
	usleep(delay);

	// if we are not pressing the button, mute
	if (!button_pressed)
	{
		fprintf(stderr, "Muting");
		setvol(0);
	}
}

int main(int argc, char **argv)
{
	struct libevdev *dev = NULL;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s /dev/input/by-id/<device-name>\n", argv[0]);
		exit(0);
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		perror("Failed to open device");
		if (getuid() != 0)
			fprintf(stderr, "Fix permissions to %s or run as root\n", argv[1]);
		exit(1);
	}
	int rc = libevdev_new_from_fd(fd, &dev);
	if (rc < 0)
	{
		fprintf(stderr, "Failed to init libevdev (%s)\n", strerror(-rc));
		exit(1);
	}
	fprintf(stderr, "Input device name: \"%s\"\n", libevdev_get_name(dev));
	fprintf(stderr, "Input device ID: bus %#x vendor %#x product %#x\n",
			libevdev_get_id_bustype(dev),
			libevdev_get_id_vendor(dev),
			libevdev_get_id_product(dev));

	if (!libevdev_has_event_code(dev, EV_KEY, PTT_EV_KEY_CODE))
	{
		fprintf(stderr, "This device is not capable of sending this key code\n");
		exit(1);
	}

	do
	{
		struct input_event ev;

		rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
		if (rc != LIBEVDEV_READ_STATUS_SUCCESS)
			continue;
		if (ev.code == PTT_EV_KEY_CODE && ev.value != 2)
		{
			if (ev.value == 1)
			{
				fprintf(stderr, "Pushed");
				button_pressed = true;
				setvol(100);
			}
			else
			{
				button_pressed = false;
				fprintf(stderr, "Released");
				std::thread thread(delay_mute, delay);
				thread.detach();
				fflush(stdout);
			}
		}
	} while (rc == LIBEVDEV_READ_STATUS_SYNC || rc == LIBEVDEV_READ_STATUS_SUCCESS || rc == -EAGAIN);

	libevdev_free(dev);
	close(fd);

	return 0;
}
