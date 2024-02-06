/**
* @file map.c 
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2023-03-13
*
* @copyright Copyright (c) 2023
*
*/

#include "ext_drivers/map.h"

// Parsed from Arduino's Wiring.h
long double map(long x, long in_min, long in_max, long out_min, long out_max) {
	long in_range = in_max - in_min;
	long out_range = out_max - out_min;
	if (in_range == 0) return out_min + (long double)out_range / 2;
	long double num = (x - in_min) * out_range;
	if (out_range >= 0) {
		num += (long double)in_range / 2;
	} else {
		num -= (long double)in_range / 2;
	}
	long double result = num / (long double)in_range + out_min;
	if (out_range >= 0) {
		if (in_range * num < 0) return result - 1;
	} else {
		if (in_range * num >= 0) return result + 1;
	}
	return result;
}
