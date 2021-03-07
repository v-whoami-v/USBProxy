#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUTTON_L 0x1000
#define BUTTON_R 0x2000
#define BUTTON_ZL 0x4000
#define BUTTON_ZR 0x8000
#define BUTTON_X 0x0800
#define BUTTON_A 0x0400
#define BUTTON_B 0x0200
#define BUTTON_Y 0x0100
#define BUTTON_CAP 0x0020
#define BUTTON_HOME 0x0010

typedef struct { unsigned char buf[8]; } state_t;

void init(state_t *state) {
    state->buf[0] = (unsigned char)0x00;
    state->buf[1] = (unsigned char)0x00;
    state->buf[2] = (unsigned char)0x0f;
    state->buf[3] = (unsigned char)0x80;
    state->buf[4] = (unsigned char)0x80;
    state->buf[5] = (unsigned char)0x80;
    state->buf[6] = (unsigned char)0x80;
    state->buf[7] = (unsigned char)0x00;
}

void push(state_t *state, int button) {
    unsigned char b0 = (button & 0xff00) >> 8;
    unsigned char b1 = button & 0x00ff;
    state->buf[0] |= b0;
    state->buf[1] |= b1;
}

void release(state_t *state, int button) {
    unsigned char b0 = (button & 0xff00) >> 8;
    unsigned char b1 = button & 0x00ff;
    state->buf[0] &= ~b0;
    state->buf[1] &= ~b1;
}

void left(state_t *state, int x, int y) {
    state->buf[3] = (unsigned char)x;
    state->buf[4] = (unsigned char)y;
}

void right(state_t *state, int x, int y) {
    state->buf[5] = (unsigned char)x;
    state->buf[6] = (unsigned char)y;
}

void output(state_t *state) {
    FILE *fp = fopen("/run/input.txt", "w");
    if (fp == NULL) {
        fprintf(stderr, "cannot open: %s\n", "/run/input.txt"); 
        return;
    }

    fprintf(fp, "%02x %02x %02x %02x %02x %02x %02x %02x", state->buf[0], state->buf[1], state->buf[2], state->buf[3], state->buf[4], state->buf[5], state->buf[6], state->buf[7]);
    fclose(fp);
}

void usage(FILE *fp, const char *name) {
    fprintf(fp, "Usage: %s [-a|-A] [-b|-B] [-x|-X] [-y|-Y] [-l|-L] [-r|-R] [-zl|-ZL] [-zr|-ZR] [-cap|-CAP] [-home|-HOME] [-leftx NUM] [-lefty NUM] [-rightx NUM] [righty NUM]\n", name);

}

int main(int argc, char **argv) {
    int i;
    state_t state;

    int leftx = 128;
    int lefty = 128;
    int rightx = 128;
    int righty = 128;

    init(&state);

    for (i=1; i<argc; i++) {
        if (strcmp(argv[i], "-a") == 0)      release(&state, BUTTON_A);
        else if (strcmp(argv[i], "-A") == 0) push(&state, BUTTON_A);
        else if (strcmp(argv[i], "-b") == 0) release(&state, BUTTON_B);
        else if (strcmp(argv[i], "-B") == 0) push(&state, BUTTON_B);
        else if (strcmp(argv[i], "-x") == 0) release(&state, BUTTON_X);
        else if (strcmp(argv[i], "-X") == 0) push(&state, BUTTON_X);
        else if (strcmp(argv[i], "-y") == 0) release(&state, BUTTON_Y);
        else if (strcmp(argv[i], "-Y") == 0) push(&state, BUTTON_Y);
        else if (strcmp(argv[i], "-l") == 0) release(&state, BUTTON_L);
        else if (strcmp(argv[i], "-L") == 0) push(&state, BUTTON_L);
        else if (strcmp(argv[i], "-r") == 0) release(&state, BUTTON_R);
        else if (strcmp(argv[i], "-R") == 0) push(&state, BUTTON_R);
        else if (strcmp(argv[i], "-zl") == 0) release(&state, BUTTON_ZL);
        else if (strcmp(argv[i], "-ZL") == 0) push(&state, BUTTON_ZL);
        else if (strcmp(argv[i], "-zr") == 0) release(&state, BUTTON_ZR);
        else if (strcmp(argv[i], "-ZR") == 0) push(&state, BUTTON_ZR);
        else if (strcmp(argv[i], "-cap") == 0) release(&state, BUTTON_CAP);
        else if (strcmp(argv[i], "-CAP") == 0) push(&state, BUTTON_CAP);
        else if (strcmp(argv[i], "-home") == 0) release(&state, BUTTON_HOME);
        else if (strcmp(argv[i], "-HOME") == 0) push(&state, BUTTON_HOME);
        else if (strcmp(argv[i], "-lefty") == 0) lefty = atoi(argv[++i]);
        else if (strcmp(argv[i], "-leftx") == 0) leftx = atoi(argv[++i]);
        else if (strcmp(argv[i], "-righty") == 0) righty = atoi(argv[++i]);
        else if (strcmp(argv[i], "-rightx") == 0) rightx = atoi(argv[++i]);
        else if (strcmp(argv[i], "-h") == 0) {
            usage(stdout, argv[0]);
            return 0;
        } else {
            usage(stdout, argv[0]);
            return -1;
        }
    }

    left(&state, leftx, lefty);
    right(&state, rightx, righty);

    output(&state);

    return 0;
}

