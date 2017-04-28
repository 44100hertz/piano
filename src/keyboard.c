#include <SDL2/SDL.h>
#include "music.h"
#include "keyboard.h"

static void destroy();

static Note current = {0};

static int note_scancode[] = {
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

void keyboard_init()
{
    /* Generate a reverse table */
    const int max = 284; /* highest possible scancode */
    scancode_note = malloc(max * sizeof(int));
    memset(scancode_note, -1, max * sizeof(int));
    const int num_notes = sizeof(note_scancode) / sizeof(int);
    for(int i=0; i<num_notes; ++i) {
        scancode_note[note_scancode[i]] = i;
    }

    atexit(destroy);
}

static void destroy()
{
    free(scancode_note);
}

void keyboard_keydown(SDL_Scancode scancode)
{
    int note = scancode_note[scancode];
    if(note != -1) {
        current.pitch = note;
        current.on = 1;
    }
}

Note keyboard_callback()
{
    Note copy = current;
    current.on = 0;
    return copy;
}

