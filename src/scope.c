#include <SDL2/SDL.h>
#include <stdlib.h>
#include "scope.h"

void scope_destroy(Scope* s)
{
    free(s->points);
}

Scope scope_init(int srate, int fps)
{
    int frame_size = srate/fps;
    return (Scope){ frame_size, 0, malloc(frame_size * sizeof(SDL_Point)) };
}

void scope_fill(Scope* s, Sint16* samples, int size)
{
    for(int i=0; i<size; ++i) {
        s->offset = (s->offset+1) % s->frame_size;
        s->points[s->offset] = (SDL_Point){
            .x = 800 * s->offset / s->frame_size,
            .y = 200 + 400 * samples[i] / INT16_MAX
        };
    }
}
