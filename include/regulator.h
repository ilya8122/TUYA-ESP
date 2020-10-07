#include "tuya_smart_api.h"
#include "define.h"

#include "tuya_msg.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>




 void rele_on(LED_HANDLE Rele);
 void rele_off(LED_HANDLE Rele);

 void rele_on_msg(LED_HANDLE Rele);
 void rele_off_msg(LED_HANDLE Rele);


 void check_protecki(bool def_protechki,int ISTOCHNIK,bool w1,bool w2);
void check_REGULATOR(int REGULATOR,int temp,int temp_min,int temp_max,bool def_protechki,int ISTOCHNIK,int w1,int w2,int LOW_LEVEL,int HIGH_LEVEL,LED_HANDLE Rele);
