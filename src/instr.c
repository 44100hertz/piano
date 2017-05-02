#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "global.h"
#include "instr.h"
#include "wave.h"

float instr_env_get(Note* note)
{
    switch(note->key_state) {
    case KEY_OFF: return 0;
    case KEY_HELD: return fmaxf(1 - (note->age / 80.0f), 0);
    case KEY_RELEASE: return fmaxf(1 - (note->age / 80.0f), 0) * 0.5f;
    default: fprintf(stderr, "invalid note state: %d\n", note->key_state); return 0;
    }
}

float instr_get(Note* note, long srate)
{
    uint16_t car = INT16_MAX * (note->rampvol *
                               wave_camelsine(note->phase * INT16_MAX / srate / PP));
    return wave_sine(car);
}
