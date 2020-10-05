#define SERIAL_R 370 // сопротивление последовательного резистора, 370 Ом
#define THERMISTOR_R 10000 // номинальное сопротивления термистора, 10 кОм
#define NOMINAL_T 25 // номинальная температура (при которой TR = 10 кОм)

#define WF_RESET_KEY GPIO_ID_PIN(0)
#define WF_DIR_LED   GPIO_ID_PIN(5)
#define RELE_PIN     GPIO_ID_PIN(4)

#define W1_io12 GPIO_ID_PIN(12)
#define W2_io13 GPIO_ID_PIN(13)

//REGULATOR                  
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

//ISTOCHNIK
#define  w1		0
#define  w2		1
#define  w_or		2
#define  w_and		3
