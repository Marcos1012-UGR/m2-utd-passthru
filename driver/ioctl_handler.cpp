#include "pch.h"
#include "ioctl_handler.h"
#include "Logger.h"
#include "channel.h"
#include "globals.h"

int ioctl_handler::set_config(unsigned long channelID, SCONFIG_LIST* pInput)
{
	return STATUS_NOERROR;
}

int ioctl_handler::get_config(unsigned long channelID, SCONFIG_LIST* pInput)
{
    return STATUS_NOERROR;
}

int ioctl_handler::read_batt(unsigned long* vbatt)
{
    if (vbatt == nullptr) {
        globals::setErrorString("READ_VBATT: output pointer is null");
        LOGGER.logError(
            "IOCTL",
            "READ_VBATT called with null output pointer"
        );
        return ERR_NULL_PARAMETER;
    }

    *vbatt = globals::getBatVoltage();

    LOGGER.logInfo(
        "IOCTL",
        "READ_VBATT -> %lu mV",
        *vbatt
    );

    return STATUS_NOERROR;
}

int ioctl_handler::read_prog_voltage(unsigned long* vProg)
{
    if (vProg == nullptr) { return ERR_NULLPARAMETER; }
    LOGGER.logDebug("IOCTL", "READ_PROGRAMMING_VOLTAGE called");
    return STATUS_NOERROR;
}

int ioctl_handler::five_baud_init(unsigned long channelID, SBYTE_ARRAY* pInput, SBYTE_ARRAY* pOutput)
{
    if (pInput == nullptr || pOutput == nullptr) { return ERR_NULLPARAMETER; }
    LOGGER.logDebug("IOCTL", "FIVE_BAUD_INIT called with %d target ECU addresses", pInput->NumOfBytes);
    return STATUS_NOERROR;
}

int ioctl_handler::fast_init(unsigned long channelID, PASSTHRU_MSG* pInput, PASSTHRU_MSG* pOutput)
{
    if (pInput == nullptr || pOutput == nullptr) { return ERR_NULLPARAMETER; }
    LOGGER.logDebug("IOCTL", "FAST_INIT called");
    return STATUS_NOERROR;
}

int ioctl_handler::clear_tx_buffers(unsigned long channelID)
{
    LOGGER.logDebug("IOCTL", "CLEAR_TX_BUFFERS called");
    return STATUS_NOERROR;
}

int ioctl_handler::clear_rx_buffers(unsigned long channelID)
{
    LOGGER.logDebug("IOCTL", "CLEAR_RX_BUFFERS called");
    return STATUS_NOERROR;
}

int ioctl_handler::clear_periodic_msgs(unsigned long channelID)
{
    LOGGER.logDebug("IOCTL", "CLEAR_PERIODIC_MSGS called");
    return STATUS_NOERROR;
}

int ioctl_handler::clear_msg_filters(unsigned long channelID)
{
    LOGGER.logDebug("IOCTL", "CLEAR_MSG_FILTERS called");
    return STATUS_NOERROR;
}

int ioctl_handler::clear_mlt(unsigned long channelID)
{
    LOGGER.logDebug("IOCTL", "CLEAR_FUNCT_MSG_LOOKUP_TABLE called");
    return STATUS_NOERROR;
}

int ioctl_handler::add_to_mlt(unsigned long channelID, SBYTE_ARRAY* pInput)
{
    LOGGER.logDebug("IOCTL", "ADD_TO_FUNCT_MSG_LOOKUP_TABLE called");
    return STATUS_NOERROR;
}

int ioctl_handler::del_from_mlt(unsigned long channelID, SBYTE_ARRAY* pInput)
{
    LOGGER.logDebug("IOCTL", "DELETE_FROM_FUNCT_MSG_LOOKUP_TABLE called");
    return STATUS_NOERROR;
}
