/* #include <stdint.h> */

#ifndef NUMV
#define NUMV 32 /* Max number of voices for polyphony. */
#endif

enum {
    KEY_OFF,
    KEY_HELD,
    KEY_RELEASE,
};

enum {
    WAVE_SINE,
    WAVE_HSINE,
    WAVE_CSINE,
    WAVE_QSINE,
    WAVE_PULSE,
    WAVE_RAMP,
};
#define NUM_WAVES 6;

typedef struct {
    int      car;   /* Carrier wave type for PM synth */
    int      mod;   /* Modulo wave type for PM synth */
    float    level; /* Intensity of PM carrying */
    uint32_t phase;     /* Instant wave phase */
    uint32_t note_rate; /* Derivative of wave phase */
    float    vol;       /* Goal volume */
    float    rampvol;   /* Instant volume */
    int      key_state;
    int      age;
    int      note;
} Note;
