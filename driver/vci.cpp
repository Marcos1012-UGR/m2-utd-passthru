#include "pch.h"
#include "vci.h"
#include "globals.h"
#include "logger.h"
#include "ioctl_handler.h"

int VCI_Open(unsigned long* pDeviceID)
{
    LOGGER.logInfo(
        "VCI",
        "VCI_Open called - pDeviceID=%p",
        pDeviceID
    );

    if (pDeviceID == nullptr) {
        LOGGER.logError(
            "VCI",
            "VCI_Open -> ERR_NULL_PARAMETER"
        );
        return ERR_NULL_PARAMETER;
    }

    *pDeviceID = 1;

    LOGGER.logInfo(
        "VCI",
        "VCI_Open -> STATUS_NOERROR - DeviceID=%lu",
        *pDeviceID
    );

    return STATUS_NOERROR;
}

int VCI_Close(unsigned long DeviceID)
{
    LOGGER.logInfo(
        "VCI",
        "VCI_Close called - DeviceID=%lu",
        DeviceID
    );

    LOGGER.logInfo(
        "VCI",
        "VCI_Close -> STATUS_NOERROR"
    );

    return STATUS_NOERROR;
}

int VCI_Connect(
    unsigned long ChannelID,
    unsigned long ProtocolID,
    unsigned long Flags,
    unsigned long Baudrate)
{
    LOGGER.logInfo(
        "VCI",
        "VCI_Connect called - ChannelID=%lu ProtocolID=%lu Flags=0x%08lX Baudrate=%lu",
        ChannelID,
        ProtocolID,
        Flags,
        Baudrate
    );
    LOGGER.logInfo(
        "VCI",
        "VCI_Connect -> STATUS_NOERROR"
    );
    return STATUS_NOERROR;
}

int VCI_Disconnect(unsigned long ChannelID)
{
    LOGGER.logInfo(
        "VCI",
        "VCI_Disconnect called - ChannelID=%lu",
        ChannelID
    );
    LOGGER.logInfo(
        "VCI",
        "VCI_Disconnect -> STATUS_NOERROR"
    );
    return STATUS_NOERROR;
}

int VCI_Send(
    unsigned long ChannelID,
    PASSTHRU_MSG* pMsg,
    unsigned long NumMsgs,
    unsigned long Timeout)
{
    LOGGER.logInfo(
        "VCI",
        "VCI_Send called - ChannelID=%lu NumMsgs=%lu Timeout=%lu",
        ChannelID,
        NumMsgs,
        Timeout
    );
    if (pMsg != nullptr && NumMsgs > 0)
    {
        LOGGER.logInfo(
            "VCI",
            "VCI_Send[0] - ProtocolID=%lu DataSize=%lu Data=%s",
            pMsg[0].ProtocolID,
            pMsg[0].DataSize,
            LOGGER.bytesToString(
                pMsg[0].Data,
                pMsg[0].DataSize
            ).c_str()
        );
    }
    LOGGER.logInfo(
        "VCI",
        "VCI_Send -> STATUS_NOERROR"
    );
    return STATUS_NOERROR;
}

int VCI_Receive(
    unsigned long ChannelID,
    PASSTHRU_MSG* pMsg,
    unsigned long* pNumMsgs,
    unsigned long Timeout)
{
    LOGGER.logInfo(
        "VCI",
        "VCI_Receive called - ChannelID=%lu pMsg=%p pNumMsgs=%p Timeout=%lu",
        ChannelID,
        pMsg,
        pNumMsgs,
        Timeout
    );

    if (pNumMsgs != nullptr)
    {
        LOGGER.logInfo(
            "VCI",
            "VCI_Receive requested NumMsgs=%lu",
            *pNumMsgs
        );
    }
    if (pNumMsgs == nullptr) {
		LOGGER.logInfo(
			"VCI",
			"VCI_Receive -> ERR_NULL_PARAMETER"
		);
        return ERR_NULL_PARAMETER;
    }

    *pNumMsgs = 0;

    LOGGER.logInfo(
        "VCI",
        "VCI_Receive -> STATUS_NOERROR, NumMsgs=%lu",
        pNumMsgs ? *pNumMsgs : 0
    );
    return STATUS_NOERROR;
}

int VCI_StartPeriodicMsg(
    unsigned long ChannelID,
    PASSTHRU_MSG* pMsg,
    unsigned long* pMsgID,
    unsigned long TimeInterval)
{
    return STATUS_NOERROR;
}

int VCI_StopPeriodicMsg(
    unsigned long ChannelID,
    unsigned long MsgID)
{
    return STATUS_NOERROR;
}

int VCI_StartFilter(
    unsigned long ChannelID,
    unsigned long FilterType,
    PASSTHRU_MSG* pMaskMsg,
    PASSTHRU_MSG* pPatternMsg,
    PASSTHRU_MSG* pFlowControlMsg,
    unsigned long* pFilterID)
{
    LOGGER.logInfo(
        "VCI",
        "VCI_StartFilter called - ChannelID=%lu FilterType=%lu Mask=%p Pattern=%p FlowControl=%p FilterID=%p",
        ChannelID,
        FilterType,
        pMaskMsg,
        pPatternMsg,
        pFlowControlMsg,
        pFilterID
    );

    if (pFilterID != nullptr) {
		LOGGER.logInfo(
			"VCI",
			"VCI_StartFilter -> STATUS_NOERROR - FilterID=%lu",
			1
		);
        *pFilterID = 1;
    }
	LOGGER.logInfo(
		"VCI",
		"VCI_StartFilter -> STATUS_NOERROR"
	);
    return STATUS_NOERROR;
}

int VCI_StopFilter(
    unsigned long ChannelID,
    unsigned long FilterID)
{
    LOGGER.logInfo(
        "VCI",
        "VCI_StopFilter called - ChannelID=%lu FilterID=%lu",
        ChannelID,
        FilterID
    );

    LOGGER.logInfo(
        "VCI",
        "VCI_StopFilter -> STATUS_NOERROR"
    );
    return STATUS_NOERROR;
}

int VCI_SetProgrammingVoltage(
    unsigned long DeviceID,
    unsigned long PinNumber,
    unsigned long Voltage)
{
    LOGGER.logInfo(
        "VCI",
        "VCI_SetProgrammingVoltage called - DeviceID=%lu Pin=%lu Voltage=%lu",
        DeviceID,
        PinNumber,
        Voltage
    );
    LOGGER.logInfo(
        "VCI",
        "VCI_SetProgrammingVoltage -> STATUS_NOERROR"
    );
    return STATUS_NOERROR;
}

int VCI_Ioctl(
    unsigned long ChannelID,
    unsigned long IoctlID,
    void* pInput,
    void* pOutput)
{
    LOGGER.logInfo(
        "VCI",
        "VCI_Ioctl called - ChannelID=%lu IoctlID=%lu pInput=%p pOutput=%p",
        ChannelID,
        IoctlID,
        pInput,
        pOutput
    );

    int result = STATUS_NOERROR;

    switch (IoctlID)
    {
    case READ_VBATT:
        result = ioctl_handler::read_batt(
            static_cast<unsigned long*>(pOutput)
        );
        break;

    case READ_PROG_VOLTAGE:
        result = ioctl_handler::read_prog_voltage(
            static_cast<unsigned long*>(pOutput)
        );
        break;

    case SET_CONFIG:
        result = ioctl_handler::set_config(
            ChannelID,
            static_cast<SCONFIG_LIST*>(pInput)
        );
        break;

    case GET_CONFIG:
        result = ioctl_handler::get_config(
            ChannelID,
            static_cast<SCONFIG_LIST*>(pInput)
        );
        break;

    case FIVE_BAUD_INIT:
        result = ioctl_handler::five_baud_init(
            ChannelID,
            static_cast<SBYTE_ARRAY*>(pInput),
            static_cast<SBYTE_ARRAY*>(pOutput)
        );
        break;

    case FAST_INIT:
        result = ioctl_handler::fast_init(
            ChannelID,
            static_cast<PASSTHRU_MSG*>(pInput),
            static_cast<PASSTHRU_MSG*>(pOutput)
        );
        break;

    case CLEAR_TX_BUFFER:
        result = ioctl_handler::clear_tx_buffers(
            ChannelID
        );
        break;

    case CLEAR_RX_BUFFER:
        result = ioctl_handler::clear_rx_buffers(
            ChannelID
        );
        break;

    case CLEAR_PERIODIC_MSGS:
        result = ioctl_handler::clear_periodic_msgs(
            ChannelID
        );
        break;

    case CLEAR_MSG_FILTERS:
        result = ioctl_handler::clear_msg_filters(
            ChannelID
        );
        break;

    case CLEAR_FUNCT_MSG_LOOKUP_TABLE:
        result = ioctl_handler::clear_mlt(
            ChannelID
        );
        break;

    case ADD_TO_FUNCT_MSG_LOOKUP_TABLE:
        result = ioctl_handler::add_to_mlt(
            ChannelID,
            static_cast<SBYTE_ARRAY*>(pInput)
        );
        break;

    case DELETE_FROM_FUNCT_MSG_LOOKUP_TABLE:
        result = ioctl_handler::del_from_mlt(
            ChannelID,
            static_cast<SBYTE_ARRAY*>(pInput)
        );
        break;

    default:
        LOGGER.logError(
            "VCI",
            "VCI_Ioctl -> unsupported IoctlID=%lu",
            IoctlID
        );

        globals::setErrorString(
            "Unsupported IOCTL command"
        );

        return ERR_NOT_SUPPORTED;
    }

    LOGGER.logInfo(
        "VCI",
        "VCI_Ioctl -> result=%d",
        result
    );

    return result;
}