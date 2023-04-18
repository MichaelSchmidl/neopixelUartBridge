#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "cmdline.h"
#include "main.h"
#include "neopixel.h"
#include "ledctrl.h"

#define VERSION_STRING "neoPixelUart.100_001"

#define MAX_ARGV_LEN 10U
#define MAX_ARGV_NUM 8U
static char argv[MAX_ARGV_NUM][MAX_ARGV_LEN+1U];

static uint8_t nCmdBuf = 0U;
static uint8_t cmdBuf[40];
static uint8_t bParseCmdLineNow = 0U;

extern UART_HandleTypeDef huart1;
#define hCmdLineUart huart1
static uint8_t uartCircularRxDMABuffer[2]; // DGB_UART


/*********************************************************************
    Function:       GetArgs
    Description:    Extracts arguments from command line
*********************************************************************/
static uint32_t GetArgs (char *s)
{
   uint32_t i;
   uint32_t j=0U;
   uint32_t iarg=0U;
   uint32_t string=0U;
   uint32_t leading=1U;
   uint32_t slen = strlen(s);
   for (i=0; i<slen; i++) {
      if (s[i] == ' ' || s[i] ==0) {
         if (!string && !leading) {
            argv[iarg++][j] = '\0';
            j=0U;
         } else if (string) {
            argv[iarg][j++] = s[i];
         }
      } else if (s[i] == '\"') {
         if (string) {
            argv[iarg++][j] = '\0';
            j=0U;
            leading = 1U;
            string=0U;
         } else {
            string = 1U;
         }
      } else {
         argv[iarg][j++] = s[i];
         leading = 0U;
      }
      if (MAX_ARGV_LEN <= j) {
         j = MAX_ARGV_LEN;
      }
      if (MAX_ARGV_NUM <= iarg) {
         iarg = MAX_ARGV_NUM;
      }
   }
   argv[iarg++][j] = '\0';
   return iarg;
}

/*********************************************************************
    Function:       GetArg
    Description:    Retrieves the given command line argument
*********************************************************************/
static char *GetArg( uint32_t iarg )
{
   return &argv[iarg][0];
}


void cmdlineShowUnknownCommand( uint32_t argc )
{
    uint32_t n = 0;
    cmdlinePuts( "\n[" );
    while ( argc-- )
    {
       cmdlinePuts( GetArg ( n++ ) );
        if ( argc )
        {
           cmdlinePuts( " " );
        }
    }
    cmdlinePuts( "] unsupported. Try the command \"help\".\n" );
}

static void ProcessLED( int argc )
{
   if ( 1 == argc )
   {
       cmdlineShowUnknownCommand( argc );
   }
   else if ( 2 == argc )
   {
       if ( !strncmp( "-h", GetArg( 1 ), 2 ) )
       {
          cmdlinePuts("led <nr> <r> <g> <b>      - set RGB value to given LED\n");
          cmdlinePuts("led all <r> <g> <b>       - set all LEDs with given RGB value\n");
          cmdlinePuts("led blink <nr> <on> <off> - set all LEDs with given RGB value\n");
       }
       else
       {
           cmdlineShowUnknownCommand(argc);
       }
   }
   else if ( 5 == argc )
   {
      if ( !strncmp( "all", GetArg(1), 3))
      {
         for ( int i = 0; i < neopixelGetNumberOfLeds(); i++ )
         {
            neopixelSetColor(i, atoi(GetArg(2)), atoi(GetArg(3)),atoi(GetArg(4)));
            ledctrlSetColor(i, atoi(GetArg(2)), atoi(GetArg(3)),atoi(GetArg(4)));
         }
      }
      else if ( !strncmp( "blink", GetArg(1), 5))
      {
         ledctrlSetBlink(atoi(GetArg(2)), atoi(GetArg(3)),atoi(GetArg(4)));
      }
      else
      {
         neopixelSetColor(atoi(GetArg(1)), atoi(GetArg(2)), atoi(GetArg(3)),atoi(GetArg(4)));
         ledctrlSetColor(atoi(GetArg(1)), atoi(GetArg(2)), atoi(GetArg(3)),atoi(GetArg(4)));
      }
      neopixelRefresh();
   }
   else
   {
       cmdlineShowUnknownCommand( argc );
   }
}


static void cmdlineProcess(void)
{
   char *p_cl = (char*)&cmdBuf[0];

   if ( p_cl && (0 < strlen(p_cl))) {
      uint32_t argc = GetArgs(p_cl);
      nCmdBuf = 0;
      cmdBuf[nCmdBuf] = '\0';
      if ( argc ) {
         char *cmd = GetArg(0);
         if (!strncmp("help",cmd,4)) {
            cmdlinePuts("led -h				- led commands\n");
         } // end HELP
         else if ( !strncmp("led", cmd, 3) ) {
            ProcessLED(argc);
         } // end LED
         else if ( !strncmp(".", cmd, 1) ) {
            neopixelWheel();
            cmdlinePuts("\n");
         } // end ?
         else if ( !strncmp("?", cmd, 1) ) {
            cmdlinePuts(VERSION_STRING);
            cmdlinePuts("\n");
         } // end ?
         else if ( !strncmp("!", cmd, 1) ) {
           cmdlinePuts("Compiled ");
            cmdlinePuts(__DATE__);
            cmdlinePuts(" ");
            cmdlinePuts(__TIME__);
            cmdlinePuts("\n");
         } // end !
         else {
            cmdlinePuts(cmd);
            cmdlinePuts(" not supported\n");
         } // end ELSE
      } // end if argc
   } // end if p_cl
   //nCmdBuf = 0;
   //cmdBuf[nCmdBuf] = '\0';
}


void cmdlineRxCallback(uint8_t receivedByte)
{
  if ('\r' == receivedByte) {
     bParseCmdLineNow = 1U;
     cmdlinePuts("\n");
  } else if ('\n' == receivedByte) {
     // ignore it
  } else if ('\b' == receivedByte) {
     if ( nCmdBuf > 0 ) {
        nCmdBuf--;
        cmdBuf[nCmdBuf] = 0U;
     }
  } else {
     cmdlinePutc(receivedByte);
     if (!bParseCmdLineNow) { // only store new bytes if parsing is not active currently
        if (nCmdBuf < (sizeof(cmdBuf) - 1U)) {
           cmdBuf[nCmdBuf++] = receivedByte;
           cmdBuf[nCmdBuf] = 0U;
        }
     }
  }
}


void cmdlinePutc(char c)
{
  HAL_UART_Transmit( &hCmdLineUart, (uint8_t*)&c, 1, 100000L);
}


void cmdlinePuts(char *szString)
{
  HAL_UART_Transmit( &hCmdLineUart, (uint8_t*)szString, strlen(szString), 100000L);
}


void cmdlineSendPrompt(void)
{
   cmdlinePuts(">");
}


static void handle_UART_RxCallbacks ( UART_HandleTypeDef* huart, uint8_t bufIndex )
{
    uint8_t recv;
    if ( huart == &hCmdLineUart )
    {
        recv = uartCircularRxDMABuffer[ bufIndex ];
        cmdlineRxCallback( recv );
    }
}


void HAL_UART_RxCpltCallback ( UART_HandleTypeDef* huart )
{
   handle_UART_RxCallbacks ( huart, 1 );
}


void HAL_UART_RxHalfCpltCallback ( UART_HandleTypeDef* huart )
{
    handle_UART_RxCallbacks ( huart, 0 );
}

void cmdlineInit(void)
{
   HAL_UART_Receive_DMA ( &hCmdLineUart, uartCircularRxDMABuffer, 2 );       // DGB_UART
   cmdlinePuts("\n\n");
   cmdlinePuts(VERSION_STRING);
   cmdlinePuts(" compiled ");
   cmdlinePuts(__DATE__);
   cmdlinePuts(" ");
   cmdlinePuts(__TIME__);
   cmdlinePuts("\n");
   cmdlineSendPrompt();
}

void cmdlineWorker(void)
{
   if (bParseCmdLineNow) {
      cmdlineProcess();
      bParseCmdLineNow = 0;
      cmdlineSendPrompt();
   }
}
