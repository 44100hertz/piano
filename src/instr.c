#include <math.h>
#include "global.h"
#include "instr.h"

int instr_get(Instr* instr, long phase, long srate)
{
    return sin(fmod(phase * (2 * M_PI / PP / srate), 10)) * 0x1fff;
}
