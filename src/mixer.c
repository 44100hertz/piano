#include <string.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "global.h"
#include "instr.h"
#include "mixer.h"

/* Apply hard clipping to an out-of-bounds signal */
static inline int16_t clamp16(int i)
{
    i = i < -0x7fff ? -0x7fff : i;
    i = i >  0x7fff ?  0x7fff : i;
    return i;
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
    m->tickrate = 6;
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
                m->note_rate[i] = get_rate(b.note[i]+69);
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
        int mix[NUMV] = {0};
        for(int i=0; i<NUMV; i++) {
            if(m->note_on[i]) {
                mix[i] = instr_get(&m->instr[i], m->phase[i], m->srate);
            }
        }

        /* Sum the channel values */
        int total = 0;
        for(int i=0; i<NUMV; i++) total += mix[i];
        uint16_t t = clamp16(total);
        memcpy(&stream[i], &t, 2);
        m->scount++;
    }
}
