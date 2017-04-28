#include <string.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "global.h"
#include "mixer.h"

/* Apply hard clipping to an out-of-bounds signal */
static int16_t clamp16(int i)
{
    i = i < -0x7fff ? -0x7fff : i;
    i = i >  0x7fff ?  0x7fff : i;
    return i;
}

/* Make a usable sine wave */
static int16_t sin16(int phase, int srate)
{
    return sin(phase * (M_2_PI / PP / srate)) * 0x7fff;
}

/* Given a midi note, find the period */
static int get_rate(int note)
{
    return (440.0*PP) * pow(2, (note-69)/12.0);
}

void mixer_init(Mixer *m, int srate, Beat (*callback)())
{
    memset(m, 0, sizeof(Mixer));
    m->srate = srate;
    m->tickrate = 6;
    m->bpm = 120;
    m->callback = callback;
}

void tick(Mixer* m)
{
    /* Beat b = m->callback(); */
    /* TODO: load beat data into current state */
    /* m->note_rate = get_rate(m->note.pitch + 69); */
    m->next_tick = m->scount +
        (m->srate * 60) /
        (m->bpm * m->tickrate);
}

void mixer_callback(void* userdata, Uint8* stream, int len)
{
    Mixer* m = userdata;
    for(int i=0; i<len; i+=2) {
        /* Tick sometimes */
        if(m->scount == m->next_tick) {
            Beat b = m->callback();
            for(int i=0; i<NUMV; i++)
                m->note_rate[i] = get_rate(b.note[i]+69);
            for(int i=0; i<NUMV; i++)
                m->note_on[i] = b.on[i];

            int song_rate = m->bpm * m->tickrate;
            m->next_tick = m->scount + (m->srate * 60) / song_rate;
        }

        /* Advance waves by one period */
        for(int i=0; i<NUMV; i++) m->phase[i] += m->note_rate[i];

        /* Find corresponding sine tones */
        int mix[NUMV] = {0};
        for(int i=0; i<NUMV; i++) {
            if(m->note_on[i]) mix[i] = sin16(m->phase[i], m->srate);
        }

        /* Sum the channel values */
        int total = 0;
        for(int i=0; i<NUMV; i++) total += mix[i];
        uint16_t t = clamp16(total);
        memcpy(&stream[i], &t, 2);
        m->scount++;
    }
}
