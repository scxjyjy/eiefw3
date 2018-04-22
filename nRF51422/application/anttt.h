/**********************************************************************************************************************
File: anttt.h                                                                
**********************************************************************************************************************/

#ifndef __ANTTTT_H
#define __ANTTTT_H



/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
/* ANTTT Communication Protocol */
#define ANTTT_COMMAND_SIZE              (u8)3
#define ANTTT_COMMAND_ID_OFFSET         (u8)0x00
#define ANTTT_COMMAND_POSITION_OFFSET   (u8)0x01
#define ANTTT_COMMAND_UNUSED_OFFSET     (u8)0x02

#define ANTTT_COMMAND_ID_MOVE           (u8)0xCC
#define ANTTT_COMMAND_ID_MOVE_RESP      (u8)0xAC   
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
/*LED on nrf51422*/



/**********************************************************************************************************************
Type Definitions
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
/**/
typedef enum{BLUE=26,GREEN=27,YELLOW=28,RED=29}LED_Type;



/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
bool Putbyte(u32 TXBYTE );
void LedOn(LED_Type led);
void LedOff(LED_Type led);

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void AntttInitialize(void);
void AntttRunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* SM functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
static void AntttSM_Idle(void);



#endif /* __ANTTT_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
