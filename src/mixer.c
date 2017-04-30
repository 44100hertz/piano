#include <string.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "global.h"
#include "instr.h"
#include "mixer.h"

static inline double softclip(double i)
{
    if(fabs(i) > 1) return copysign(1, i);
    return i * 1.5 - 0.5 * i * i * i;
}

/* Given a midi note, find the period */
static inline long get_rate(int note)
{
    return (440*PP) * pow(2, (note-69)/12.0);
}

void mixer_init(Mixer *m, int srate, Beat (*callback)())
{
    memset(m, 0, sizeof(*m));
    m->srate = srate;
    m->tickrate = 24;
    m->bpm = 120;
    m->callback = callback;
}

void mixer_callback(void* userdata, Uint8* stream, int len)
{
    Mixer* m = userdata;
    for(int i=0; i<len; i+=2) {
        if(m->scount == m->next_tick) {
            Beat b = m->callback();
            for(int i=0; i<NUMV; i++)
                m->note_rate[i] = get_rate(b.note[i]);
            for(int i=0; i<NUMV; i++)
                m->note_on[i] = b.on[i];
            for(int i=0; i<NUMV; i++)
                m->instr[i] = b.instr[i];

            int song_rate = m->bpm * m->tickrate;
            m->next_tick = m->scount + (m->srate * 60) / song_rate;
        }

        /* Advance waves by one period */
        for(int i=0; i<NUMV; i++) m->phase[i] += m->note_rate[i];

        /* Find corresponding sine tones */
        double mix[NUMV] = {0};
        for(int i=0; i<NUMV; i++) {
            mix[i] = instr_get(&m->instr[i], m->note_on[i], m->phase[i], m->srate);
        }

        /* Sum the channel values */
        double total = 0;
        for(int i=0; i<NUMV; i++) total += mix[i];
        uint16_t t = softclip(total * 0.15) * 0x7fff;
        memcpy(&stream[i], &t, 2);
        m->scount++;
    }
}
