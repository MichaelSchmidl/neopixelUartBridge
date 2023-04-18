/*
 * cmdline.h
 *
 *  Created on: Aug 3, 2021
 *      Author: mkail
 */

#ifndef INC_CMDLINE_H_
#define INC_CMDLINE_H_

#include <stdint.h>

void cmdlineInit(void);
void cmdlineSendPrompt(void);
void cmdlineWorker(void);


void cmdlineRxCallback(uint8_t receivedByte);

void cmdlinePutc(char c);
void cmdlinePuts(char *szString);

#endif /* INC_CMDLINE_H_ */
