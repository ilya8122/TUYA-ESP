#include "device.h"
#include "tuya_smart_api.h"
#include "wf_sdk_adpt.h"

#include "HEX_TO_DEC.h"
#include "define.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void read_uart();

void t_msg_energy();

void energy_to_null();

void def_I_U(int U_MAX, int U_MIN, int I_MAX, LED_HANDLE Rele);
