#include <math.h>
#include <stdio.h>
#include "global.h"
#include "instr.h"

double instr_env_get(Note* note)
{
    switch(note->key_state) {
    case KEY_OFF: return 0;
    case KEY_HELD: return fmin(note->age / 20.0, 1.0) * 20.0;
    case KEY_RELEASE: return fmax(1 - (note->age / 80.0), 0) * 20.0;
    default: fprintf(stderr, "invalid note state: %d\n", note->key_state); return 0;
    }
}

double instr_get(Note* note, long srate)
{
    double car = fabs(sin(note->phase * (2 * M_PI / PP / srate / 4.0)) * note->vol);
    return sin(car);
}
