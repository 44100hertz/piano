#ifndef PP
#define PP 0xff /* Pitch Precision; when phase or pitch is used, I use
                 * integers, and this is the denominator. */
#endif

#ifndef NUMV
#define NUMV 32 /* Max number of voices for polyphony. */
#endif

typedef struct {
    int on[NUMV];
    int note[NUMV];
    /* other data here later */
} Beat;
