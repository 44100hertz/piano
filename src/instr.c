#include <math.h>
#include "global.h"
#include "instr.h"

int instr_get(Instr* instr, int note_on, long phase, long srate)
{
    if(!note_on) return 0;
    double vol = fmax(1 - (note_on / 80.0), 0) * 20.0;
    double car = fabs(sin(phase * (2 * M_PI / PP / srate / 4.0)) * vol);
    return sin(car) * 0x1fff;
}
