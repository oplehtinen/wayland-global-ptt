#include <alsa/asoundlib.h>
#include <iostream>

void  setvol(int vol) {
	long min, max;
	snd_mixer_t *handle;
	snd_mixer_selem_id_t *sid;
	const char *card = "default";
	const char *selem_name = "Capture";

	snd_mixer_open(&handle, 0);
	snd_mixer_attach(handle, card);
	snd_mixer_selem_register(handle, NULL, NULL);
	snd_mixer_load(handle);

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, selem_name);
	snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

	if (snd_mixer_selem_has_capture_switch(elem)) {
		snd_mixer_selem_set_capture_switch_all(elem, vol);
	}

	snd_mixer_close(handle);
}
