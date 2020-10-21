
#include "device.h"
#include "tuya_smart_api.h"
#include "wf_sdk_adpt.h"

#include "HEX_TO_DEC.h"
#include "tuya_msg.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "define.h"

struct struct_t
{     
        int  u1:24;
        int  u2:24;
        int  i1:24;
        int  i2:24;
}__attribute__((packed)) str;


struct struct_type //основная структура
{
  int U;
  float I;
  float P_calc;
  float ENERGY;
} Energy_struct;

void read_uart() {
  int i, i1,i2;
  uint16 a;
  uint8 buf[50] = {0};
  uint8 current_buf[24] = {0};
 
  Energy_struct.U = 0;//зануляем все значения
  Energy_struct.I = 0;
  Energy_struct.P_calc = 0;
  a = user_uart_read_data(buf, 50);//считываем значения с юарт и пихавем в буфер

  if (a != 0) {// если длина считанных данных не равна 0
    PR_DEBUG("Uart READ OK");
    PR_DEBUG("len %d", a);

    for (i = 0; i < 50; i++) {
      int l = 50 - i;
      if (buf[i] == 0x55 && l >= 24 && buf[i + 1] == 0x5a) {//если начало пакета найдено и пакет целый
        PR_DEBUG("Uart current_buf FOUNDED");
        for (i1 = 0; i1 <= 23; i1++)
	 {//переписчываем из общего буфера корректный пакет
          current_buf[i1] = buf[i + i1];
       	 }

uint8 U_I_buf[12]={current_buf[2], current_buf[3], current_buf[4],current_buf[5], current_buf[6], current_buf[7],current_buf[8], current_buf[9], current_buf[10],current_buf[11],current_buf[12],
current_buf[13]};//   

  unsigned char  buf[12];
    for(i2=0;i<sizeof(buf);i++)
    {
    int g=(sizeof(buf)-i-1);
    buf[i]=U_I_buf[g];
    }
    
struct struct_t *vat=(struct struct_t*)&buf;      

        Energy_struct.U = vat->u2/vat->u1;

        if (Energy_struct.U != 0) {
        
          Energy_struct.I = ((float)(vat->i2)/(float)(vat->i1));

        
          Energy_struct.P_calc = (float) Energy_struct.U * Energy_struct.I;

    float p_buf = Energy_struct.P_calc / 1000 / 3600 / (1000 / ENERGY_TIMER_PERIOD);//так как считываем 4 раза в секунду
   Energy_struct.ENERGY = Energy_struct.ENERGY + p_buf;
        }
    
     
        return;
      }
    }
  }
}

void t_msg_energy() {//отправка в облако
  tuya_msg(Energy, (int)(Energy_struct.ENERGY * 1000), 3);// 1000 for succes work
  tuya_msg(Current, (int)(Energy_struct.I * 1000), 3);
  tuya_msg(Power, (int)(Energy_struct.P_calc * 10), 3);
  tuya_msg(Voltage, (int)(Energy_struct.U * 10), 3);
}

void def_I_U(int U_MAX, int U_MIN, int I_MAX, LED_HANDLE Rele) {//защита 
  if (Energy_struct.U != 0) {
    if (Energy_struct.U >= U_MAX || Energy_struct.U <= U_MIN) {
      rele_off_msg(Rele);
    } else if (Energy_struct.I >= I_MAX) {
      rele_off_msg(Rele);
    }
  }
}

void energy_to_null() { Energy_struct.ENERGY = 0; }//сброс счётчика энергии

