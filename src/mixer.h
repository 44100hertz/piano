/* #include <SDL2/SDL.h> */

typedef struct {
    int srate;
    int phase;
    int note_rate;
    int point;
    int sample_count;
    int byte_count;
} Mixer;

void mixer_init(Mixer* m, int srate);
void mixer_callback(void* userdata, Uint8* stream, int len);
