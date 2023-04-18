
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LEDCTRL_H
#define __LEDCTRL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void ledctrlInit(void);
void ledctrlWorker(void);

void ledctrlSetColor(int led, uint8_t r, uint8_t g, uint8_t b);
void ledctrlSetBlink(int led, int onTime_ms, int offTime_ms );

#ifdef __cplusplus
}
#endif

#endif /* __LEDCTRL_H */
