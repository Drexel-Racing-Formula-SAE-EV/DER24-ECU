/*
 * ams.h
 *
 *  Created on: Mar 28, 2024
 *      Author: cole
 */

#ifndef __AMS_H_
#define __AMS_H_

#include <stdint.h>

#define NSEGS 5
#define NFANS 10
#define NVOLTS 14
#define NTEMPS 17

typedef struct
{
	uint16_t header;
	uint16_t *d0;
	uint16_t *d1;
	uint16_t *d2;
} ams_data_packet_t;

typedef struct
{
	uint16_t volts[NVOLTS];
	uint16_t temps[NTEMPS];
} segment_t;

typedef struct
{
	uint16_t air_status;
	uint16_t imd_status;
	uint16_t current;
	uint16_t max_temp;
	uint16_t min_volt;
	uint16_t max_volt;
	segment_t segs[NSEGS];
	uint16_t fans[NFANS];
} ams_t;

void segment_init(segment_t *dev);
void ams_init(ams_t *dev);


#endif /* __AMS_H_ */
