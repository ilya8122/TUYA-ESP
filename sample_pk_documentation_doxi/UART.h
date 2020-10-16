#include "device.h"
#include "tuya_smart_api.h"
#include "wf_sdk_adpt.h"

#include "HEX_TO_DEC.h"
#include "define.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
@brief reads energy meter readings via UART and converts them to current, voltage, and power values
*/
void read_uart();
/**
@brief sends power, current, voltage, and amount of energy to the cloud
*/
void t_msg_energy();
/**
@brief resets the energy count
*/
void energy_to_null();
/**
@brief resets the energy count
@param U_MAX maximum voltage value
@param U_MIN minimum voltage value
@param I_MAX maximum current value
@param Rele handler
*/
void def_I_U(int U_MAX, int U_MIN, int I_MAX, LED_HANDLE Rele);
