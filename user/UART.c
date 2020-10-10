
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

struct struct_type {
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

  Energy_struct.U = 0;
  Energy_struct.I = 0;
  Energy_struct.P_calc = 0;
  a = user_uart_read_data(buf, 50);

  if (a != 0) {
    PR_DEBUG("Uart READ OK");
    PR_DEBUG("len %d", a);

    for (i = 0; i < 50; i++) {
      int l = 50 - i;
      if (buf[i] == 0x55 && l >= 24 && buf[i + 1] == 0x5a) {
        PR_DEBUG("Uart current_buf FOUNDED");
        for (i1 = 0; i1 <= 23; i1++) {
          current_buf[i1] = buf[i + i1];
          //			PR_DEBUG("current_buf[%d]     %x
          //%d",i1,current_buf[i1],current_buf[i1]);
        }

        int u1 = get_value_3(current_buf[2], current_buf[3], current_buf[4]);
        int u2 = get_value_3(current_buf[5], current_buf[6], current_buf[7]);
        // PR_DEBUG("U1 %d,%f",u1,u2);
        // PR_DEBUG("U2 %d,%f",u2,u2);
        //  PR_DEBUG("U %d",U);
        Energy_struct.U = u1 / u2;

        if (U != 0) {
          int I1 = get_value_3(current_buf[8], current_buf[9], current_buf[10]);
          int I2 =
              get_value_3(current_buf[11], current_buf[12], current_buf[13]);
          // PR_DEBUG("I1 %d,%f",I1,I1);
          // PR_DEBUG("I2 %d,%f",I2,I2);
          // PR_DEBUG("I %d",I);
          Energy_struct.I = (float)I1 / (float)I2;

          int P1 =
              get_value_3(current_buf[14], current_buf[15], current_buf[16]);
          int P2 =
              get_value_3(current_buf[17], current_buf[18], current_buf[19]);
          // PR_DEBUG("P1 %d,%f",P1,P1);
          // PR_DEBUG("P2 %d,%f",P2,P2);
          // PR_DEBUG("P %d",P);
          Energy_struct.P_calc = (float)U * I;
        }
        float p_buf = P_calc / 1000 / 3600 / 1000 / ENERGY_TIMER_PERIOD;
        Energy_struct.ENERGY = Energy_struct.ENERGY + p_buf;
        return;
      }
    }
  }
}

void t_msg_energy() {
  tuya_msg(17, (int)(Energy_struct.ENERGY * 1000), 3);
  tuya_msg(18, (int)(Energy_struct.I * 1000), 3);
  tuya_msg(19, (int)(Energy_struct.P_calc * 10), 3);
  tuya_msg(20, (int)(Energy_struct.U * 10), 3);
}

void def_I_U(int U_MAX, int U_MIN, int I_MAX, LED_HANDLE Rele) {
  if (Energy_struct.U != 0) {
    if (Energy_struct.U >= U_MAX || Energy_struct.U <= U_MIN) {
      rele_off_msg(Rele);
    } else if (Energy_struct.I >= I_MAX) {
      rele_off_msg(Rele);
    }
  }
}

void energy_to_null() { Energy_struct.ENERGY = 0; }

