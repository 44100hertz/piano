/* #include <SDL2/SDL.h> */

typedef struct {
    int on;
    int pitch;
} Note;

typedef struct {
    long srate, scount;
    int phase;
    int note_rate;
    int point;
    int next_tick;
    int bpm;
    int tickrate;
    Note (*note_callback)();
} Mixer;

void mixer_init(Mixer* m, int srate);
void mixer_callback(void* userdata, Uint8* stream, int len);
