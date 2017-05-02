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
    m->ramp_rate = 20.0 / srate;
}

static long get_rate(int note)
{
    return (440*PP) * pow(2, (note-69)/12.0);
}
static float softclip(float i)
{
    if(fabs(i) > 1) return copysign(1, i);
    return i * 1.5 - 0.5 * i * i * i;
}
void mixer_callback(void* userdata, Uint8* stream, int len)
{
    Uint16* stream16 = (Uint16*)stream;
    Mixer* m = userdata;
    for(int i=0; i<len/2; i++) {
        if(m->scount == m->next_tick) {
            m->tick = m->callback();

            /* Set pitch and volume (once per tick) */
            for(int i=0; i<NUMV; ++i) {
                m->tick[i].vol = instr_env_get(&m->tick[i]);
                m->tick[i].note_rate = get_rate(m->tick[i].note);
            }

            ++m->num_ticks;
            m->next_tick = m->num_ticks * m->srate * 60 / m->bpm / m->tickrate;
        }

        float total = 0;
        for(int i=0; i<NUMV; i++) {
            Note* n = &m->tick[i];

            n->rampvol += fminf(m->ramp_rate,
                                fmaxf(-m->ramp_rate, n->vol - n->rampvol));
            n->phase += n->note_rate;
            total += instr_get(&m->tick[i], m->srate);
        }

        /* Quantize, fill buffer */
        stream16[i] = softclip(total * 0.15) * INT16_MAX;

        m->scount++;
    }
}
