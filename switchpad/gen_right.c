#include <stdio.h>
#include <unistd.h>

void output(const char *str) {
    FILE *fp = fopen("/run/input.txt", "w");
    if (fp == NULL) {
        fprintf(stderr, "cannot open: %s\n", "/run/input.txt");
        return;
    }

    fprintf(fp, "%s", str);
    fclose(fp);
}

int upper[] = {4, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39};
int lower[] = {252, 251, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217};

int main() {
    int i;
    char buf[30];

    for (;;) {
        for (i=36; 0<i; i--) {
            sprintf(buf, "00 00 0f %02x %02x 80 80 00", 255 - i, upper[i]); output(buf); usleep(1000 * 5);
        }
        for (i=upper[0]; i<lower[0]; i++) {
            sprintf(buf, "00 00 0f %02x %02x 80 80 00", 255, i);
            output(buf);
            usleep(1000 * 5);
        }
        for (i=0; i<36; i++) {
            sprintf(buf, "00 00 0f %02x %02x 80 80 00", 255 - i, lower[i]);
            output(buf);
            usleep(1000 * 5);
        }
        for (i=lower[36]; upper[36]<i; i--) {
            sprintf(buf, "00 00 0f %02x %02x 80 80 00", 219, i);
            output(buf);
            usleep(1000 * 5);
        }
    }
}

