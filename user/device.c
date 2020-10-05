/***********************************************************
*  File: device.c 
*  Author: ILYA
*  Date: 20150605
***********************************************************/
#define __DEVICE_GLOBALS
#include "device.h"
#include "tuya_smart_api.h"
#include "wf_sdk_adpt.h"

#include "adclib.h"
#include "HEX_TO_DEC.h"
#include "RES_TO_TEMP.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define B 3950 // B-коэффициент
#define SERIAL_R 370 // сопротивление последовательного резистора, 370 Ом
#define THERMISTOR_R 10000 // номинальное сопротивления термистора, 10 кОм
#define NOMINAL_T 25 // номинальная температура (при которой TR = 10 кОм)

#define WF_RESET_KEY GPIO_ID_PIN(0)
#define WF_DIR_LED   GPIO_ID_PIN(5)
#define RELE_PIN   GPIO_ID_PIN(4)

#define W1_io12 GPIO_ID_PIN(12)
#define W2_io13 GPIO_ID_PIN(13)



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
int REGULATOR=0;
int ISTOCHNIK,LOW_LEVEL,HIGH_LEVEL,temp_min,temp_max;
bool def_protechki, RELE_stat;
bool first_iterator=true;
int U,U_MIN,U_MAX,I_MAX;
float I,P_calc,P,ENERGY;

   

/*
int REGULATOR=0;
int ISTOCHNIK=0;
int LOW_LEVEL=0;
int HIGH_LEVEL=0;
int temp_min=0;
int temp_max=0;
bool def_protechki=false;
*/


//REGULATOR                  
//0 Off
//1 Heater
//2 Conditioner
//3 Inrange
//4 OutRange
//5 HeaterW
//6 ConditionerW
//7 InrangeW
//8 OutRangeW
//9 Filling
//10 Drain
//ISTOCHNIK
//0 w1
//1 w2

//3 w_or
//4 w_and
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
    set_console(TRUE);//СЋР°СЂС‚0
    app_cfg_set(WCM_LOW_POWER,prod_test);
}

STATIC VOID tuya_msg(UINT dpid,UINT num_int,UINT type,float num_float)//send msg to tuya func
{
   PR_DEBUG("tuya_msg");

   GW_WIFI_STAT_E wf_stat = tuya_get_wf_status();
    if((wf_stat < STAT_STA_CONN) || (tuya_get_gw_status() != STAT_WORK)){PR_DEBUG("return WM_FAIL");return;}
    cJSON *root = cJSON_CreateObject();
    if(NULL == root) {PR_DEBUG("return WM_FAIL");return;} 
switch(dpid)
{
case 1:
if (type=1){   cJSON_AddFalseToObject(root,"1");	}
if (type=2){   cJSON_AddTrueToObject(root,"1");		}
break;

case 17:
if (type=3)
{  cJSON_AddNumberToObject(root,"17",num_int);}  //Energy
break;

case 18:
if (type=3)
{  cJSON_AddNumberToObject(root,"18",num_int);}  //I
break;

case 19:
if (type=3)
{ cJSON_AddNumberToObject(root,"19",num_int);}  //p
break;

case 20:
if (type=3)
{ cJSON_AddNumberToObject(root,"20",num_int);	}  //U
break;

case 103:
if (type=3){   cJSON_AddNumberToObject(root,"103",num_int);	}  //w1
break;

case 104:
if (type=3){   cJSON_AddNumberToObject(root,"104",num_int);	}  //w2
break;

case 106:
if (type=3){
 //char buf[16];sprintf(buf,"%d",num_int);const char* p = buf; cJSON_AddStringToObject(root,"106",buf);
 cJSON_AddNumberToObject(root,"106",num_int);}  //ADC
break;

case 110:
if (type=1){   cJSON_AddFalseToObject(root,"110");	}
if (type=2){   cJSON_AddTrueToObject(root,"110");	}
break;

default:
PR_DEBUG("NO DPID");
break;
}
    CHAR *out = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    if(NULL == out)
    {
        PR_ERR("cJSON_PrintUnformatted err:");
        PR_DEBUG("return WM_FAIL");return;
    }    
    PR_DEBUG("out:  %s ", out);
    OPERATE_RET op_ret = tuya_obj_dp_report(out);
    Free(out);

        if( OPRT_OK == op_ret ) 	{      PR_DEBUG("return suc");return;  	}
	else	{        PR_DEBUG("return WM_FAIL");return;   	} 
}


STATIC VOID rele_on(){tuya_set_led_type(Rele,OL_HIGH,0);RELE_stat=true; PR_DEBUG("rele_on");}
STATIC VOID rele_off(){tuya_set_led_type(Rele,OL_LOW,0);RELE_stat=false; PR_DEBUG("rele_off");}

STATIC VOID rele_on_msg(){tuya_set_led_type(Rele,OL_HIGH,0);tuya_msg(1,0,2,0);RELE_stat=true; PR_DEBUG("rele_on_msg");}
STATIC VOID rele_off_msg(){tuya_set_led_type(Rele,OL_LOW,0);tuya_msg(1,0,1,0);RELE_stat=false; PR_DEBUG("rele_off_msg");}

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
/////////////////////////////////////////////////
    adc_config_t adc_config;
    adc_config.mode = ADC_READ_TOUT_MODE;
    adc_config.clk_div = 8; 
    adc_init(&adc_config);
    uint16_t adc_data[100];
    adc_read(&adc_data[0]);
////////////////////////////////////////////////// 

PR_DEBUG("ADC:%d", adc_data[0]);
int t = adc_data[0];

Free(&adc_config);

float tr = 1023.0 / t - 1;
    tr = SERIAL_R / tr*240;//240- подбором в зависимости от текущей температуры
  
   int temp =get_temp(tr);

return temp ;
}




STATIC VOID read_uart()
{
int i,i1;
uint16 a;
uint8 buf[50]={0};
uint8 current_buf[24]={0};
	U=0;I=0; P_calc=0;
a=user_uart_read_data(buf,50);

if (a!=0)
{
PR_DEBUG("Uart READ OK");
PR_DEBUG("len %d",a);

for(i=0;i<50;i++)
{

	int l = 50-i;
	if (buf[i]==0x55 && l >=24 )
		{
PR_DEBUG("Uart current_buf FOUNDED");
			for(i1=0;i1<=23;i1++)
			{
			current_buf[i1]=buf[i+i1];
//			PR_DEBUG("current_buf[%d]     %x       %d",i1,current_buf[i1],current_buf[i1]);
			}

			int u1=get_value_3(current_buf[2],current_buf[3],current_buf[4]);
			int u2=get_value_3(current_buf[5],current_buf[6],current_buf[7]);
			U =u1/u2;	
			
//PR_DEBUG("U1 %d,%f",u1,u2);
//PR_DEBUG("U2 %d,%f",u2,u2);	
if(U!=0){
                      //  PR_DEBUG("U %d",U);

			int I1 = get_value_3(current_buf[8],current_buf[9],current_buf[10]);
			int I2 = get_value_3(current_buf[11],current_buf[12],current_buf[13]);
			 I =	(float)I1/(float)I2;  
			     
//PR_DEBUG("I1 %d,%f",I1,I1);
//PR_DEBUG("I2 %d,%f",I2,I2);       
         	//	PR_DEBUG("I %d",I);

			 P_calc= (float)U*I;
			 
//PR_DEBUG("U %d,%f",U,U);
//PR_DEBUG("I %d,%f",I,I);   
          //		PR_DEBUG("P_calc %d",P_calc,P_calc);

			int P1 = get_value_3(current_buf[14],current_buf[15],current_buf[16]);
			int P2 = get_value_3(current_buf[17],current_buf[18],current_buf[19]);
			 P =(float)P1/(float)P2	;
			
//PR_DEBUG("P1 %d,%f",P1,P1);
//PR_DEBUG("P2 %d,%f",P2,P2);   
 //         		PR_DEBUG("P %f %d",P,P);

			float f= ((I*U)/(2.43*2.43))*(3.579/128);
			ENERGY=(get_value_2(current_buf[21],current_buf[22]))/(f*1000*3600);
			break;
		}
}  
}
}
	
}
STATIC VOID uart_timer_cb(UINT timerID,PVOID pTimerArg)// my timer handle
{
if( tuya_get_gw_status() == STAT_WORK&& tuya_get_wf_status()==STAT_STA_CONN) 
{  
PR_DEBUG("UART TICK\n");
read_uart();
tuya_msg(17,(int)(ENERGY*1000),3,0);
tuya_msg(18,(int)(I*1000),3,0);
tuya_msg(19,(int)(P_calc*10),3,0);
tuya_msg(20,(int)(U*10),3,0);
}
}









STATIC VOID heart_bit_timer_cb(UINT timerID,PVOID pTimerArg)// my timer handle
{


//if(RELE_stat){rele_on();}else if(!RELE_stat){rele_off();}

if( tuya_get_gw_status() == STAT_WORK&& tuya_get_wf_status()==STAT_STA_CONN) 
{  
PR_DEBUG("TICK\n");


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


uint16_t W_value,W_value1;
bool flag;


if (first_iterator) 
{
//PR_DEBUG("Defolt value send-nothing");
first_iterator=false; 
}

////////////////////////////////////////////////////////////////////
uint16_t temp = read_adc_value();
 tuya_msg(106,temp*10,3,0);

uint16_t i2 = tuya_read_gpio_level(W1_io12);
 tuya_msg(103,i2,3,0);

uint16_t i3 = tuya_read_gpio_level(W2_io13);
 tuya_msg(104,i3,3,0);


///////////////////////////////////////////////////////////////////


if (def_protechki==true)
{
switch(ISTOCHNIK)
	{
	case 0:
W_value = tuya_read_gpio_level(W1_io12);
if(W_value==0){flag=true;}else {flag=false;}
	break;

	case 1:
W_value = tuya_read_gpio_level(W2_io13);
if(W_value==0){flag=true;}else {flag=false;}
	break;

	case 2:
W_value =tuya_read_gpio_level(W1_io12);
W_value1=tuya_read_gpio_level(W2_io13);
if (W_value==0||W_value1==0){flag=true;}else {flag=false;}
	break;

	case 3:
W_value =tuya_read_gpio_level(W1_io12);
W_value1=tuya_read_gpio_level(W2_io13);
if (W_value==0) {if (W_value1==0){flag=true;}}else {flag=false;}
	break;
	default:
PR_DEBUG("ISTOCHNIK-UNKOWN");
	break;
}
if (flag==true){rele_off_msg();}
}


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
tuya_msg(110,0,1,0);
}
	switch(ISTOCHNIK)
	{
	case 0:
W_value = tuya_read_gpio_level(W1_io12);
if(W_value==0){flag=true;}else {flag=false;}
	break;

	case 1:
W_value = tuya_read_gpio_level(W2_io13);
if(W_value==0){flag=true;}else {flag=false;}
	break;

	case 2:
W_value =tuya_read_gpio_level(W1_io12);
W_value1=tuya_read_gpio_level(W2_io13);
if (W_value==0||W_value1==0){flag=true;}else {flag=false;}
	break;

	case 3:
W_value =tuya_read_gpio_level(W1_io12);
W_value1=tuya_read_gpio_level(W2_io13);
if (W_value==0&&W_value1==0){flag=true;}else {flag=false;}
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
tuya_msg(110,0,1,0);
}
	switch(ISTOCHNIK)
	{
	case 0:
W_value = tuya_read_gpio_level(W1_io12);
if(W_value==0){flag=true;}else {flag=false;}
	break;

	case 1:
W_value = tuya_read_gpio_level(W2_io13);
if(W_value==0){flag=true;}else {flag=false;}
	break;

	case 2:
W_value =tuya_read_gpio_level(W1_io12);
W_value1=tuya_read_gpio_level(W2_io13);
if (W_value==0||W_value1==0){flag=true;}else {flag=false;}
	break;

	case 3:
W_value =tuya_read_gpio_level(W1_io12);
W_value1=tuya_read_gpio_level(W2_io13);
if (W_value==0&&W_value1==0){flag=true;}else {flag=false;}
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
tuya_msg(110,0,1,0);
}
	switch(ISTOCHNIK)
	{
	case 0:
W_value = tuya_read_gpio_level(W1_io12);
if(W_value==0){flag=true;}else {flag=false;}
	break;

	case 1:
W_value = tuya_read_gpio_level(W2_io13);
if(W_value==0){flag=true;}else {flag=false;}
	break;

	case 2:
W_value =tuya_read_gpio_level(W1_io12);
W_value1=tuya_read_gpio_level(W2_io13);
if (W_value==0||W_value1==0){flag=true;}else {flag=false;}
	break;

	case 3:
W_value =tuya_read_gpio_level(W1_io12);
W_value1=tuya_read_gpio_level(W2_io13);
if (W_value==0&&W_value1==0){flag=true;}else {flag=false;}
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
tuya_msg(110,0,1,0);
}
	switch(ISTOCHNIK)
	{
	case 0:
W_value = tuya_read_gpio_level(W1_io12);
if(W_value==0){flag=true;}else {flag=false;}
	break;

	case 1:
W_value = tuya_read_gpio_level(W2_io13);
if(W_value==0){flag=true;}else {flag=false;}
	break;

	case 2:
W_value =tuya_read_gpio_level(W1_io12);
W_value1=tuya_read_gpio_level(W2_io13);
if (W_value==0||W_value1==0){flag=true;}else {flag=false;}
	break;

	case 3:
W_value =tuya_read_gpio_level(W1_io12);
W_value1=tuya_read_gpio_level(W2_io13);
if (W_value==0&&W_value1==0){flag=true;}else {flag=false;}
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
tuya_msg(110,0,1,0);
}

switch(LOW_LEVEL)
{
case 0:
W_value =tuya_read_gpio_level(W1_io12);
break;

case 1:
W_value =tuya_read_gpio_level(W2_io13);
break;
}
switch(HIGH_LEVEL)
{
case 0:
W_value1=tuya_read_gpio_level(W1_io12);
break;

case 1:
W_value1=tuya_read_gpio_level(W2_io13);
break;
}

if(W_value1==1&&W_value==0){rele_on_msg();} 
else if(W_value1==0&&W_value==0){rele_off_msg();} 
else{}
break;




case 10://10 Drain
if (def_protechki==true)
{
def_protechki=false; 
tuya_msg(110,0,1,0);
}
switch(LOW_LEVEL)
{
case 0:
W_value =tuya_read_gpio_level(W1_io12);
break;

case 1:
W_value =tuya_read_gpio_level(W2_io13);
break;
}
switch(HIGH_LEVEL)
{
case 0:
W_value1=tuya_read_gpio_level(W1_io12);
break;

case 1:
W_value1=tuya_read_gpio_level(W2_io13);
break;
}
if(W_value1==0&&W_value==0){rele_on_msg();} 
else if(W_value1==1 && W_value==0){rele_off_msg();} 
break;
	default:
PR_DEBUG("REGULATOR-UNKOWN");
	break;
}}
else 
{/*PR_DEBUG("FALLLLLLLLLLLLL");*/}

}
/*


int i4 = tuya_read_gpio_level(W3_io14);

 tuya_msg(105,i4,3);
*/



