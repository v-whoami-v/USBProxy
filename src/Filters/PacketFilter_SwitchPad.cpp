#include <cmath>
#include <cstdio>
#include <cstring>
#include "HexString.h"
#include "PacketFilter_SwitchPad.h"

const char *RULEFILE = "/run/rules.txt";
const char *INPUTFILE = "/run/input.txt";


bool input_map[256][256];

struct point {
    __u8 x;
    __u8 y;
};

class ConvMap {
private:
    point points[256][256];

public:
    ConvMap() {
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
            fprintf(stderr, "no such file: %s\n", filename);
            exit(1);
        }

        int x, y, cx, cy;

        while (fscanf(fp, "%d,%d,%d,%d", &x, &y, &cx, &cy) == 4) {
            points[x][y].x = cx;
            points[x][y].y = cy;
        }

        fclose(fp);
    }

    void set(int x, int y, int cx, int cy) {
        if (x < 0 || 255 < x || y < 0 || 255 < y) {
            return;
        }

        points[x][y].x = cx;
        points[x][y].y = cy;
    }


    __u8 getX(int x, int y) {
        return points[x][y].x;
    }

    __u8 getY(int x, int y) {
        return points[x][y].y;
    }
};

template<typename T>
T min(T a, T b) { return (a<b)?a:b; }

ConvMap convMap;

PacketFilter_SwitchPad::PacketFilter_SwitchPad() {
	for (int i=0; i<0x80; i++) {
		angle[i] = acos((i - 0x80) / (double)0x80) / (M_PI * 2) * 360;
	}
	for (int i=0x80; i<=0xff; i++) {
		angle[i] = acos((i - 0x80) / (double)0x7f) / (M_PI * 2) * 360;
	}

	for (int i=0; i<=0xff; i++) {
		printf("%02x: %f\n", i, angle[i]);
	}

        convMap.read(RULEFILE);
}

void add_input_map(int x, int y) {
    if (x < 0 || y < 0 || 255 < x || 255 < y) {
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

void PacketFilter_SwitchPad::convData(__u8* data, __u16 len) {
	if (len == 8) {
		add_input_map(0x00ff & data[3], 0x00ff & data[4]);

		if (data[1] & 0x01) {
			print_input_map();
		}

		FILE *fp = fopen(INPUTFILE, "r");
		static bool isvalid = false;
		static __u8 inputData[8];

		if (fp != NULL) {
			static unsigned int d[8];

			int len = fscanf(fp, "%02x %02x %02x %02x %02x %02x %02x %02x", &d[0], &d[1], &d[2], &d[3], &d[4], &d[5], &d[6], &d[7]);
			if (len == 8) {
				for (int i=0; i<8; i++) {
					inputData[i] = __u8(d[i]);
				}
				isvalid = true;
			}

			fclose(fp);

			if (isvalid) {
				memcpy(data, inputData, 8);
			}
		} else {
			isvalid = false;
			int x = convMap.getX(data[3], data[4]);
			int y = convMap.getY(data[3], data[4]);
			data[3] = x;
			data[4] = y;
		}
	}
}

void PacketFilter_SwitchPad::filter_packet(Packet* packet) {
	if (packet->wLength == 8 && packet->transmit) {
		convData(packet->data, packet->wLength);
	}
}
