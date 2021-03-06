/*
 * Copyright 2013 Dominic Spill
 * Copyright 2013 Adam Stasiak
 *
 * This file is part of USBProxy.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 *
 * RelayReader.h
 *
 * Created on: Dec 8, 2013
 */
#ifndef RELAYREADER_H_
#define RELAYREADER_H_

#include <linux/types.h>
#include <mqueue.h>

class Proxy;
class HostProxy;
class Endpoint;

class RelayReader {
private:
	__u8 haltSignal;
	mqd_t sendQueue;
	mqd_t recvQueue;
	Proxy* proxy;
	HostProxy* hostProxy;
	__u8 endpoint;
	__u8 attributes;
	__u16 maxPacketSize;

public:
	RelayReader(Endpoint* _endpoint,Proxy* _proxy,mqd_t _sendQueue);
	RelayReader(Endpoint* _endpoint,HostProxy* _proxy,mqd_t _sendQueue,mqd_t _recvQueue);
	virtual ~RelayReader();

	void relay_read();
	void relay_read_setup();

	void set_haltsignal(__u8 _haltSignal);
	static void* relay_read_helper(void* context);
};

#endif /* RELAYREADER_H_ */
