/*
 * TbusQueueSelection.h
 *
 *  Created on: 23.02.2015
 *      Author: bialon
 */

#ifndef TBUSQUEUESELECTION_H_
#define TBUSQUEUESELECTION_H_

enum TbusQueueSelection {
	CDRQ = 1,
	CRRQ = 2,
	CDSQ = 4,
	CRSQ = 8,
	ALL  = 15
};

#endif /* TBUSQUEUESELECTION_H_ */
