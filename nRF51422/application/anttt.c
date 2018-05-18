/**********************************************************************************************************************
File: anttt.c                                                                

Description:
Implements TIC-TAC-TOE using data input from ANT or BLE.



**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_xxAnttt"
***********************************************************************************************************************/
/* New variables */
u32 G_u32AntttFlags;                                     /* Global state flags */

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */
extern volatile u32 G_u32BPEngenuicsFlags;             /* From bleperipheral_engenuics.c  */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Anttt_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type Anttt_pfnStateMachine;              /* The application state machine function pointer */
static u32 Anttt_u32RXBuffer=0x00000000UL;
static u8 Anttt_u8RXBuffer[3]={0,0,0};
static u8* Anttt_pu8RXBuffer=&Anttt_u8RXBuffer[0];
static bool bSendTask=false;
static u16 u16SendByteNumber=8;
/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: AntttInitialize

Description:
Initializes the State Machine and its variables.

Requires:

Promises:
*/
void AntttInitialize(void)
{
#if 1
   /*configure the spi-related pins*/
  NRF_SPI0 ->PSELSCK=11;
  NRF_SPI0 ->PSELMOSI=13;
  NRF_SPI0 ->PSELMISO=12;
  NRF_SPI0 ->CONFIG=0x00000003UL;

  /*Configure frequency*/
  NRF51422_SPI0->FREQUENCY=K125;

  /*enable chip*/
  //NRF_GPIO->OUTCLR=0X00000400UL;//10
  /*initialize set up sucessfully*/
  //LedOn(GREEN);;//GREEN LED
  //LedOff(BLUE);
  /*Disable twi0*/
  NRF_TWI0->ENABLE=0x00000000UL;
  NRF_SPI0 ->ENABLE=0x00000001UL;
  #endif
  LedOn(GREEN);
  LedOn(YELLOW);
  LedOn(RED);
  LedOn(BLUE);
  for(u16 i=0;i<1000;i++)
  {
    for(u16 a=0;a<10000;a++);
  }
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(RED);
  LedOff(BLUE);
  nrf_delay_us(10000);
  Anttt_pfnStateMachine = AntttSM_Idle;
  

} /* end AntttInitialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function AntttRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void AntttRunActiveState(void)
{
  Anttt_pfnStateMachine();

} /* end AntttRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------------------------*/
/* State Machine definitions                                                                                          */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
State: AntttSM_Idle
*/

static void AntttSM_Idle(void)
{
/*BLE  Portion*/   
//BPEngenuicsSendData(Anttt_pu8RXBuffer,1);
/*SPI  Portion*/  
#if 0
  static bool bLedIsOn=false;
  
  u8 u8TxByte=0x30;
  //bSendTask=true;
  /*if  you  want  to send useful byte,configure the bSendTask true*/
  if(bSendTask&&(u16SendByteNumber))
  {
    u16SendByteNumber--;
    /*if u32TxByte==0x00000000,we send dummy byte*/
    if(u8TxByte==0x00UL)
    {
      Putbyte(SSP_DEFAULT_TX_BYTE);
    }
    else
    {
      Putbyte(u8TxByte);
    }
    ReadByte(Anttt_u32RXBuffer);
    if(u16SendByteNumber==0)
    {
      DeAssertCS();
      u16SendByteNumber=8;
      bSendTask=false;
    }
  }
 
  if(NRF51422_SPI0->EVENTS_READY)
  {
    /*Clear the ready*/
    NRF51422_SPI0->EVENTS_READY=0;                 
    /*Blink Led*/
    
    if(bLedIsOn)
    {
      LedOff(BLUE);
      bLedIsOn=false;
    }
    else
    {
      LedOn(BLUE);
      bLedIsOn=true;
    }
  }
  switch(Anttt_u32RXBuffer)
  {
   case 0x00000001:LedToggle(GREEN);break;
   case 0x00000002:LedToggle(YELLOW);break;
   case 0x00000003:LedToggle(GREEN);break;
   case 0x00000004:LedToggle(RED);break;
  default:;
  }
  nrf_delay_us(1000000);
 #endif  
} 

#if 1
/*-----()-----*/
bool Putbyte(u32 TXBYTE )
{
  NRF51422_SPI0->TXD=TXBYTE&(0x000000FF);
  return 1;
}/*End  putbyte(u32 TXBYTE )*/
/*-----()-----*/
void LedOn(LED_Type led)
{
  NRF_GPIO->OUT|=(1<<led);
}/*End  LedOff(LED_Type led)*/
/*-----()-----*/
void LedOff(LED_Type led)
{
  NRF_GPIO->OUTCLR=(1<<led);
}/*End  LedOff(LED_Type led)*/

void LedToggle(LED_Type led)
{
  NRF_GPIO->OUT^=(1<<led);
}
#endif   
void SRDYCallBack()
{
  /*if the last the last sendtask has been finished,we deassertCs,so when
  the new event coming,we assert cs and  the token*/ 
  if(bSendTask==false)
  {
    SspAssertCS();
    bSendTask=true;
  }
}
void SspAssertCS(void)
{
  
  NRF_GPIO->OUTCLR=0X00000400UL;
}
void DeAssertCS(void)
{
  NRF_GPIO->OUTSET=0X00000400UL;
}
u8 ReadBleByte(void)
{
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
