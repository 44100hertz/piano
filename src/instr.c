#include <math.h>
#include <stdio.h>
#include "global.h"
#include "instr.h"

double env_get(Instr* instr, int key_state, int age)
{
    switch(key_state) {
    case KEY_OFF: return 0;
    case KEY_HELD: return fmin(age / 20.0, 1.0) * 20.0;
    case KEY_RELEASE: return fmax(1 - (age / 80.0), 0) * 20.0;
    default: fprintf(stderr, "invalid note state: %d\n", key_state); return 0;
    }
}

double instr_get(Note* note, long srate)
{
    double vol = env_get(note->instr, note->key_state, note->age);
    double car = fabs(sin(note->phase * (2 * M_PI / PP / srate / 4.0)) * vol);
    return sin(car);
}
