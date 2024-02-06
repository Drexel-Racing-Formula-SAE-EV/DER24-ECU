/**
* @file pressTrans.c
* @author Cole Bardin (cab572@drexel.edu) & Benedict Hofmockel
* @brief
* @version 0.1
* @date 2023-04-24
*
* @copyright Copyright (c) 2023
*
*/

#include "ext_drivers/pressTrans.h"
#include "ext_drivers/map.h"
#include "math.h"


void pressTrans_init(struct pressTrans *pressTrans, uint16_t min, uint16_t max, void *handle, uint8_t channelNum, uint16_t(*read_count)(void *arg)) {
	pressTrans->min = min;
	pressTrans->max = max;
	pressTrans->handle = handle;
	pressTrans->channelNum = channelNum;
	pressTrans->read_count = read_count;
}

float presstransGetPercent(struct pressTrans *root) {
	float percent = (float)map(root->count, root->min, root->max, 0, 100);
	if(percent > 100.0){
		return 100.0;
	}else if(percent < 0.0){
		return 0.0;
	}else{
		return percent;
	}
}

uint8_t presstransCheckImplausability(float L, float R, int thresh, int count){
    static unsigned int counts = 0;

	// Check if APPS1 and APPS2 are more than 10% different
	if(fabs(L - R) > thresh){
		counts++;

		// If there are consecutive errors for more than 100ms, error
		return counts <= count;
	}else{
		// If potentiometers are within spec, reset count
		counts = 0;
		return 1;
	}
}



uint8_t switchChannelADC (struct pressTrans *root){
	//TODO: add code here from the prefeature APPS branch
	ADC_ChannelConfTypeDef sConfig = {0};
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	*/
	sConfig.Channel = root->channelNum;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(root->handle, &sConfig) != HAL_OK)
	{
//		Error_Handler(); //Ben currently has no clue what this does
		return HAL_ERROR;
	} else {
		return HAL_OK;
	}
}


