#include <cmath>
#include <cstdio>
#include <cstring>
#include "PacketFilter_SwitchPad.h"

using namespace std;

const char * const RULEFILE = "/run/rules.txt";
const char * const INPUTFILE = "/run/input.txt";

struct point {
    __u8 x;
    __u8 y;
};

inline bool isvalid_xy(int x, int y) {
    return (0<=x) && (x<=255) && (0<=y) && (y<=255);
}

class ConvMap {
private:
    point points[256][256];

public:
    ConvMap() {
        init();
    }

    void init() {
        for (int i=0; i<256; i++) {
            for (int j=0; j<256; j++) {
                points[i][j].x = i;
                points[i][j].y = j;
            }
        }
    }

    void read(const char *filename) {
        FILE* fp = fopen(filename, "r");

        if (fp == NULL) {
            perror(filename);
            return;
        }

        int x, y, cx, cy;

        while (fscanf(fp, "%d,%d,%d,%d", &x, &y, &cx, &cy) == 4) {
            set(x, y, cx, cy);
        }

        fclose(fp);
    }

    void set(int x, int y, int cx, int cy) {
        if (!isvalid_xy(x,y) || !isvalid_xy(cx,cy)) {
            fprintf(stderr, "Invalid value(s): (%d,%d) -> (%d,%d)\n", x, y, cx, cy);
            return;
        }

        points[x][y].x = cx;
        points[x][y].y = cy;
    }

    point get(int x, int y) {
        return points[x][y];
    }
};

ConvMap convMap;

PacketFilter_SwitchPad::PacketFilter_SwitchPad() {
    convMap.read(RULEFILE);
}

bool input_map[256][256];

void init_input_map() {
    for (int i=0; i<256; i++) {
        for (int j=0; j<256; j++) {
            input_map[i][j] = false;
        }
    }
}

void add_input_map(int x, int y) {
    if (!isvalid_xy(x, y)) {
        fprintf(stderr, "add_input_map: invalid value(s) (%d,%d)\n", x, y);
        return;
    }

    input_map[x][y] = true;
}

void print_input_map() {
    for (int i=0; i<=255; i++) {
        for (int j=0; j<=255; j++) {
            if (input_map[i][j]) {
                printf("%d %d\n", i, j);
            }
        }
    }
}

void dump(Packet* packet) {
    if (packet->wLength == 8) {
        for (int i=0; i<8; i++) {
            if (i == 7) {
                printf("%02x: ", packet->data[i]);
            } else {
                printf("%02x ", packet->data[i]);
            }
        }
    }
}

void PacketFilter_SwitchPad::convData(__u8* data, __u16 len) {
    if (len != 8) {
        return;
    }

    int rx = 0x00ff & data[3];
    int ry = 0x00ff & data[4];

    add_input_map(rx, ry);

    if (data[1] & 0x01) {
        print_input_map();
    }

    if (data[1] & 0x02) {
        printf("reload rules: %s\n", RULEFILE);
        convMap.init();
        convMap.read(RULEFILE);
        init_input_map();
    }

    FILE *fp = fopen(INPUTFILE, "r");
    static bool isvalid = false;
    static __u8 inputData[8];

    if (fp != NULL) {
        static unsigned int d[8];

        int len = fscanf(fp, "%02x %02x %02x %02x %02x %02x %02x %02x", &d[0], &d[1], &d[2], &d[3], &d[4], &d[5], &d[6], &d[7]);
        fclose(fp);

        if (len == 8) {
            for (int i=0; i<8; i++) {
                inputData[i] = __u8(d[i]);
            }
            isvalid = true;
        }

        if (isvalid) {
            memcpy(data, inputData, 8);
        }

        int x = 0x00ff & data[3];
        int y = 0x00ff & data[4];
        double angle = 0;

        if (y != 0x0080) {
            double xx = abs(x - 0x0080);
            double yy = abs(y - 0x0080);
            double l = sqrt(pow(xx, 2) + pow(yy, 2));
            angle = acos(xx / l) / (M_PI * 2) * 360;
        }

        printf("(X: %d Y: %d Angle: %0.4f)\n", data[3], data[4], angle);
    } else {
        isvalid = false;
        point p = convMap.get(data[3], data[4]);
        data[3] = p.x;
        data[4] = p.y;
        double angle = 0;
        if (p.y != 0x80) {
            double xx = abs(p.x - 0x0080);
            double yy = abs(p.y - 0x0080);
            double l = sqrt(pow(xx, 2) + pow(yy, 2));
            angle = acos(xx / l) / (M_PI * 2) * 360;
        }

        printf("(X:%3d Y: %3d) -> (X: %3d Y: %3d Angle: %0.4f)\n", rx, ry, data[3], data[4], angle);
    }
}

void PacketFilter_SwitchPad::filter_packet(Packet* packet) {
    if (packet->wLength == 8 && packet->transmit) {
        dump(packet);
        convData(packet->data, packet->wLength);
    }
}
