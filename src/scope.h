/* #include <SDL2/SDL.h> */

typedef struct {
    int frame_size;
    int offset;
    SDL_Point *points;
} Scope;

Scope scope_init(int srate, int fps);
void scope_destroy(Scope* s);
void scope_fill(Scope* s, Sint16* samples, int size);
