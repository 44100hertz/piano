/* #include <SDL2/SDL.h> */

void mixer_init(int _srate);
void mixer_callback(void* userdata, Uint8* stream, int len);
