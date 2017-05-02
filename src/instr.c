#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "global.h"
#include "instr.h"
#include "wave.h"

static double get_rate(int note)
{
    return 440.0 * powf(2, (note-69)/12.0);
}
void instr_tick(Note* note, long srate)
{
    note->note_rate = get_rate(note->note) * 0xfffff / srate;

    switch(note->key_state) {
    case KEY_OFF:
        note->vol = 0;
        break;
    case KEY_HELD:
        note->vol = fmaxf(1 - (note->age / 160.0f), 0);
        printf("%d\n", note->note_rate);
        break;
    case KEY_RELEASE:
        note->vol = fmaxf(1 - (note->age / 160.0f), 0) * (1/128.0f);
        break;
    default:
        fprintf(stderr, "invalid note state: %d\n", note->key_state);
        break;
    }
}
float instr_get(Note* note)
{
    float car = (note->rampvol * wave_camelsine(note->phase * (1.0f/0xf)));
    return wave_sine(UINT16_MAX * car);
}
