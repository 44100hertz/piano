#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "wave.h"
#include "global.h"
#include "mixer.h"
#include "keyboard.h"
#include "piano.h"

/* Print the printable part of SDL audio device info */
static void print_deviceinfo(SDL_AudioSpec a)
{
    printf(
        "freq:\t%d\n"
        "chans:\t%d\n"
        "buffer:\t%d\n",
        a.freq, a.channels, a.samples);
}

int piano_init()
{
    Mixer m;

    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) {
        SDL_Log("%s", SDL_GetError());
        return -1;
    }
    SDL_AudioSpec want = {
        .freq = 48000,
        .format = AUDIO_S16SYS,
        .channels = 1,
        .samples = 1024,
        .callback = mixer_callback,
        .userdata = &m,
    };
    SDL_AudioSpec have;

    SDL_AudioDeviceID dev;
    if(!(dev = SDL_OpenAudioDevice(
             NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE))) {
        SDL_Log("%s", SDL_GetError());
        return -1;
    }

    print_deviceinfo(have);
    SDL_GL_SetSwapInterval(1);

    SDL_Window* window = SDL_CreateWindow(
        "Getting SDL to work",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );
    SDL_Renderer* rdr = SDL_CreateRenderer(window, -1,
                                           SDL_RENDERER_PRESENTVSYNC |
                                           SDL_RENDERER_ACCELERATED |
                                           SDL_RENDERER_TARGETTEXTURE
        );
    mixer_init(&m, have.freq, keyboard_callback);
    wave_init();
    keyboard_init();
    SDL_PauseAudioDevice(dev, 0);

    SDL_Event e;
    int quit = 0;
    while(!quit) {
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
            case SDL_QUIT: quit=1; break;
            case SDL_KEYDOWN:
                if(!e.key.repeat) keyboard_keydown(e.key.keysym.scancode);
                break;
            case SDL_KEYUP:
                if(!e.key.repeat) keyboard_keyup(e.key.keysym.scancode);
                break;
            }
        }
        SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
        SDL_RenderClear(rdr);
        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyRenderer(rdr);
    SDL_CloseAudioDevice(dev);
    return 0;
}
