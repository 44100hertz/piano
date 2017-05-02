/* #include <stdint.h> */

#ifndef NUMV
#define NUMV 32 /* Max number of voices for polyphony. */
#endif

typedef struct {
} Instr;

enum {
    KEY_OFF,
    KEY_HELD,
    KEY_RELEASE,
};

typedef struct {
    Instr*   instr;
    uint32_t phase;
    uint32_t note_rate;
    float    vol;
    float    rampvol;
    int      key_state;
    int      age;
    int      note;
} Note;
