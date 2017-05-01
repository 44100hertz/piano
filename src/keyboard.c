#include <SDL2/SDL.h>
#include "global.h"
#include "keyboard.h"

static void destroy();

static Note tick[NUMV];
static int instr;
static int octave;
static int slot; /* Rolling instrument index */
static const int NO_NOTE = -1024;

static int note_scancode[] = {
    SDL_SCANCODE_A, /* B -1 */
    SDL_SCANCODE_Z, /* C +0 */
    SDL_SCANCODE_S, /* C#+0 */
    SDL_SCANCODE_X, /* D +0 */
    SDL_SCANCODE_D, /* D#+0 */
    SDL_SCANCODE_C, /* E +0 */
    SDL_SCANCODE_V, /* F +0 */
    SDL_SCANCODE_G, /* F#+0 */
    SDL_SCANCODE_B, /* G +0 */
    SDL_SCANCODE_H, /* G#+0 */
    SDL_SCANCODE_N, /* A +0 */
    SDL_SCANCODE_J, /* A#+0 */
    SDL_SCANCODE_M, /* B +0 */
    SDL_SCANCODE_Q, /* C +1 */
    SDL_SCANCODE_2, /* C#+1 */
    SDL_SCANCODE_W, /* D +1 */
    SDL_SCANCODE_3, /* D#+1 */
    SDL_SCANCODE_E, /* E +1 */
    SDL_SCANCODE_R, /* F +1 */
    SDL_SCANCODE_5, /* F#+1 */
    SDL_SCANCODE_T, /* G +1 */
    SDL_SCANCODE_6, /* G#+1 */
    SDL_SCANCODE_Y, /* A +1 */
    SDL_SCANCODE_7, /* A#+1 */
    SDL_SCANCODE_U, /* B +1 */
    SDL_SCANCODE_I, /* C +2 */
    SDL_SCANCODE_9, /* C#+2 */
    SDL_SCANCODE_O, /* D +2 */
    SDL_SCANCODE_0, /* D#+2 */
    SDL_SCANCODE_P, /* E +2 */
};
static int* scancode_note;

static int to_a440(int note) {
    return note + octave * 12 - 3;
}

void keyboard_init()
{
    instr = 0;
    octave = 4;
    /* Generate a reverse table */
    const int max = SDL_NUM_SCANCODES;
    scancode_note = malloc(max * sizeof(int));
    memset(scancode_note, NO_NOTE, max * sizeof(int));
    const int num_notes = sizeof(note_scancode) / sizeof(int);
    for(int i=0; i<num_notes; ++i) {
        scancode_note[note_scancode[i]] = i;
    }
    atexit(destroy);

    memset(&tick, 0, sizeof(tick));
}

static void destroy()
{
    free(scancode_note);
}

static void clear_keys()
{
    for(int i=0; i<NUMV; ++i)
        if(tick[i].key_state==KEY_HELD)
            tick[i].key_state = KEY_RELEASE;
}

void keyboard_keydown(SDL_Scancode scancode)
{
    int note;
    switch(scancode) {
    case SDL_SCANCODE_PAGEUP:   ++instr; break;
    case SDL_SCANCODE_PAGEDOWN: --instr; break;
    case SDL_SCANCODE_HOME: ++octave; clear_keys(); break;
    case SDL_SCANCODE_END:  --octave; clear_keys(); break;
    default:
        note = scancode_note[scancode];
        if(note==NO_NOTE) return;

        slot = (slot + 1) % NUMV;
        tick[slot].note = to_a440(note);
        tick[slot].key_state = KEY_HELD;
        tick[slot].age = 0;
        /* tick.instr[i] = instr; */
    }
}

void keyboard_keyup(SDL_Scancode scancode)
{
    int note = to_a440(scancode_note[scancode]);
    for(int i=0; i<NUMV; ++i) {
        if(tick[i].note == note && tick[i].key_state == KEY_HELD) {
            tick[i].key_state = KEY_RELEASE;
            return;
        }
    }
}

Note* keyboard_callback()
{
    for(int i=0; i<NUMV; ++i) {
        if(tick[i].key_state > KEY_OFF) tick[i].age++;
    }
    return tick;
}
