#define __DEVICE_GLOBALS

#include "tuya_smart_api.h"
#include "wf_sdk_adpt.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/**
@brief sends a message to the cloud
@param dpid ID of the item in the cloud
@param value_int the value of the item
@param type type
*/
void tuya_msg(UINT dpid,UINT value_int,UINT type);
