#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "global.h"
#include "instr.h"
#include "wave.h"

double instr_env_get(Note* note)
{
    switch(note->key_state) {
    case KEY_OFF: return 0;
    case KEY_HELD: return fmax(1 - (note->age / 80.0), 0);
    case KEY_RELEASE: return fmax(1 - (note->age / 80.0), 0) * 0.5;
    default: fprintf(stderr, "invalid note state: %d\n", note->key_state); return 0;
    }
}

float instr_get(Note* note, long srate)
{
    return wave_sine(note->phase * INT16_MAX / srate / PP) * note->rampvol;
}
