#include <string.h>
#include "main.h"
#include "neopixel.h"

extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_tim3_ch1;

#define H_VAL 26
#define L_VAL 14
#define N_LEDS 5
#define BITS_PER_LED (3*8)
#define BIT_BUF_SIZE (N_LEDS * BITS_PER_LED)
uint16_t ws2812BitBuf[BIT_BUF_SIZE + 1]; //DMA transfer needs one byte more with ZERO, because this last value will be output after DMA transfer has finished!

void neopixelSetColor(int led, uint8_t r, uint8_t g, uint8_t b)
{
  if (led >= N_LEDS) return;
  int i = led * BITS_PER_LED;
  uint8_t mask;
  mask = 0x80;
  while(mask) {
    ws2812BitBuf[i] = (mask & g)?H_VAL:L_VAL;
    mask >>= 1;
    i++;
  }
  mask = 0x80;
  while(mask) {
    ws2812BitBuf[i] = (mask & r)?H_VAL:L_VAL;
    mask >>= 1;
    i++;
  }
  mask = 0x80;
  while(mask) {
    ws2812BitBuf[i] = (mask & b)?H_VAL:L_VAL;
    mask >>= 1;
    i++;
  }
}


int neopixelGetNumberOfLeds( void )
{
   return N_LEDS;
}


void neopixelWheel( void )
{
   static uint8_t r = 0;
   static uint8_t g = 0;
   static uint8_t b = 0;
   static uint8_t state = 0;
   for (int led = 0; led < neopixelGetNumberOfLeds(); led++)
   {
      switch (state) {
        case 0:
          r++;
          if (g) g--;
          b = 0;
          if (r == 255) {
            state++;
          }
          break;
        case 1:
          if (r) r--;
          g = 0;
          b++;
          if (b == 255) {
            state++;
          }
          break;
        case 2:
          r = 0;
          g++;
          if (b) b--;
          if (g == 255) {
            state++;
          }
          break;
        default:
          state = 0;
          break;
      }
      neopixelSetColor(led, r, g, b);
   }
   neopixelRefresh();
}


void neopixelRefresh( void )
{
//    HAL_GPIO_TogglePin(Led_GPIO_Port, Led_Pin);
    HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_1);
    if (HAL_LOCKED == hdma_tim3_ch1.Lock)
    {
       __HAL_UNLOCK(&hdma_tim3_ch1); // ist scheinbar nötig - beim STM32L053 war es nicht notwendig
    }
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)ws2812BitBuf, sizeof(ws2812BitBuf) / sizeof(ws2812BitBuf[0]));
}


void neopixelInit( void )
{
   // clear buffer
   memset(ws2812BitBuf, 0, sizeof(ws2812BitBuf));
   // turn off all LEDs
   for ( int i = 0; i < neopixelGetNumberOfLeds(); i++ )
   {
      neopixelSetColor( i, 0, 0, 0);
   }
   neopixelRefresh();
}


void neopixelWorker( void )
{
   static uint32_t cnt = 0;
   cnt++;
   if ( cnt >= 10 )
   {
      cnt = 0;
      neopixelRefresh();
   }
}
