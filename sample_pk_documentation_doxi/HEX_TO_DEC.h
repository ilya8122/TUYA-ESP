#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include "define.h"

	/**
	@brief 	combines 3 int variables into 1 example: arg1 = 0x5a, int arg2 = 0x6b, int ARG = 0x3 result=0x5a6b3
	@param arg1 parameter 1
	@param arg2 parameter 2 
	@param arg3 parameter 3
	@return result of merging 3 variables
	*/
	int get_value_3(int arg1, int arg2, int arg3);
	/**
	@brief combines 2 int variables into 1 example: arg1 = 0x5a, int arg2 = 0x6b result=0x5a6b
	@param arg1 parameter 1
	@param arg2 parameter 2 
	@return result of merging 2 variables
	*/
	int get_value_2(int arg1, int arg2);
	/**
	@brief converts an int to a string the itoa Function converts an integer value to a string in radix notation
	@param number  the number to be converted
	@param base  the base of the number degree 
	@param destination the string to which the number is converted
	@return the itoa Function always returns a pointer to string
	*/
	char* itoa(int number, char* destination, int base);
	/**
	@brief exponentiation
	@param number number
	@param pow power value
	@return calculation result
	*/
	double pow(double number, double pow);
