#include <alloca.h>
#include <alsa/asoundlib.h>

typedef union {
	int i;
	unsigned int ui;
	float f;
	const void *v;
} Arg;

#include "volume.h"

snd_mixer_t *handle;
snd_mixer_selem_id_t *sid;

snd_mixer_elem_t *getelem(const char *selem_name) {
	if (handle == NULL) init();

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, selem_name);

	return snd_mixer_find_selem(handle, sid);
}

long getvol(const char *selem_name) {
	long min, max, vol;

	snd_mixer_elem_t* elem = getelem(selem_name);

	snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
	snd_mixer_selem_get_playback_volume(elem, 0, &vol);
	
	return vol;
}

void setvol(long vol, const char *selem_name) {
	long min, max;

	snd_mixer_elem_t* elem = getelem(selem_name);

	snd_mixer_selem_get_playback_volume_range(elem, &min, &max);

	if (vol < min) vol = min;
	if (vol > max) vol = max;

	snd_mixer_selem_set_playback_volume_all(elem, vol);
}

int getmute(void) {
	const char channel[] = "Master";
	int status;

	snd_mixer_elem_t* elem = getelem(channel);

	snd_mixer_selem_get_playback_switch(elem, 0, &status);

	return status;
}

void setmute(int status) {
	const char channel[] = "Master";

	snd_mixer_elem_t* elem = getelem(channel);

	snd_mixer_selem_set_playback_switch_all(elem, status);
}

void togglemute(void) {
	const char channel[] = "Master";
	int status;

	snd_mixer_elem_t* elem = getelem(channel);

	snd_mixer_selem_get_playback_switch(elem, 0, &status);
	switch (status) {
		case 0:
			status = 1;
			break;
		case 1:
			status = 0;
			break;
		default:
			return;
	}
	snd_mixer_selem_set_playback_switch_all(elem, status);
}

void init(void) {
	const char card[] = "default";

	snd_mixer_open(&handle, 0);
	snd_mixer_attach(handle, card);
	snd_mixer_selem_register(handle, NULL, NULL);
	snd_mixer_load(handle);
}

void deinit(void) {
	snd_mixer_close(handle);
}

void incvolmaster(const Arg *arg) {
	const char channel[] = "Master";
	int delta = arg->i;
	long vol = getvol(channel);

	setvol(vol+delta, channel);
}

void incvolpcm(const Arg *arg) {
	const char channel[] = "PCM";
	int delta = arg->i;
	long vol = getvol(channel);

	setvol(vol+delta, channel);
}

void mutevol(const Arg *arg) {
	switch (arg->i) {
		case -1:
			togglemute();
			return;
		case 0:
			setmute(0);
			return;
		case 1:
			setmute(1);
			return;
	}

	return;
}

