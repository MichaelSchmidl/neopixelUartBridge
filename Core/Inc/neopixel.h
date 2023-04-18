
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NEOPIXEL_H
#define __NEOPIXEL_H

#ifdef __cplusplus
extern "C" {
#endif

void neopixelSetColor(int led, uint8_t r, uint8_t g, uint8_t b);
int neopixelGetNumberOfLeds( void );
void neopixelInit( void );
void neopixelWorker( void );
void neopixelRefresh( void );
void neopixelWheel( void );

#ifdef __cplusplus
}
#endif

#endif /* __NEOPIXEL_H */
