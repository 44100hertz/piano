#include <SDL2/SDL.h>
#include "keyboard.h"

static int get_note(int scancode);
static void build_reverse_table();
static void destroy();

static int note_scancode[] = {
    SDL_SCANCODE_Q,
    SDL_SCANCODE_2,
    SDL_SCANCODE_W,
    SDL_SCANCODE_3,
    SDL_SCANCODE_E,
    SDL_SCANCODE_R,
    SDL_SCANCODE_5,
    SDL_SCANCODE_T,
    SDL_SCANCODE_6,
    SDL_SCANCODE_Y,
    SDL_SCANCODE_7,
    SDL_SCANCODE_U,
    SDL_SCANCODE_I,
    SDL_SCANCODE_9,
    SDL_SCANCODE_O,
    SDL_SCANCODE_0,
    SDL_SCANCODE_P,
};
static int* scancode_note;

void keyboard_init()
{
    /* Generate a reverse table */
    const int max = 284; /* highest possible scancode */
    scancode_note = calloc(max, sizeof(int));
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
    printf("%d\n", note);
}

void keyboard_tick()
{
    
}

