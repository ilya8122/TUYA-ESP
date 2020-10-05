/***********************************************************
*  File: device.c 
*  Author: ILYA
*  Date: 20150605
***********************************************************/
//#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define __DEVICE_GLOBALS
#include "device.h"
#include "tuya_smart_api.h"
#include "wf_sdk_adpt.h"

#include "adclib.h"
#include "HEX_TO_DEC.h"
#include "RES_TO_TEMP.h"
#include "define.h"
#include "UART.h"
#include "tuya_msg.h"


/***********************************************************
*************************micro define***********************
***********************************************************/
TIMER_ID heart_bit_timer;// my timer
TIMER_ID sys_timer;//wf timer
TIMER_ID uart_timer;//wf timer
/***********************************************************
*************************function define********************
***********************************************************/
STATIC OPERATE_RET device_differ_init(VOID);//system init func
STATIC VOID key_process(INT gpio_no,PUSH_KEY_TYPE_E type,INT cnt);//system func

STATIC VOID wfl_timer_cb(UINT timerID,PVOID pTimerArg);//handler system timer
STATIC VOID heart_bit_timer_cb(UINT timerID,PVOID pTimerArg);//handler my timer
STATIC VOID uart_timer_cb(UINT timerID,PVOID pTimerArg);//handler my timer

/***********************************************************
*************************variable define********************
***********************************************************/


/***********************************************************
*************************variable **************************
***********************************************************/
int REGULATOR;
int ISTOCHNIK,LOW_LEVEL,HIGH_LEVEL,temp_min,temp_max;

bool def_protechki, RELE_stat;
adc_config_t adc_config;

int U_MIN,U_MAX,I_MAX;
float ENERGY;

LED_HANDLE Rele = NULL;
LED_HANDLE light = NULL;


// call back function
VOID set_firmware_tp(IN OUT CHAR *firm_name, IN OUT CHAR *firm_ver)//system func
{
	strcpy(firm_name,APP_BIN_NAME);
	strcpy(firm_ver,USER_SW_VER);
	return;
}

BOOL gpio_func_test(VOID)//system func
{
	return TRUE;
}

VOID pre_app_init(VOID)//system func
{
    return;
}

VOID prod_test(BOOL flag,CHAR rssi)//system func
{
     if(flag == FALSE){
         PR_ERR("no auth \r\n");
         return;
     }
     PR_NOTICE("product test mode \r\n");
}

VOID app_init(VOID)//system func
{
    set_console(TRUE);//uart 0(DEBUG)
    app_cfg_set(WCM_LOW_POWER,prod_test);
}

STATIC VOID rele_on(){tuya_set_led_type(Rele,OL_HIGH,0);RELE_stat=true; PR_DEBUG("rele_on");}
STATIC VOID rele_off(){tuya_set_led_type(Rele,OL_LOW,0);RELE_stat=false; PR_DEBUG("rele_off");}

STATIC VOID rele_on_msg(){tuya_set_led_type(Rele,OL_HIGH,0);tuya_msg(1,0,2);RELE_stat=true; PR_DEBUG("rele_on_msg");}
STATIC VOID rele_off_msg(){tuya_set_led_type(Rele,OL_LOW,0);tuya_msg(1,0,1);RELE_stat=false; PR_DEBUG("rele_off_msg");}

STATIC VOID reaction(UINT dpid,UINT dps,char *dps_char)//reaction on json msg func
{
switch(dpid)
{
case 1://rele dpid 

if(dps==0) rele_off();
if(dps==1) rele_on();
break;

case 101:

if(strcmp(dps_char,"Off")==0){REGULATOR=0;}

if(strcmp(dps_char,"Heater")==0){REGULATOR=1;}
if(strcmp(dps_char,"Conditioner")==0){REGULATOR=2;}
if(strcmp(dps_char,"Inrange")==0){REGULATOR=3;}
if(strcmp(dps_char,"OutRange")==0){REGULATOR=4;}

if(strcmp(dps_char,"HeaterW")==0){REGULATOR=5;}
if(strcmp(dps_char,"ConditionerW")==0){REGULATOR=6;}
if(strcmp(dps_char,"InrangeW")==0){REGULATOR=7;}
if(strcmp(dps_char,"OutRangeW")==0){REGULATOR=8;}

if(strcmp(dps_char,"Filling")==0){REGULATOR=9;}
if(strcmp(dps_char,"Drain")==0){REGULATOR=10;}

break;

case 107://temperature min 
temp_min=dps;
break;

case 108://temperature max 
temp_max=dps;
break;

case 109://istochnik protechki choose 
if(strcmp(dps_char,"W1")==0){ISTOCHNIK=0;}
if(strcmp(dps_char,"W2")==0){ISTOCHNIK=1;}
//if(strcmp(dps_char,"W3")==0){ISTOCHNIK=2;}
if(strcmp(dps_char,"W_OR")==0){ISTOCHNIK=2;}
if(strcmp(dps_char,"W_AND")==0){ISTOCHNIK=3;}
break;

case 110://defend protechki  
 if(dps==0)def_protechki=false;
 else if(dps==1)def_protechki=true;
break;

case 111://hi lvl protechki  
if(strcmp(dps_char,"W1")==0){HIGH_LEVEL=0;}
if(strcmp(dps_char,"W2")==0){HIGH_LEVEL=1;}
//if(strcmp(dps_char,"W3")==0){HIGH_LEVEL=2;}

break;

case 112://low lvl protechki  
if(strcmp(dps_char,"W1")==0){LOW_LEVEL=0;}
if(strcmp(dps_char,"W2")==0){LOW_LEVEL=1;}
//if(strcmp(dps_char,"W3")==0){LOW_LEVEL=2;}
break;

case 113://I_MAX
I_MAX=dps;
break;

case 114://u_MAX
U_MAX=dps;
break;

case 115://u_MIN
U_MIN=dps;
break;

	default:
PR_DEBUG("NO DPID");
	break;
}
}


VOID device_cb(SMART_CMD_E cmd,cJSON *root)//json interpritation //system func
{
    CHAR *buf = cJSON_PrintUnformatted(root);

    if(NULL == buf) {PR_ERR("malloc error"); return;}

    PR_DEBUG("root cmd:%s",buf);

   cJSON *nxt = root->child;
	UINT dpid = 0;
	UINT dps_UINT = 0;	
	char dps_Char[20];

	while (nxt) {		
		dpid = atoi(nxt->string);
		
		if (nxt->type == cJSON_False) {
			dps_UINT = 0;PR_DEBUG("dpid:%d, dps:%d", dpid, dps_UINT);
		} else if (nxt->type == cJSON_True) {
			dps_UINT = 1;PR_DEBUG("dpid:%d, dps:%d", dpid, dps_UINT);
		} else if (nxt->type == cJSON_Number) {
			dps_UINT = nxt->valueint;PR_DEBUG("dpid:%d, dps:%d",dpid, dps_UINT);
		} else if (nxt->type == cJSON_String) {
		int i = 0;
		for(; i < 10; i++)
		{dps_Char[i] = *((nxt->valuestring)+i);}		
		PR_DEBUG("dpid:%d, dps:%d, dps str:%s", dpid, dps_UINT,dps_Char);		
		}

		reaction(dpid,dps_UINT,dps_Char);
		nxt = nxt->next;
		SystemSleep(50);
	}

    OPERATE_RET op_ret = tuya_obj_dp_report(buf);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_obj_dp_report err:%d",op_ret);
        PR_DEBUG_RAW("%s\r\n",buf);
        Free(buf);
        return;
    }
    Free(buf);   
}

STATIC VOID dp_qeury_cb(IN CONST TY_DP_QUERY_S *dp_qry)//system func
{

}

OPERATE_RET device_init(VOID)//system init func
{
    PR_NOTICE("fireware info name:%s version:%s",APP_BIN_NAME,USER_SW_VER);

    OPERATE_RET op_ret;

    op_ret = tuya_device_init(PRODECT_KEY,device_cb,USER_SW_VER);
    if(op_ret != OPRT_OK) {
        return op_ret;
    }

    TY_IOT_CBS_S wf_cbs = {
        .dev_dp_query_cb = dp_qeury_cb,
        .ug_reset_inform_cb = NULL,
    };
    gw_register_cbs(&wf_cbs);

    op_ret = device_differ_init();
    if(op_ret != OPRT_OK) {
        return op_ret;
    }

    return op_ret;
}

STATIC VOID wifi_rssi_scan(OPERATE_RET *ret,CHAR *rssi)//system func
{
	signed char result = *rssi;
	if(*ret != OPRT_OK) {
		PR_DEBUG("NO SSID");
	} else {
		PR_DEBUG("RSSI:%d",result);
	}
}

STATIC VOID key_process(INT gpio_no,PUSH_KEY_TYPE_E type,INT cnt)///system func
{
    PR_DEBUG("gpio_no: %d",gpio_no);
    PR_DEBUG("type: %d",type);
    PR_DEBUG("cnt: %d",cnt);

    if(WF_RESET_KEY == gpio_no) {
        if(LONG_KEY == type) {
        	  tuya_dev_reset_select(NW_AP_CFG);
        }else if(SEQ_KEY == type && cnt == 2) {//double klick
            ShowSysMemPoolInfo();
        }else if(NORMAL_KEY == type) {
            PR_NOTICE("remain size:%d",system_get_free_heap_size());
        }
    }
}

STATIC OPERATE_RET device_differ_init(VOID)////system init func (can change)
{
    OPERATE_RET op_ret;

    // key process init
    op_ret = tuya_kb_init();
    if(OPRT_OK  != op_ret) {
        return op_ret;
    }

    // register key to process
    op_ret = tuya_kb_reg_proc(WF_RESET_KEY,3000,key_process);
    if(OPRT_OK  != op_ret) {
        return op_ret;
    }

    // create led handle
    op_ret = tuya_create_led_handle(WF_DIR_LED,&light);
    if(OPRT_OK  != op_ret) {
        return op_ret;
    }

  // create rele handle
    op_ret = tuya_create_led_handle(RELE_PIN,&Rele);
    if(OPRT_OK  != op_ret) {
        return op_ret;
    }

// create timer handle
    op_ret = sys_add_timer(wfl_timer_cb,NULL,&sys_timer);
    if(OPRT_OK != op_ret) {
        return op_ret;
    }else {
        sys_start_timer(sys_timer,300,TIMER_CYCLE);
    }

// create timer uart handle
    op_ret = sys_add_timer(uart_timer_cb,NULL,&uart_timer);
    if(OPRT_OK != op_ret) {
        return op_ret;
    }else {
        sys_start_timer(uart_timer,300,TIMER_CYCLE);
    }
// create timer reg handle
    op_ret = sys_add_timer(heart_bit_timer_cb,NULL,&heart_bit_timer);
       if(OPRT_OK != op_ret) {
           return op_ret;
       }else {
           sys_start_timer(heart_bit_timer,1000,TIMER_CYCLE);
       }
print_port_init(UART1);
user_uart_raw_full_init(BIT_RATE_4800, UART_WordLength_8b,USART_Parity_Even,USART_StopBits_1);
   

adc_config.mode = ADC_READ_TOUT_MODE;
adc_config.clk_div = 8; 
adc_init(&adc_config);

    return OPRT_OK;
}

STATIC VOID wfl_timer_cb(UINT timerID,PVOID pTimerArg)// wf handle sys func
{
    STATIC UINT last_wf_stat = 0xffffffff;
    GW_WIFI_STAT_E wf_stat = tuya_get_wf_status();

    if(last_wf_stat != wf_stat) {
        PR_DEBUG("wf_stat:%d",wf_stat);
        switch(wf_stat) {
            case STAT_UNPROVISION: {
				PR_DEBUG("STAT_UNPROVISION");
                tuya_set_led_type(light,OL_FLASH_HIGH,1000);
            }
            break;

            case STAT_AP_STA_UNCONN:
{
PR_DEBUG("STAT_AP_STA_UNCONN"); 
tuya_set_led_type(light,OL_FLASH_HIGH,1000);
} 
break;
            case STAT_AP_STA_CONN: {
            	PR_DEBUG("STAT_AP_STA_CONN");
            	tuya_set_led_type(light,OL_FLASH_HIGH,1000);
                   }
            break;

			case STAT_LOW_POWER:
            case STAT_STA_UNCONN: {
				PR_DEBUG("STAT_STA_UNCONN");
                tuya_set_led_type(light,OL_LOW,0);
            }
            break;

            case STAT_STA_CONN: {
				PR_DEBUG("STAT_STA_CONN");
                tuya_set_led_type(light,OL_HIGH,0);
            }
            break;
        }
        last_wf_stat = wf_stat;
    }
}


int read_adc_value()
{

uint16_t adc_data[100];
adc_read(&adc_data[0]);
PR_DEBUG("ADC:%d", adc_data[0]);
int adc = adc_data[0];

float U = 0.0009765625*(float)adc;//1/1024
//printf("\nU %f",U);

float r = (core_U*SERIAL_Resistor2-SERIAL_Resistor2*U)/(U);

PR_DEBUG("RESISTANCE:%d", (int)r);
  
int temp =get_temp(r);
PR_DEBUG("temp:%d", temp);
return temp ;
}





STATIC VOID uart_timer_cb(UINT timerID,PVOID pTimerArg)// my timer handle
{
if( tuya_get_gw_status() == STAT_WORK&& tuya_get_wf_status()==STAT_STA_CONN) 
{  
PR_DEBUG("UART TICK\n");
read_uart();
int U= get_U_value();
float ENERGY=get_ENERGY_value();
float I=get_I_value();
float P=get_P_calc_value();
if (U!=0)
	  {
		
		if(U>=U_MAX||U<=U_MIN)
			{
			rele_off_msg();
			}
		else if(I>=I_MAX)
			{
			rele_off_msg();
			}		
	  }

tuya_msg(17,(int)(ENERGY*1000),3);
tuya_msg(18,(int)(I*1000),3);
tuya_msg(19,(int)(P*10),3);
tuya_msg(20,(int)(U*10),3);
}
}

/*STATIC VOID check_protecki(bool def,bool istoch,bool w1,bool w2)
{
if (def==true)
{
switch(istoch)
	{
	case 0:
if(!w1){rele_off_msg();}
	break;

	case 1:
if(!w2){rele_off_msg();}
	break;

	case 2:
if (!w1 || !w2){rele_off_msg();}
	break;

	case 3:
if (!w1 && !w2){rele_off_msg();}
	break;

	default:
PR_DEBUG("ISTOCHNIK-UNKOWN");
	break;
	}
}*/



STATIC VOID heart_bit_timer_cb(UINT timerID,PVOID pTimerArg)// my timer handle
{

//if(RELE_stat){rele_on();}else if(!RELE_stat){rele_off();}

if( tuya_get_gw_status() == STAT_WORK&& tuya_get_wf_status()==STAT_STA_CONN) 
{  
PR_DEBUG("TICK\n");

bool flag;

////////////////////////////////////////////////////////////////////
uint16_t temp = read_adc_value();
 tuya_msg(106,temp*10,3);

bool w1 = tuya_read_gpio_level(W1_io12);
 tuya_msg(103,(int)(!w1),3);

bool w2 = tuya_read_gpio_level(W2_io13);
 tuya_msg(104,(int)(!w2),3);


///////////////////////////////////////////////////////////////////

//check_protecki()



switch(REGULATOR)
{
case 0://off
 
break;

case 1://heater
temp = read_adc_value();
if(temp>=temp_max){rele_off_msg();}
else if (temp<temp_min){rele_on_msg();}
break;


case 2://conditioner
temp = read_adc_value();
if(temp<=temp_min){rele_off_msg();}
else if (temp>=temp_max){rele_on_msg();}
break;


case 3://inrange
if(temp_min<=temp&&temp<=temp_max){rele_on_msg();}
else{rele_off_msg();}
break;



case 4:// OutRange
if(temp_min<=temp&&temp<=temp_max){rele_off_msg();}
else{rele_on_msg();} 
break;



case 5://5 HeaterW
if (def_protechki==true)
{
def_protechki=false; 
tuya_msg(110,0,1);
}
	switch(ISTOCHNIK)
	{
	case 0:
if(!w1){rele_off_msg();}
	break;

	case 1:
if(!w2){rele_off_msg();}
	break;

	case 2:
if (!w1 || !w2){rele_off_msg();}
	break;

	case 3:
if (!w1 && !w2){rele_off_msg();}
	break;

	default:
PR_DEBUG("ISTOCHNIK-UNKOWN");
	break;
	}

if (flag==true)
{
 temp = read_adc_value();
 if(temp>=temp_max){rele_off_msg();}
else if (temp<temp_min){rele_on_msg();}
flag=false;
}
break;

case 6://6 ConditionerW
if (def_protechki==true)
{
def_protechki=false; 
tuya_msg(110,0,1);
}
	switch(ISTOCHNIK)
	{
	case 0:
if(!w1){rele_off_msg();}
	break;
	case 1:
if(!w2){rele_off_msg();}
	break;
	case 2:
if (!w1 || !w2){rele_off_msg();}
	break;
	case 3:
if (!w1 && !w2){rele_off_msg();}
	break;

	default:
PR_DEBUG("ISTOCHNIK-UNKOWN");
	break;
	}

if (flag==true)
{
 temp = read_adc_value();
 if(temp<=temp_min){rele_off_msg();}
else if (temp>=temp_max){rele_on_msg();}
flag=false;
}
break;



case 7://7 InrangeW
if (def_protechki==true)
{
def_protechki=false; 
tuya_msg(110,0,1);
}
switch(ISTOCHNIK)
	{
	case 0:
if(!w1){rele_off_msg();}
	break;
	case 1:
if(!w2){rele_off_msg();}
	break;
	case 2:
if (!w1 || !w2){rele_off_msg();}
	break;
	case 3:
if (!w1 && !w2){rele_off_msg();}
	break;

	default:
PR_DEBUG("ISTOCHNIK-UNKOWN");
	break;
	}

if (flag==true)
{
if(temp_min<=temp&&temp<=temp_max){rele_on_msg();}
else{rele_off_msg();}
flag=false;
}
break;





case 8://8 OutRangeW
if (def_protechki==true)
{
def_protechki=false; 
tuya_msg(110,0,1);
}
	switch(ISTOCHNIK)
	{
	case 0:
if(!w1){rele_off_msg();}
	break;
	case 1:
if(!w2){rele_off_msg();}
	break;
	case 2:
if (!w1 || !w2){rele_off_msg();}
	break;
	case 3:
if (!w1 && !w2){rele_off_msg();}
	break;

	default:
PR_DEBUG("ISTOCHNIK-UNKOWN");
	break;
	}

if (flag==true)
{
if(temp_min<=temp&&temp<=temp_max){rele_off_msg();}
else{rele_on_msg();} 
flag=false;
}
break;





case 9://9 Filling
if (def_protechki==true)
{
def_protechki=false; 
tuya_msg(110,0,1);
}

switch(LOW_LEVEL)
{
case 0:
w1 =tuya_read_gpio_level(W1_io12);
break;

case 1:
w2 =tuya_read_gpio_level(W2_io13);
break;
}
switch(HIGH_LEVEL)
{
case 0:
w1=tuya_read_gpio_level(W1_io12);
break;

case 1:
w2=tuya_read_gpio_level(W2_io13);
break;
}

if(w1==1&&w2==0){rele_on_msg();} 
else if(w1==0&&w2==0){rele_off_msg();} 
else{}
break;




case 10://10 Drain
if (def_protechki==true)
{
def_protechki=false; 
tuya_msg(110,0,1);
}
switch(LOW_LEVEL)
{
case 0:
w1 =tuya_read_gpio_level(W1_io12);
break;

case 1:
w2 =tuya_read_gpio_level(W2_io13);
break;
}
switch(HIGH_LEVEL)
{
case 0:
w1=tuya_read_gpio_level(W1_io12);
break;

case 1:
w2=tuya_read_gpio_level(W2_io13);
break;
}
if(w1==0&&w2==0){rele_on_msg();} 
else if(w1==1 && w2==0){rele_off_msg();} 
break;
	default:
PR_DEBUG("REGULATOR-UNKOWN");
	break;
}}
else 
{/*PR_DEBUG("FALLLLLLLLLLLLL");*/}

}




