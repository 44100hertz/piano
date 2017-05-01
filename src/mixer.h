/* #include <SDL2/SDL.h> */
/* #include "global.h" */

typedef struct {
    long  srate, scount;
    long  next_tick;
    int   bpm, tickrate;
    Instr instr[NUMV];
    long  phase[NUMV];
    long  note_rate[NUMV];
    int   key_state[NUMV];
    int   age[NUMV];
    Tick  (*callback)();
} Mixer;

void mixer_init(Mixer* m, int srate, Tick (*callback)());
void mixer_callback(void* userdata, Uint8* stream, int len);
