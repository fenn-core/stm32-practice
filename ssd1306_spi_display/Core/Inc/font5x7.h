/*
 * font5x7.h
 *
 *  Created on: Apr 25, 2026
 *      Author: pusana
 */

#ifndef INC_FONT5X7_H_
#define INC_FONT5X7_H_

#include <stdint.h>

#define FONT5X7_WIDTH  5
#define FONT5X7_HEIGHT 7
#define FONT5X7_FIRST_CHAR ' '
#define FONT5X7_LAST_CHAR '~'

extern const uint8_t font5x7[][FONT5X7_WIDTH];

#endif /* INC_FONT5X7_H_ */
