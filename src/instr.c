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
    note->note_rate = get_rate(note->note) * 0x100000 / srate;

    switch(note->key_state) {
    case KEY_OFF: case KEY_RELEASE:
        note->vol = 0;
        break;
    case KEY_HELD:
        note->vol = fmaxf(1 - (note->age / 160.0f), 0.5);
        break;
    default:
        fprintf(stderr, "invalid note state: %d\n", note->key_state);
        break;
    }
}
static float switch_wave(uint16_t phase, int kind)
{
    switch(kind) {
    case WAVE_SINE:  return wave_sine(phase);
    case WAVE_HSINE: return wave_halfsine(phase);
    case WAVE_CSINE: return wave_camelsine(phase);
    case WAVE_QSINE: return wave_quartersine(phase);
    case WAVE_PULSE: return wave_pulse(phase);
    case WAVE_RAMP:  return wave_ramp(phase);
    default:
        fprintf(stderr, "Invalid waveform\n");
        return 0;
    }
}
float instr_get(Note* note)
{
    float car = switch_wave(note->phase / 0xf, note->car) * note->rampvol;
    return switch_wave(UINT16_MAX * note->level * car, note->mod);
}
