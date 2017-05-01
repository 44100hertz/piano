#include <string.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "global.h"
#include "instr.h"
#include "mixer.h"

void mixer_init(Mixer *m, int srate, Note* (*callback)())
{
    memset(m, 0, sizeof(*m));
    m->srate = srate;
    m->tickrate = 24;
    m->bpm = 120;
    m->callback = callback;
}

static long get_rate(int note)
{
    return (440*PP) * pow(2, (note-69)/12.0);
}
static double softclip(double i)
{
    if(fabs(i) > 1) return copysign(1, i);
    return i * 1.5 - 0.5 * i * i * i;
}
void mixer_callback(void* userdata, Uint8* stream, int len)
{
    Mixer* m = userdata;
    for(int i=0; i<len; i+=2) {
        if(m->scount == m->next_tick) {
            m->next_tick = m->scount + (m->srate * 60) / m->bpm / m->tickrate;
            m->tick = m->callback();
            /* note pitches */
            for(int i=0; i<NUMV; ++i) m->tick[i].note_rate = get_rate(m->tick[i].note);
        }

        for(int i=0; i<NUMV; i++) m->tick[i].phase += m->tick[i].note_rate;

        /* Sum the channel values */
        double total = 0;
        for(int i=0; i<NUMV; i++) total += instr_get(&m->tick[i], m->srate);

        /* Quantize and fill part of buffer */
        int16_t total16 = softclip(total * 0.15) * 0x7fff;
        memcpy(&stream[i], &total16, 2);

        m->scount++;
    }
}
