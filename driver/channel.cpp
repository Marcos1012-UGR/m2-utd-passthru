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

#include "vci.h"
#include "pch.h"
#include "channel.h"
#include "Logger.h"
#include "globals.h"



std::tuple<int, unsigned long> channel_group::addChannel(
    unsigned long ProtocolID,
    unsigned long Flags,
    unsigned long Baudrate)
{
    unsigned long chanid = getFreeChannelID();

    if (chanid == 0) {
        LOGGER.logError("CHAN_GROUP", "Error creating channel!");
        globals::setErrorString("No more free channels");
        return std::make_tuple(ERR_FAILED, 0);
    }

    channel c(chanid);

    int res = c.setProtocol(ProtocolID);
    if (res != STATUS_NOERROR) {
        LOGGER.logError(
            "CHAN_GROUP",
            "Error setting channel protocol!"
        );
        used[chanid - 1] = false;
        return std::make_tuple(res, 0);
    }

    res = c.setFlags(Flags);
    if (res != STATUS_NOERROR) {
        LOGGER.logError(
            "CHAN_GROUP",
            "Error setting channel flags!"
        );
        used[chanid - 1] = false;
        return std::make_tuple(res, 0);
    }

    res = c.setBaud(Baudrate);
    if (res != STATUS_NOERROR) {
        LOGGER.logError(
            "CHAN_GROUP",
            "Error setting channel baudrate!"
        );
        used[chanid - 1] = false;
        return std::make_tuple(res, 0);
    }

    res = c.connectVCI();
    if (res != STATUS_NOERROR) {
        LOGGER.logError(
            "CHAN_GROUP",
            "Error connecting channel to VCI!"
        );
        used[chanid - 1] = false;
        return std::make_tuple(res, 0);
    }

    this->channels.emplace(std::make_pair(chanid, c));

    LOGGER.logDebug(
        "CHAN_GROUP",
        "Created channel OK. Id is %lu",
        chanid
    );

    return std::make_tuple(STATUS_NOERROR, chanid);
}

int channel_group::removeChannel(unsigned long channelid)
{
    if (channelid == 0 || channelid > MAX_CHANNELS)
    {
        LOGGER.logError(
            "CHAN_GROUP",
            "Invalid channel ID %lu",
            channelid
        );

        return ERR_INVALID_CHANNEL_ID;
    }

    auto it = channels.find(channelid);

    if (it == channels.end())
    {
        LOGGER.logDebug(
            "CHAN_GROUP",
            "Channel %lu does not exist",
            channelid
        );

        return ERR_INVALID_CHANNEL_ID;
    }

    int ret = it->second.removeChannel();

    if (ret != STATUS_NOERROR)
    {
        LOGGER.logError(
            "CHAN_GROUP",
            "VCI_Disconnect failed for channel %lu: %d",
            channelid,
            ret
        );

        return ret;
    }

    channels.erase(it);
    used[channelid - 1] = false;

    LOGGER.logDebug(
        "CHAN_GROUP",
        "Channel %lu removed",
        channelid
    );

    return STATUS_NOERROR;
}

void channel_group::recvPayload(char * data)
{
    
}

int channel_group::requestChannelData(unsigned long ChannelID, PASSTHRU_MSG* pMsg, unsigned long* pNumMsgs, unsigned long Timeout)
{
   channel* chan = getChannelWithID(ChannelID);
   if (chan == nullptr) {
       return ERR_INVALID_CHANNEL_ID;
   }
   return chan->requestData(pMsg, pNumMsgs, Timeout);
}

unsigned long channel_group::getFreeChannelID()
{
    for (int i = 0; i < MAX_CHANNELS; i++) {
        if (used[i] == false) { // Channel isn't being used
            used[i] = true; // Channel is now marked as used
            return i + 1; // Return +1 to where we are in the array (ID 0 indicates no channel created)
        }
    }
    LOGGER.logError("CHAN_GROUP", "No free channels!");
    return 0;
}

int channel_group::setFilter(unsigned long channel_id, unsigned long FilterType, PASSTHRU_MSG* pMaskMsg, PASSTHRU_MSG* pPatternMsg, PASSTHRU_MSG* pFlowControlMsg, unsigned long* pFilterID)
{
    channel* chan = getChannelWithID(channel_id);
    if (chan == nullptr) {
        return ERR_INVALID_CHANNEL_ID;
    }  
    return chan->setFilter(FilterType, pMaskMsg, pPatternMsg, pFlowControlMsg, pFilterID);
}

int channel_group::remove_filter(unsigned long channel_id, unsigned long filterID)
{
    channel* chan = getChannelWithID(channel_id);
    if (chan == nullptr) {
        return ERR_INVALID_CHANNEL_ID;
    }
    return chan->remove_filter(filterID);
}

int channel_group::send_payload(
    unsigned long channel_id,
    PASSTHRU_MSG* pMsg,
    unsigned long* pNumMsgs,
    unsigned long timeout)
{
    channel* chan = getChannelWithID(channel_id);

    if (chan == nullptr) {
        return ERR_INVALID_CHANNEL_ID;
    }

    if (pMsg == nullptr || pNumMsgs == nullptr) {
        return ERR_NULL_PARAMETER;
    }

    LOGGER.logInfo(
        "CHAN_SEND",
        "Sending %lu messages to channel %lu",
        *pNumMsgs,
        channel_id
    );

    for (unsigned long i = 0; i < *pNumMsgs; i++) {
        int result = VCI_Send(
            channel_id,
            &pMsg[i],
            1,
            timeout
        );

        if (result != STATUS_NOERROR) {
            return result;
        }
    }

    return STATUS_NOERROR;
}

channel* channel_group::getChannelWithID(unsigned long id)
{
    try {
        return &this->channels.at(id);
    }
    catch (std::exception) {
        return nullptr;
    }
}

channel_group channels = channel_group();

channel::channel(unsigned long id)
{
    this->id = id;
}

int channel::setProtocol(unsigned long ProtocolID)
{
    switch (ProtocolID) {
    case ISO15765:
        this->handler = new iso15765_handler(this->id);
        break;

    case ISO9141:
        this->handler = new iso9141_handler(this->id);
        break;

    case CAN:
        this->handler = new can_handler(this->id);
        break;

    default:
        LOGGER.logError(
            "CHAN_PROT",
            "Unsupported protocol %lu",
            ProtocolID
        );
        return ERR_INVALID_PROTOCOL_ID;
    }

    this->protocolID = ProtocolID;

    return STATUS_NOERROR;
}

int channel::setFlags(unsigned long Flags)
{
    if (this->handler == nullptr) {
        globals::setErrorString("Handler is null");
        return ERR_FAILED;
    }

    this->flags = Flags;
    this->handler->setFlags(Flags);

    return STATUS_NOERROR;
}

int channel::setBaud(unsigned long Baudrate)
{
    if (this->handler == nullptr) {
        globals::setErrorString("Handler is null");
        return ERR_FAILED;
    }

    this->baudrate = Baudrate;
    this->handler->setBaud(Baudrate);

    return STATUS_NOERROR;
}

int channel::connectVCI()
{
    if (this->handler == nullptr) {
        globals::setErrorString("Handler is null");
        return ERR_FAILED;
    }

    return VCI_Connect(
        this->id,
        this->protocolID,
        this->flags,
        this->baudrate
    );
}

int channel::sendPayload(PASSTHRU_MSG* msg)
{
    if (msg == nullptr) {
        return ERR_NULL_PARAMETER;
    }

    return VCI_Send(
        this->id,
        msg,
        1,
        0
    );
}

int channel::setFilter(
    unsigned long FilterType,
    PASSTHRU_MSG* pMaskMsg,
    PASSTHRU_MSG* pPatternMsg,
    PASSTHRU_MSG* pFlowControlMsg,
    unsigned long* pFilterID)
{
    if (pFilterID == nullptr) {
        return ERR_NULL_PARAMETER;
    }

    return VCI_StartFilter(
        this->id,
        FilterType,
        pMaskMsg,
        pPatternMsg,
        pFlowControlMsg,
        pFilterID
    );
}

int channel::remove_filter(unsigned long filterID)
{
    return VCI_StopFilter(
        this->id,
        filterID
    );
}

int channel::removeChannel()
{
    return VCI_Disconnect(this->id);
}

void channel::recvData(uint8_t* m, uint16_t len)
{
    if (this->handler != nullptr) {
        this->handler->recvData(m, len);
    }
}

int channel::requestData(
    PASSTHRU_MSG* pMsg,
    unsigned long* pNumMsgs,
    unsigned long Timeout)
{
    if (pMsg == nullptr || pNumMsgs == nullptr) {
        return ERR_NULL_PARAMETER;
    }

    return VCI_Receive(
        this->id,
        pMsg,
        pNumMsgs,
        Timeout
    );
}