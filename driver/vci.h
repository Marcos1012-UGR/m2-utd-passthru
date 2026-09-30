#pragma once

#include "j2534_v0404.h"

int VCI_Open(unsigned long* pDeviceID);

int VCI_Close(unsigned long DeviceID);

int VCI_Connect(
    unsigned long ChannelID,
    unsigned long ProtocolID,
    unsigned long Flags,
    unsigned long Baudrate
);

int VCI_Disconnect(
    unsigned long ChannelID
);

int VCI_Send(
    unsigned long ChannelID,
    PASSTHRU_MSG* pMsg,
    unsigned long NumMsgs,
    unsigned long Timeout
);

int VCI_Receive(
    unsigned long ChannelID,
    PASSTHRU_MSG* pMsg,
    unsigned long* pNumMsgs,
    unsigned long Timeout
);

int VCI_StartPeriodicMsg(
    unsigned long ChannelID,
    PASSTHRU_MSG* pMsg,
    unsigned long* pMsgID,
    unsigned long TimeInterval
);

int VCI_StopPeriodicMsg(
    unsigned long ChannelID,
    unsigned long MsgID
);

int VCI_StartFilter(
    unsigned long ChannelID,
    unsigned long FilterType,
    PASSTHRU_MSG* pMaskMsg,
    PASSTHRU_MSG* pPatternMsg,
    PASSTHRU_MSG* pFlowControlMsg,
    unsigned long* pFilterID
);

int VCI_StopFilter(
    unsigned long ChannelID,
    unsigned long FilterID
);

int VCI_SetProgrammingVoltage(
    unsigned long DeviceID,
    unsigned long PinNumber,
    unsigned long Voltage
);

int VCI_Ioctl(
    unsigned long ChannelID,
    unsigned long IoctlID,
    void* pInput,
    void* pOutput
);