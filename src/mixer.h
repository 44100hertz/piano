/* #include <SDL2/SDL.h> */
/* #include "music.h" */

typedef struct {
    long srate, scount;
    int phase;
    int note_rate;
    int point;
    int next_tick;
    int bpm;
    int tickrate;
    Note note;
    Note (*note_callback)();
} Mixer;

void mixer_init(Mixer* m, int srate, Note (*callback)());
void mixer_callback(void* userdata, Uint8* stream, int len);
