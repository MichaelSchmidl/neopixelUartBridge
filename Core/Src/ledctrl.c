#include <stdint.h>
#include <string.h>
#include "ledctrl.h"
#include "neopixel.h"

typedef struct
{
   uint8_t  r;
   uint8_t  g;
   uint8_t  b;
   uint16_t ontime_ms;
   uint16_t offtime_ms;
   uint32_t cnt_ms;
}ledctrlProperty_t;

#define N_LEDCTRLS 5
static ledctrlProperty_t _led[N_LEDCTRLS];


void ledctrlSetColor(int led, uint8_t r, uint8_t g, uint8_t b)
{
   if (led > N_LEDCTRLS) return;
   _led[led].r = r;
   _led[led].g = g;
   _led[led].b = b;
}


void ledctrlSetBlink(int led, int onTime_ms, int offTime_ms )
{
   if (led > N_LEDCTRLS) return;
   _led[led].ontime_ms = onTime_ms;
   _led[led].offtime_ms = offTime_ms;
}


void ledctrlInit( void )
{
   memset(_led, 0, sizeof(_led));
}


void ledctrlWorker( void )
{
   for (int i = 0; i < N_LEDCTRLS; i++)
   {
      if ( 0 != _led[i].ontime_ms )
      {
         _led[i].cnt_ms++;
         if ( _led[i].cnt_ms >= (_led[i].ontime_ms + _led[i].offtime_ms) )
         {
            _led[i].cnt_ms = 0;
         }
         if (_led[i].cnt_ms < _led[i].ontime_ms)
         {
            neopixelSetColor(i, _led[i].r, _led[i].g, _led[i].b);
         }
         else if ( _led[i].cnt_ms < (_led[i].ontime_ms + _led[i].offtime_ms))
         {
            neopixelSetColor(i, 0, 0, 0);
         }
      }
   }
}
