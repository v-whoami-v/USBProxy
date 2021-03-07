#ifndef PACKETFILTER_SWITCHPAD_H_
#define PACKETFILTER_SWITCHPAD_H_

#include "PacketFilter.h"

class PacketFilter_SwitchPad : public PacketFilter {
private:
	void convData(__u8 *data, __u16 len);
public:
	PacketFilter_SwitchPad();
	void filter_packet(Packet* packet);
	virtual char* toString() {return (char*)"Switch GamePad Filter";}
};
#endif /* PACKETFILTER_SWITCHPAD_H_ */
