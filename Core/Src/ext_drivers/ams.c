/*
 * ams.c
 *
 *  Created on: Mar 28, 2024
 *      Author: cole
 */

#include "ext_drivers/ams.h"

void segment_init(segment_t *dev)
{
	int i;
	for(i = 0; i < NVOLTS; i++) dev->volts[i] = 0;
	for(i = 0; i < NTEMPS; i++) dev->temps[i] = 0;
}

void ams_init(ams_t *dev)
{
	int i;
	dev->state = 0;
	dev->air_state = 0;
	dev->imd_ok = 0;
	dev->imd_status = 0;;
	dev->imd_duty = 0;
	dev->current = 0;
	dev->max_temp = 0;
	dev->min_volt = 0;
	dev->max_volt = 0;
	for(i = 0; i < NSEGS; i++) segment_init(&dev->segs[i]);
	for(i = 0; i < NFANS; i++) dev->fans[i] = 0;
}
