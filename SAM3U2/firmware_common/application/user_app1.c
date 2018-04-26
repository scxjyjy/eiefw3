/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */
static u8 u8RxBuffer[128]="";
static SspConfigurationType *Sspcfg;
static SspPeripheralType  *sAvaliablesp;
static u8* pu8Temp=NULL;
/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{
  
  Sspcfg->SspPeripheral=USART2;
  Sspcfg->pCsGpioAddress=AT91C_BASE_PIOB;
  Sspcfg->u32CsPin=22;
  Sspcfg->eBitOrder=MSB_FIRST;
  Sspcfg->eSspMode=SPI_SLAVE;
  Sspcfg->fnSlaveTxFlowCallback=SlaveTxFlowCallback;
  Sspcfg->fnSlaveRxFlowCallback=SlaveRxFlowCallback;
  Sspcfg->pu8RxBufferAddress=u8RxBuffer;
  pu8Temp=Sspcfg->pu8RxBufferAddress;
  pu8Temp++;
  Sspcfg->ppu8RxNextByte=&(pu8Temp);
  Sspcfg->u16RxBufferSize=128;
  sAvaliablesp=SspRequest(Sspcfg);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{
   static u8 au8Display[13]="          \n" ;
   static u8* pu8BufferParser=u8RxBuffer;
   static u8 u8DisplayIndex=0;
   static u8 u8ActiveCounter=0;
   if((pu8BufferParser)!=(*(sAvaliablesp->ppu8RxNextByte)))
   {
     /*READ*/
     au8Display[u8DisplayIndex]=*pu8BufferParser;
     u8DisplayIndex++;
     pu8BufferParser++;
     u8ActiveCounter++; 
   }
   if(u8DisplayIndex>12)
   {
     u8DisplayIndex=0;
   }
   if(u8ActiveCounter>0)
   {
     u8ActiveCounter=0;
     LedToggle(GREEN);
   }
   
    
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
void SlaveRxFlowCallback(void)
{
  sAvaliablesp->pu8RxBuffer++; 
  *(sAvaliablesp->ppu8RxNextByte)++;
  if(*(sAvaliablesp->ppu8RxNextByte)==(u8RxBuffer+sAvaliablesp->u16RxBufferSize))
  {
    sAvaliablesp->pu8RxBuffer=u8RxBuffer;
    pu8Temp=u8RxBuffer+1;
    sAvaliablesp->ppu8RxNextByte=&(pu8Temp);
  }
}

void SlaveTxFlowCallback(void)
{
  
}


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
