#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "wave.h"

#define LUTSIZE (INT16_MAX / 4)
#define M_PI 3.141592653589
static float lut[LUTSIZE];

void wave_init()
{
    for(int i=0; i<LUTSIZE; ++i) {
        lut[i] = sinf(i * M_PI / 2.0 / LUTSIZE);
    }
}

/*  .-.
 --/---\---/--
        '-'  */
float wave_sine(int off) {
    div_t d = div(off, LUTSIZE);
    int index = (d.quot % 2 == 0) ? d.rem : LUTSIZE - d.rem - 1;
    int negative = (d.quot % 4 < 2) ? 1 : -1;
    return negative * lut[index];
}

/*  .-.
 --/---\___/--
             */
float wave_halfsine(int off) {
    div_t d = div(off, LUTSIZE);
    if(d.quot % 4 > 2) return 0;
    int index = (d.quot % 2 == 0) ? d.rem : LUTSIZE - d.rem - 1;
    return lut[index];
}

/*  .-. .-.
 --/---V---\--
             */
float wave_camelsine(int off) {
    div_t d = div(off, LUTSIZE);
    int index = (d.quot % 2 == 0) ? d.rem : LUTSIZE - d.rem - 1;
    return lut[index];
}

/*  .-  .-
 --/-|_/-|_/--
             */
float wave_quartersine(int off) {
    div_t d = div(off, LUTSIZE);
    return (d.quot % 2 == 0) ? lut[d.rem] : 0;
}

/* .---.
 --|---|---|--
       '---' */
float wave_pulse(int off) {
    return off < INT16_MAX/4 ? 1 : -1;
}

/*  /|  /|
 --/-|-/-|-/--
     |/  |/ */
float wave_ramp(int off) {
    return fmodf(off / (float)INT16_MAX, 1.0);
}
