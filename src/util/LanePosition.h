/*
 * LanePosition.h
 *
 *  Created on: 04.12.2014
 *      Author: bialon
 */

#ifndef LANEPOSITION_H_
#define LANEPOSITION_H_

#include <string>
#include <math.h>

typedef struct {
		std::string vehicleId;
		std::string laneId;
		double_t distance;
} lanePos_t;

#endif /* LANEPOSITION_H_ */
