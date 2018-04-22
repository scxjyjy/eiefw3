/**********************************************************************************************************************
File: spi-Master.h  

Description:
Header file for spi-Master.c

**********************************************************************************************************************/

#include "configuration.h"


/**********************************************************************************************************************
Type Defination
**********************************************************************************************************************/
typedef volatile unsigned long U32NRF51;
typedef struct
{
  U32NRF51 RESERVED0[66];
  U32NRF51 EVENTS_READY;
  U32NRF51 RESERVED1[126];
  U32NRF51 INTENSET;
  U32NRF51 INTENCLR;
  U32NRF51 RESERVED2[125];
  U32NRF51 ENABLE;        //Enable SPI
  U32NRF51 RESERVED3;
  U32NRF51 PSELSCK ;       //Pin select for SCK
  U32NRF51 PSELMOSI;        //Pin select for MOSI
  U32NRF51 PSELMISO ;       //Pin select for MISO
  U32NRF51 RESERVED4;  
  U32NRF51 RXD   ;          // RXD register
  U32NRF51 TXD ;            // TXD register
  U32NRF51 RESERVED5;
  U32NRF51 FREQUENCY  ;     // SPI frequency
  U32NRF51 RESERVED6[11];
  U32NRF51 CONFIG  ;//Configuration register 
}NRF_SPIM_TYPE;







/**********************************************************************************************************************
Macro defination
**********************************************************************************************************************/
/*SPI frequency*/
#define K125  0x02000000UL
#define K250  0x04000000UL
#define K500  0x08000000UL
#define M1    0x10000000UL
#define M2    0x20000000UL
#define M4    0x40000000UL
#define M8    0x80000000UL
/*SPI Peripheral BaseAddress,it will name as <product name>_<peripherial name>_Base*/
#define NRF51422_SPI1_BASE 0x40004000UL
#define NRF51422_SPI0_BASE 0x40003000UL
/*register baseaddress*/
#define NRF51422_SPI0  ((NRF_SPIM_TYPE*)NRF51422_SPI0_BASE)


/**********************************************************************************************************************
Function declartations
**********************************************************************************************************************/

/*-----public function-----*/
bool putbyte(u32 TXBYTE );
/*-----protected function-----*/
void SpimRunActiveState(void);
void SpimInitialize(void);
/*-----private function-----*/



/**********************************************************************************************************************
State machine declartations
**********************************************************************************************************************/
void SpiMSM_Idle(void);
void SpiMSM_Error(void);
/**********************************************************************************************************************
-----End of File-----
**********************************************************************************************************************/
































