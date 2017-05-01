/* #include <SDL2/SDL.h> */
/* #include "global.h" */

typedef struct {
    long  srate, scount;
    long  next_tick;
    int   bpm, tickrate;
    Note* tick;
    Note* (*callback)();
} Mixer;

void mixer_init(Mixer* m, int srate, Note* (*callback)());
void mixer_callback(void* userdata, Uint8* stream, int len);
