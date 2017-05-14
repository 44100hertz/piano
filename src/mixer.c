#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "global.h"
#include "instr.h"
#include "scope.h"
#include "mixer.h"

void mixer_init(Mixer *m, int srate, Note* (*callback)())
{
    memset(m, 0, sizeof(*m));
    m->srate = srate;
    m->tickrate = 24;
    m->bpm = 120;
    m->callback = callback;
    m->ramp_rate = 20.0f / srate;
    m->scope = scope_init(srate, 60);
}

static float softclip(float i)
{
    if(fabsf(i) > 1) return copysignf(1, i);
    return i * 1.5f - 0.5f * i * i * i;
}
void mixer_callback(void* userdata, Uint8* stream, int len)
{
    Sint16* stream16 = (Sint16*)stream;
    Mixer* m = userdata;
    for(int i=0; i<len/2; i++){
        if(m->scount == m->next_tick){
            /* Get note data from keyboard */
            m->tick = m->callback();
            /* Update instrument with note data */
            for(int i=0; i<NUMV; ++i) instr_tick(&m->tick[i], m->srate);

            /* Find the next tick */
            ++m->num_ticks;
            m->next_tick = m->num_ticks * m->srate * 60 / m->bpm / m->tickrate;
        }

        float total = 0;
        for(int i=0; i<NUMV; i++){
            Note* n = &m->tick[i];

            n->rampvol += fminf(m->ramp_rate, fmaxf(-m->ramp_rate,
                                                    n->vol - n->rampvol));
            n->phase += n->note_rate;
            total += instr_get(&m->tick[i]);
        }

        stream16[i] = INT16_MAX * softclip(total * 0.15);

        m->scount++;
    }
    scope_fill(&m->scope, stream16, len/2);
}
