/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */
/**
 * Modified for VSimRTI-Tbus by Raphael Bialon <Raphael.Bialon@hhu.de>
 */

#include "Bbdelay.h"

namespace projekt {

/**
 * default constructor
 * seqnumber and stamp equals -1
 */
Bbdelay::Bbdelay() {
}

/**
 * default destructor
 */
Bbdelay::~Bbdelay() {
}

/**
 * Returns a new Bbdelay, pointed to current values
 * @return Bbdelay*
 */
Bbdelay* Bbdelay::copy() {
	return new Bbdelay(*this);
}

/**
 * Get current backbone delay
 * @return simtime_t
 */
simtime_t Bbdelay::bbdelay() {
	return bb;
}

std::ostream& Bbdelay::operator<<(std::ostream& os) {
	return os;
}

/**
 * Returns all important informations as string
 * @param s
 * @return std::string
 */
//std::string Bbdelay::toString(std::string s) {
//	char buff[100];
//	sprintf(buff, "%s%.6f%s%s%s%s%s", "(", bb.dbl(), ",", (validbb) ? "true" : "false", ",", modem.c_str(), ")");
//	s = buff;
//	return s;
//}

/**
 *
 * @return
 */
double Bbdelay::droprate(){
	return dropValue;
}

/**
 * Set current backbone delay
 * @param delay
 */
void Bbdelay::setBbdelay(simtime_t delay) {
	bb = delay;
}

/**
 *
 * @param rate
 */
void Bbdelay::setDroprate(double rate){
	dropValue = rate;
}

} /* namespace projekt */
