#include <string.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "mixer.h"

#define PP 0xffff /* Pitch Precision */
static const int voices = 32;

static int offset;
static int note_rate;
static int srate;

/* Apply hard clipping to an out-of-bounds signal */
static int16_t clamp16(int i)
{
    i = i < -0x7fff ? -0x7fff : i;
    i = i >  0x7fff ?  0x7fff : i;
    return i;
}

/* Make a usable sine wave */
static int16_t sin16(int phase)
{
    return sin(phase * (M_2_PI / PP / srate)) * 0x7fff;
}

/* Apply an operation to an array of ints */
static int* map(int* data, int(*fn)(int))
{
    int v[voices];
    for(int i=voices; i--;) v[i] = fn(data[i]);
}

/* Sum an array of ints */
static int mix(int* data)
{
    int total = 0;
    int i = voices;
    while(i--) total += data[i];
    return clamp16(total);
}

/* Given a midi note, find the period */
static int get_rate(int note)
{
    return (440.0*PP) * pow(2, (note-69)/12.0);
}

void mixer_init(int _srate)
{
    srate = _srate;
    note_rate = get_rate(78);
}

void mixer_callback(void* userdata, Uint8* stream, int len)
{
    for(int i=0; i<len; i+=2) {
        int16_t point = sin16(offset);
        memcpy(&stream[i], &point, 2);
        offset += note_rate;
    }
}
