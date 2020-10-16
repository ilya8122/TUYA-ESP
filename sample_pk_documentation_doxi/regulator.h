#include "tuya_smart_api.h"
#include "define.h"

#include "tuya_msg.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/**
@brief enabling the relay
@param Rele relay handler
*/
void rele_on(LED_HANDLE Rele);
/**
@brief relay shutdown
@param Rele relay handler
*/
void rele_off(LED_HANDLE Rele);
/**
@brief enabling the relay and sending information about it to the cloud
@param Rele relay handler
*/
void rele_on_msg(LED_HANDLE Rele);
/**
@brief turning off the relay and sending information about it to the cloud
@param Rele relay handler
*/
void rele_off_msg(LED_HANDLE Rele);
/**
@brief checking for a leak in the rear source and turning off the relay if necessary
@param def_protechki leak protection status
@param ISTOCHNIK source of the leak
@param w1 the leak sensor 1
@param w2 the leak sensor 2

*/
void check_protecki(bool def_protechki, int ISTOCHNIK, bool w1, bool w2);
/**
@brief This method hides the operation of all controller modes
@param REGULATOR number of the regulator
@param temp the current temperature
@param temp_min temperature
@param temp_max max temperature
@param def_protechki protection status on the leak
@param ISTOCHNIK source of the leak
@param w1 the leak sensor 1
@param w2 the leak sensor 2
@param LOW_LEVEL lower level of leakage
@param HIGH_LEVEL upper leak LEVEL
@param Rele relay handler
*/

void check_REGULATOR(int REGULATOR, int temp, int temp_min, int temp_max,
                     bool def_protechki, int ISTOCHNIK, int w1, int w2,
                     int LOW_LEVEL, int HIGH_LEVEL, LED_HANDLE Rele);
