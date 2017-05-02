/* #include <SDL2/SDL.h> */
/* #include "global.h" */

typedef struct {
    Note* tick;
    long  srate, scount;
    long  next_tick;
    int   num_ticks;
    int   bpm, tickrate;
    Note* (*callback)();
} Mixer;

void mixer_init(Mixer* m, int srate, Note* (*callback)());
void mixer_callback(void* userdata, Uint8* stream, int len);
