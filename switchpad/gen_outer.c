#include <stdio.h>
#include <unistd.h>

void output(const char *str) {
/*
    printf("%s\n", str);
*/
    FILE *fp = fopen("/run/input.txt", "w");
    if (fp == NULL) {
        fprintf(stderr, "cannnot open: %s\n", "/run/input.txt");
        return;
    }

    fprintf(fp, "%s", str);
    fclose(fp);
}

int main() {
    int i;
    char buf[30];

    for (;;) {
        for (i=0; i<255; i++) {
            sprintf(buf, "00 00 0f %02x %02x 80 80 00", i, 0);
            output(buf);
            usleep(1000 * 10);
        }
        for (i=0; i<255; i++) {
            sprintf(buf, "00 00 0f %02x %02x 80 80 00", 255, i);
            output(buf);
            usleep(1000 * 10);
        }
        for (i=255; 0<i; i--) {
            sprintf(buf, "00 00 0f %02x %02x 80 80 00", i, 255);
            output(buf);
            usleep(1000 * 10);
        }
        for (i=255; 0<i; i--) {
            sprintf(buf, "00 00 0f %02x %02x 80 80 00", 0, i);
            output(buf);
            usleep(1000 * 10);
        }
    }
}

