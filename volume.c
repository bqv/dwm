#include <alloca.h>
#include <alsa/asoundlib.h>

typedef union {
	int i;
	unsigned int ui;
	float f;
	const void *v;
} Arg;

#include "volume.h"

static const char alsachannel[] = "Master";
static const char alsacard[] = "default";

snd_mixer_t *handle;
snd_mixer_selem_id_t *sid;

snd_mixer_elem_t *getelem(const char *selem_name) {
	if (handle == NULL) init();

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, selem_name);

	return snd_mixer_find_selem(handle, sid);
}

long getvol(snd_mixer_elem_t *elem) {
	long vol;

	snd_mixer_selem_get_playback_volume(elem, 0, &vol);
	
	return vol;
}

void setvol(long vol, snd_mixer_elem_t *elem) {
	snd_mixer_selem_set_playback_volume_all(elem, vol);
}

void setmute(snd_mixer_elem_t *elem, int status) {
	snd_mixer_selem_set_playback_switch_all(elem, status);
}

void togglemute(snd_mixer_elem_t *elem) {
	int status;

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
	snd_mixer_open(&handle, 0);
	snd_mixer_attach(handle, alsacard);
	snd_mixer_selem_register(handle, NULL, NULL);
	snd_mixer_load(handle);
}

void deinit(void) {
	snd_mixer_close(handle);
}

void incvol(const Arg *arg) {
	long min, max;

	snd_mixer_elem_t* elem = getelem(alsachannel);

	snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    
	int delta = (arg->i*(max-min))/100;
	long vol = getvol(elem);
    int newvol = vol+delta;
    
	if (newvol < min) newvol = min;
	if (newvol > max) newvol = max;

	setvol(newvol, elem);
}

void mutevol(const Arg *arg) {
	snd_mixer_elem_t* elem = getelem(alsachannel);

	switch (arg->i) {
		case -1:
			togglemute(elem);
			return;
		case 0:
			setmute(elem, 0);
			return;
		case 1:
			setmute(elem, 1);
			return;
	}

	return;
}

