/*
**
** Copyright (C) 2020 Ashcon Mohseninia
** Author: Ashcon Mohseninia <ashcon50@gmail.com>
**
** This library is free software; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation, either version 3 of the License, or (at
** your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, <http://www.gnu.org/licenses/>.
**
*/

#pragma once
#include <map>
#include <queue>
#include <tuple>
#include "protocol_handler.h"
#include "vci.h"

/**
	Class that holds data about 1 channel
**/

#define CHANNEL_MAX_FILTERS 10

/// <summary>
/// Struct for storing filter data
/// </summary>
struct handler_filter {
	uint8_t id; // Unique ID per channel
	uint8_t type; // Type of filter
	PASSTHRU_MSG mask; // Mask
	PASSTHRU_MSG filter; // Filter
	PASSTHRU_MSG flow; // Flow control CAN ID for ISO15765
};

class channel
{
public:
    channel(unsigned long id);

    int setProtocol(unsigned long ProtocolID);
    int setFlags(unsigned long Flags);
    int setBaud(unsigned long Baudrate);
    int connectVCI();

    int sendPayload(PASSTHRU_MSG* msg);

    int setFilter(
        unsigned long FilterType,
        PASSTHRU_MSG* pMaskMsg,
        PASSTHRU_MSG* pPatternMsg,
        PASSTHRU_MSG* pFlowControlMsg,
        unsigned long* pFilterID
    );

    int remove_filter(unsigned long filterID);
    int removeChannel();

    void recvData(uint8_t* m, uint16_t len);

    int requestData(
        PASSTHRU_MSG* pMsg,
        unsigned long* pNumMsgs,
        unsigned long Timeout
    );

private:
    protocol_handler* handler = nullptr;
    handler_filter* filters[CHANNEL_MAX_FILTERS] = { nullptr };

    unsigned long id;
    unsigned long protocolID = 0;
    unsigned long flags = 0;
    unsigned long baudrate = 0;
};


/**
	Class that holds a group of channels
**/
class channel_group {
#define MAX_CHANNELS 10
private:
	std::map<unsigned long, channel> channels;
	unsigned long getFreeChannelID();
	bool used[MAX_CHANNELS] = { false };
public:
	int setFilter(unsigned long channel_id, unsigned long FilterType, PASSTHRU_MSG* pMaskMsg, PASSTHRU_MSG* pPatternMsg, PASSTHRU_MSG* pFlowControlMsg, unsigned long* pFilterID);
	int remove_filter(unsigned long channel_id,  unsigned long filterID);
	int send_payload(unsigned long channel_id, PASSTHRU_MSG *pMsg, unsigned long* pNumMsgs, unsigned long timeout);
	channel* getChannelWithID(unsigned long id);
	std::tuple<int, unsigned long> addChannel(unsigned long ProtocolID, unsigned long Flags, unsigned long Baudrate);
	int removeChannel(unsigned long channelid);
	void recvPayload(char * data);
	int requestChannelData(unsigned long ChannelID, PASSTHRU_MSG* pMsg, unsigned long* pNumMsgs, unsigned long Timeout);
};

extern channel_group channels;

