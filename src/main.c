#include <stdio.h>
#include <SDL2/SDL.h>

#include "mixer.h"

/* Print the printable part of SDL audio device info */
void print_deviceinfo(SDL_AudioSpec a)
{
    printf(
        "freq:\t%d\n"
        "chans:\t%d\n"
        "buffer:\t%d\n",
        a.freq, a.channels, a.samples);
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    SDL_AudioSpec want = {
        .freq = 48000,
        .format = AUDIO_S16,
        .channels = 1,
        .samples = 512,
        .callback = mixer_callback,
    };
    SDL_AudioSpec have;

    SDL_AudioDeviceID dev;
    if(!(dev = SDL_OpenAudioDevice(
             NULL, 0, &want, &have, SDL_AUDIO_ALLOW_ANY_CHANGE))) {
        SDL_Log("%s", SDL_GetError());
        return 3;
    }

    print_deviceinfo(have);
    mixer_init(have.freq);
    SDL_PauseAudioDevice(dev, 0);

    SDL_GL_SetSwapInterval(1);
    SDL_Window* window = SDL_CreateWindow(
        "Getting SDL to work",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );

    SDL_Event e;
    int quit = 0;
    while(!quit) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) quit=1;
        }
        SDL_GL_SwapWindow(window);
    }

    SDL_CloseAudioDevice(dev);
    return 0;
}
