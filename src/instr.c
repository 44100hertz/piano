#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "global.h"
#include "instr.h"
#include "wave.h"

static long get_rate(int note)
{
    return (440*PP) * powf(2, (note-69)/12.0f);
}
void instr_tick(Note* note)
{
    note->note_rate = get_rate(note->note);

    switch(note->key_state) {
    case KEY_OFF: note->vol = 0; break;
    case KEY_HELD: note->vol = fmaxf(1 - (note->age / 80.0f), 0); break;
    case KEY_RELEASE: note->vol = fmaxf(1 - (note->age / 80.0f), 0) * 0.5f; break;
    default: fprintf(stderr, "invalid note state: %d\n", note->key_state);
    }
}
float instr_get(Note* note, long srate)
{
    float car = (note->rampvol * wave_camelsine(note->phase * INT16_MAX / srate / PP));
    return wave_sine(UINT16_MAX * 2 * car);
}
