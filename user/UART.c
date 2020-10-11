
#include "device.h"
#include "tuya_smart_api.h"
#include "wf_sdk_adpt.h"

#include "HEX_TO_DEC.h"
#include "tuya_msg.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "define.h"

float I, P, P_calc, ENERGY;
int U;

struct struct_type //основная структура
{
  int U;
  float I;
  float P_calc;
  float ENERGY;
} Energy_struct;

void read_uart() {
  int i, i1;
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
        for (i1 = 0; i1 <= 23; i1++) {//переписчываем из общего буфера корректный пакет
          current_buf[i1] = buf[i + i1];
          //			PR_DEBUG("current_buf[%d]     %x
          //%d",i1,current_buf[i1],current_buf[i1]);
        }

        int u1 = get_value_3(current_buf[2], current_buf[3], current_buf[4]);//преобразуем 3 байта коффецента напряжения в 1 целочисленную переменную (а=1 и=7 г=4 результат=174)
        int u2 = get_value_3(current_buf[5], current_buf[6], current_buf[7]);//преобразуем 3 байта цикла напряжения
        // PR_DEBUG("U1 %d,%f",u1,u2);
        // PR_DEBUG("U2 %d,%f",u2,u2);
        //  PR_DEBUG("U %d",U);
        Energy_struct.U = u1 / u2;

        if (U != 0) {
          int I1 = get_value_3(current_buf[8], current_buf[9], current_buf[10]);//аналогично
          int I2 =
              get_value_3(current_buf[11], current_buf[12], current_buf[13]);//аналогично
          // PR_DEBUG("I1 %d,%f",I1,I1);
          // PR_DEBUG("I2 %d,%f",I2,I2);
          // PR_DEBUG("I %d",I);
          Energy_struct.I = (float)I1 / (float)I2;

          int P1 =
              get_value_3(current_buf[14], current_buf[15], current_buf[16]);//аналогично
          int P2 =
              get_value_3(current_buf[17], current_buf[18], current_buf[19]);//аналогично
          // PR_DEBUG("P1 %d,%f",P1,P1);
          // PR_DEBUG("P2 %d,%f",P2,P2);
          // PR_DEBUG("P %d",P);
          Energy_struct.P_calc = (float)U * I;
        }
        float p_buf = P_calc / 1000 / 3600 / 1000 / ENERGY_TIMER_PERIOD;//так как считываем 4 раза в секунду
        Energy_struct.ENERGY = Energy_struct.ENERGY + p_buf;
        return;
      }
    }
  }
}

void t_msg_energy() {//отправка в облако
  tuya_msg(Energy, (int)(Energy_struct.ENERGY * 1000), 3);
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

