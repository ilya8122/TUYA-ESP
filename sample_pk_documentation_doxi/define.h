#define SERIAL_Resistor2 370 /*!< Series-connected resistor in the ADC voltage divider*/
#define core_U 3.3 /*!< The supply voltage of the module TYWE3S*/

#define WF_RESET_KEY GPIO_ID_PIN(0) /*!< pairing button*/
#define WF_DIR_LED   GPIO_ID_PIN(5) /*!< pairing led indicator*/
#define RELE_PIN     GPIO_ID_PIN(4) /*!< rele pin*/

#define W1_io12 GPIO_ID_PIN(12) /*!< leak sensor 1*/
#define W2_io13 GPIO_ID_PIN(13) /*!< leak sensor 2*/

#define ENERGY_TIMER_PERIOD 250 /*!< frequency of operation of the electricity counting timer*/
#define REGULATOR_TIMER_PERIOD 1000 /*!< frequency of operation of the controller check timer*/

                
#define  Off 		0
#define  Heater 	1
#define  Conditioner 	2
#define  Inrange 	3
#define  OutRange 	4
#define  HeaterW 	5 
#define  ConditionerW 	6
#define  InrangeW 	7
#define  OutRangeW 	8
#define  Filling 	9
#define  Drain 		10


#define  Switch		1
#define  Reset_energy 	2
#define  Energy 	17
#define  Current 	18
#define  Power	 	19
#define  Voltage 	20
#define  Fault	 	26
#define  Regulator	101
#define  Water1 	103
#define  Water2 	104
#define  Temperature	106  
#define  TLow 		107  
#define  THigh 		108  
#define  Source_water	109  
#define  Water_def	110  
#define  WHigh 		111 
#define  WLow 		112  
#define  Imax 		113  
#define  Umax 		114  
#define  UMin 		115

//ISTOCHNIK
/*#define  w1		0
#define  w2		1
#define  w_or		2
#define  w_and		3*/
