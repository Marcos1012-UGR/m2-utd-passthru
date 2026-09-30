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

#include "pch.h"
#include "nyanko-passthru.h"
#include "Logger.h"
#include "globals.h"
#include "channel.h"
#include <tuple>
#include "vci.h"


/*
http://www.drewtech.com/support/passthru/open.html
Establish a logical communication channel with the vehicle network (via the PassThru device) using the specified network layer protocol and selected protocol options.
*/
DllExport PassThruOpen(void* pName, unsigned long* pDeviceID) {
	LOGGER.logInfo("DllExport", "PassThruOpen called");

	if (pDeviceID == nullptr) {
		return ERR_NULL_PARAMETER;
	}

	return VCI_Open(pDeviceID);
}

/*
http://www.drewtech.com/support/passthru/close.html
Close all communication with the PassThru device. All channels will be disconnected from the network,
periodic messages will halt, and the hardware will return to its default state.
*/
DllExport PassThruClose(unsigned long DeviceID) {
	LOGGER.logInfo("DllExport", "PassThruClose called");

	return VCI_Close(DeviceID);
}

/*
http://www.drewtech.com/support/passthru/connect.html
Establish a logical communication channel with the vehicle network (via the PassThru device) using the specified network layer protocol and selected protocol options.
*/
DllExport PassThruConnect(unsigned long DeviceID,
	unsigned long ProtocolID,
	unsigned long Flags,
	unsigned long Baudrate,
	unsigned long* pChannelID)
{
	LOGGER.logInfo(
		"DllExport",
		"PassThruConnect called - DeviceID=%lu ProtocolID=%lu Flags=0x%08lX Baudrate=%lu pChannelID=%p",
		DeviceID,
		ProtocolID,
		Flags,
		Baudrate,
		pChannelID
	);

	if (pChannelID == nullptr)
	{
		LOGGER.logError(
			"DllExport",
			"PassThruConnect -> pChannelID is NULL"
		);

		return ERR_NULL_PARAMETER;
	}

	int res_code;
	unsigned long chan_id;

	std::tie(res_code, chan_id) =
		channels.addChannel(
			ProtocolID,
			Flags,
			Baudrate
		);

	LOGGER.logInfo(
		"DllExport",
		"PassThruConnect -> addChannel returned %d, channel=%lu",
		res_code,
		chan_id
	);

	if (res_code != STATUS_NOERROR)
	{
		return res_code;
	}

	*pChannelID = chan_id;

	LOGGER.logInfo(
		"DllExport",
		"PassThruConnect -> SUCCESS ChannelID=%lu",
		*pChannelID
	);

	return STATUS_NOERROR;
}

/*
http://www.drewtech.com/support/passthru/disconnect.html
Terminate an existing logical communication channel between the User Application and the vehicle network (via the PassThru device).
Once disconnected the channel identifier or handle is invalid. For the associated network protocol this function will terminate
the transmitting of periodic messages and the filtering of receive messages. The PassThru device periodic and filter message tables
will be cleared.
*/
DllExport PassThruDisconnect(unsigned long ChannelID)
{
	LOGGER.logInfo(
		"DllExport",
		"PassThruDisconnect called - ChannelID=%lu",
		ChannelID
	);

	int result = channels.removeChannel(ChannelID);

	LOGGER.logInfo(
		"DllExport",
		"PassThruDisconnect -> result=%d",
		result
	);

	return result;
}

/*
http://www.drewtech.com/support/passthru/readmsgs.html
Receive network protocol messages, receive indications, and transmit indications from an existing logical communication channel.
Messages will flow through PassThru device to the User Application..
*/
DllExport PassThruReadMsgs(unsigned long ChannelID,
	PASSTHRU_MSG* pMsg,
	unsigned long* pNumMsgs,
	unsigned long Timeout) {
	return channels.requestChannelData(
		ChannelID,
		pMsg,
		pNumMsgs,
		Timeout
	);
}

/*
http://www.drewtech.com/support/passthru/writemsgs.html
Transmit network protocol messages over an existing logical communication channel. Messages will flow through PassThru device to the vehicle network.
*/
DllExport PassThruWriteMsgs(unsigned long ChannelID,
	PASSTHRU_MSG* pMsg,
	unsigned long* pNumMsgs,
	unsigned long Timeout) {
	LOGGER.logInfo("DllExport", "PassThruWriteMsgs called");

	return channels.send_payload(
		ChannelID,
		pMsg,
		pNumMsgs,
		Timeout
	);
}

/*
http://www.drewtech.com/support/passthru/startperiodicmsg.html
Repetitively transmit network protocol messages at the specified time interval over an existing logical communication channel.
There is a limit of ten periodic messages per network layer protocol.
*/
DllExport PassThruStartPeriodicMsg(unsigned long ChannelID,
	PASSTHRU_MSG* pMsg,
	unsigned long* pMsgID,
	unsigned long TimeInterval) {
	LOGGER.logInfo("DllExport", "PassThruStartPeriodicMsg called");

	return VCI_StartPeriodicMsg(
		ChannelID,
		pMsg,
		pMsgID,
		TimeInterval
	);
}

/*
http://www.drewtech.com/support/passthru/stopperiodicmsg.html
Terminate the specified periodic message. Once terminated the message identifier or handle value is invalid
*/
DllExport PassThruStopPeriodicMsg(unsigned long ChannelID,
	unsigned long MsgID) {
	LOGGER.logInfo("DllExport", "PassThruStopPeriodicMsg called");

	return VCI_StopPeriodicMsg(ChannelID, MsgID);
}

/*
http://www.drewtech.com/support/passthru/startmsgfilter.html
The PassThruStartMsgFilter function is used to setup a network protocol filter that will selectively restrict or
limit network protocol messages received by the PassThru device. The filter messages will flow from the User Application
to the PassThru device. There is a limit of ten filter messages per network layer protocol.
The PassThru device will block all vehicle network receive frames by default, when no filters are defined.
The CLEAR_RX_BUFFER (PassThruIoctl function) command must be used after establishing filters to ensure that the receive
queue only contains receive frames that adhere to the filter criteria. The PassThruStartMsgFilter function does not cause
existing receive messages to be removed from the PassThru device receive queue.
*/
DllExport PassThruStartMsgFilter(unsigned long ChannelID,
	unsigned long FilterType,
	PASSTHRU_MSG* pMaskMsg,
	PASSTHRU_MSG* pPatternMsg,
	PASSTHRU_MSG* pFlowControlMsg,
	unsigned long* pFilterID) {
	LOGGER.logInfo("DllExport", "PassThruStartMsgFilter called");

	return channels.setFilter(
		ChannelID,
		FilterType,
		pMaskMsg,
		pPatternMsg,
		pFlowControlMsg,
		pFilterID
	);
}

/*
http://www.drewtech.com/support/passthru/stopmsgfilter.html
Terminate the specified network protocol filter. Once terminated the filter identifier or handle value is invalid.
*/
DllExport PassThruStopMsgFilter(unsigned long ChannelID,
	unsigned long FilterID) {
	LOGGER.logInfo("DllExport", "PassThruStopMsgFilter called");

	return channels.remove_filter(ChannelID, FilterID);
}

/*
http://www.drewtech.com/support/passthru/setprogramming.html
Output a programmable voltage on the specified J1962 connector pin.
Only one pin can have a specified voltage applied at a time. The only exception: it is permissible to
program pin 15 for SHORT_TO_GROUND, and another pin to a voltage level.
When switching pins, the user application must disable the first voltage (VOLTAGE_OFF option)
before enabling the second. The user application protect against applying any incorrect voltage levels.
A current in excess of 200mA will damage CarDAQ; do not ground the FEPS line while energized, even briefly.
*/
DllExport PassThruSetProgrammingVoltage(unsigned long DeviceID,
	unsigned long PinNumber,
	unsigned long Voltage) {
	LOGGER.logInfo("DllExport", "PassThruSetProgrammingVoltage called");

	return VCI_SetProgrammingVoltage(
		DeviceID,
		PinNumber,
		Voltage
	);
}

/*
http://www.drewtech.com/support/passthru/readversion.html
Retrieve the PassThru device firmware version, the PassThru device DLL version,
and the version of the J2534 specification that was referenced. The version information is in the form of NULL terminated strings.
*/
DllExport PassThruReadVersion(unsigned long DeviceID,
	char* pFirmwareVersion,
	char* pDllVersion,
	char* pApiVersion) {
	LOGGER.logInfo("DllExport", "PassThruReadVersion called");

	if (pFirmwareVersion == nullptr ||
		pDllVersion == nullptr ||
		pApiVersion == nullptr) {
		return ERR_NULL_PARAMETER;
	}

	memcpy(pFirmwareVersion, FIRMWARE_VERSION, sizeof(FIRMWARE_VERSION));
	memcpy(pDllVersion, DLL_VERSION, sizeof(DLL_VERSION));
	memcpy(pApiVersion, API_VERSION, sizeof(API_VERSION));

	return STATUS_NOERROR;
}

/*
http://www.drewtech.com/support/passthru/getlasterror.html
Retrieve a text description for the most recent PassThru error as a null terminated C-string. Call this function immediately after an error occurs.
The error string refers to the most recent function call, rather than a specific DeviceID or ChannelID, and any subsequent function call may clobber the description.
*/
DllExport PassThruGetLastError(char* pErrorDescription) {
	LOGGER.logInfo("DllExport", "PassThruGetLastError called");

	if (pErrorDescription == nullptr) {
		LOGGER.logError(
			"DllExport",
			"Error description is a null pointer!?"
		);
		return ERR_NULL_PARAMETER;
	}

	std::string error = globals::getErrorString();

	memcpy(
		pErrorDescription,
		error.c_str(),
		error.size() + 1
	);

	return STATUS_NOERROR;
}

/*
http://www.drewtech.com/support/passthru/ioctl.html
The PassThruIoctl function is a general purpose I/O control function for modifying the vehicle network interface's characteristics.
*/
DllExport PassThruIoctl(unsigned long ChannelID,
	unsigned long IoctlID,
	void* pInput,
	void* pOutput) {
	LOGGER.logInfo("DllExport", "PassThruIoctl called");

	return VCI_Ioctl(
		ChannelID,
		IoctlID,
		pInput,
		pOutput
	);
}