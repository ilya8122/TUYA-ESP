#define __DEVICE_GLOBALS

#include "tuya_smart_api.h"
//#include "wf_sdk_adpt.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tuya_msg.h"

void tuya_msg(UINT dpid, UINT num_int, UINT type)  // send msg to tuya func
{
  PR_DEBUG("tuya_msg");

  GW_WIFI_STAT_E wf_stat = tuya_get_wf_status();
  if ((wf_stat < STAT_STA_CONN) || (tuya_get_gw_status() != STAT_WORK)) {
    PR_DEBUG("return WM_FAIL");
    return;
  }
  cJSON *root = cJSON_CreateObject();
  if (NULL == root) {
    PR_DEBUG("return WM_FAIL");
    return;
  }
  switch (dpid) {
    case 1:
      if (type = 1) {
        cJSON_AddFalseToObject(root, "1");
      }
      if (type = 2) {
        cJSON_AddTrueToObject(root, "1");
      }
      break;

    case 17:
      if (type = 3) {
        cJSON_AddNumberToObject(root, "17", num_int);
      }  // Energy
      break;

    case 18:
      if (type = 3) {
        cJSON_AddNumberToObject(root, "18", num_int);
      }  // I
      break;

    case 19:
      if (type = 3) {
        cJSON_AddNumberToObject(root, "19", num_int);
      }  // p
      break;

    case 20:
      if (type = 3) {
        cJSON_AddNumberToObject(root, "20", num_int);
      }  // U
      break;

    case 103:
      if (type = 3) {
        cJSON_AddNumberToObject(root, "103", num_int);
      }  // w1
      break;

    case 104:
      if (type = 3) {
        cJSON_AddNumberToObject(root, "104", num_int);
      }  // w2
      break;

    case 106:
      if (type = 3) {
        // char buf[16];sprintf(buf,"%d",num_int);const char* p = buf;
        // cJSON_AddStringToObject(root,"106",buf);
        cJSON_AddNumberToObject(root, "106", num_int);
      }  // ADC
      break;

    case 110:
      if (type = 1) {
        cJSON_AddFalseToObject(root, "110");
      }
      if (type = 2) {
        cJSON_AddTrueToObject(root, "110");
      }
      break;

    default:
      PR_DEBUG("NO DPID");
      break;
  }
  CHAR *out = cJSON_PrintUnformatted(root);
  cJSON_Delete(root);
  if (NULL == out) {
    PR_ERR("cJSON_PrintUnformatted err:");
    PR_DEBUG("return WM_FAIL");
    return;
  }
  PR_DEBUG("out:  %s ", out);
  OPERATE_RET op_ret = tuya_obj_dp_report(out);
  Free(out);

  if (OPRT_OK == op_ret) {
    PR_DEBUG("return suc");
    return;
  } else {
    PR_DEBUG("return WM_FAIL");
    return;
  }
}
