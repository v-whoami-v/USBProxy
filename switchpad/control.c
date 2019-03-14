#include <stdio.h>
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

void stick(state_t *state, int x, int y) {
    state->buf[3] = (unsigned char)x;
    state->buf[4] = (unsigned char)y;
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

int main(int argc, char **argv) {
    int i;
    state_t state;

    init(&state);

    push(&state, BUTTON_A); output(&state);
    usleep(atoi(argv[1]) * 1000);
/*
    push(&state, BUTTON_A); 
*/
    push(&state, BUTTON_R); 
/*    stick(&state, 170, 128); output(&state); */
    stick(&state, atoi(argv[2]), atoi(argv[3])); output(&state);
    usleep(1000 * 500);
/*
    release(&state, BUTTON_A); release(&state, BUTTON_R); output(&state);
*/
/*
    release(&state, BUTTON_A); release(&state, BUTTON_R); output(&state);
*/
/*    usleep(1000 * 2000); */
/*    init(&state); output(&state); */
    

/*
    sprintf(buf, "00 00 0f ff 80 80 80 00", i, 0);
    sprintf(buf, "00 04 0f 80 80 80 80 00", i, 0);
    sprintf(buf, "80 00 0f 80 80 80 80 00", i, 0);
    output(buf);
    usleep(1000 * 200);
    sprintf(buf, "00 00 0f 80 80 80 80 00", i, 0);
    output(buf);
*/
}

