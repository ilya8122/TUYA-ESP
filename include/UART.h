#include "device.h"
#include "tuya_smart_api.h"
#include "wf_sdk_adpt.h"

//#include "adclib.h"
#include "HEX_TO_DEC.h"
#include "RES_TO_TEMP.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "define.h"

void read_uart();

int   get_U_value();//use after read uart

float get_I_value();//use after read uart

float get_P_calc_value();//use after read uart

float get_ENERGY_value();//use after read uart

