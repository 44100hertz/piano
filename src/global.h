#ifndef PP
#define PP 0xff /* Pitch Precision; when phase or pitch is used, I use
                 * integers, and this is the denominator. */
#endif

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
    Instr* instr;
    long   phase;
    double vol;
    double rampvol;
    long   note_rate;
    int    key_state;
    int    age;
    int    note;
} Note;
