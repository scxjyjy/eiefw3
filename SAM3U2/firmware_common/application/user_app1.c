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
static u8 u8RxBuffer[128]="\0";

static SspPeripheralType  *psAvaliablesp;
static u8* pu8Temp=NULL;
static u8* pu8RXNextbyte=&u8RxBuffer[0];
/*use array to record the location better than use a variable*/
static u8 u8GameRecord[9]={' ',' ',' ',' ',' ',' ',' ',' ',' '};
static u8 au8BleBuffer[13]={0x00};
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
 
  SspConfigurationType Sspcfg;
  Sspcfg.SspPeripheral=USART2;
  Sspcfg.pCsGpioAddress=AT91C_BASE_PIOB;
  Sspcfg.u32CsPin=22;
  Sspcfg.eBitOrder=LSB_FIRST;
  Sspcfg.eSspMode=SPI_SLAVE_FLOW_CONTROL;
  Sspcfg.fnSlaveTxFlowCallback=SlaveTxFlowCallback;
  Sspcfg.fnSlaveRxFlowCallback=SlaveRxFlowCallback;
  Sspcfg.pu8RxBufferAddress=pu8RXNextbyte;
  pu8Temp=Sspcfg.pu8RxBufferAddress;
  //pu8Temp++;
  //Sspcfg.ppu8RxNextByte=&(pu8T emp);
  Sspcfg.ppu8RxNextByte=&(pu8RXNextbyte);
  Sspcfg.u16RxBufferSize=RxBufferMaxSize;
  psAvaliablesp=SspRequest(&Sspcfg);
  //AT91C_BASE_US2->
  /* If good initialization, set state to Idle */
  if(  psAvaliablesp!=NULL )
  {

   
     /*ENABLE US_SREADY_PB24 */
     AT91C_BASE_PIOB->PIO_OER=(1<<24);
 #if 0
     /*Reset nrf*/
      AT91C_BASE_PIOB->PIO_OER=(1<<21);
     AT91C_BASE_PIOB->PIO_CODR=(1<<21);
     for(u8 i=0;i<=100;i++);
      AT91C_BASE_PIOB->PIO_SODR=(1<<21);
#endif
      /*initialize the sready line as high level*/
      AT91C_BASE_PIOB->PIO_SODR=(1<<US_SREADY_PB24);
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
   static u8 au8Display[13]="          " ;
   
   static u8* pu8BufferParser=u8RxBuffer;
   static u8 u8DisplayIndex=0;
   static u8 u8ActiveCounter=0;
#if 1
   while((pu8BufferParser)!=(pu8RXNextbyte))
   {
     /*READ*/
     au8Display[u8DisplayIndex]=*pu8BufferParser;
     au8BleBuffer[u8DisplayIndex]=*pu8BufferParser;
     u8DisplayIndex++;
     pu8BufferParser++;
     u8ActiveCounter++; 
     if((pu8BufferParser)>=(&u8RxBuffer[RxBufferMaxSize]))
     {
       pu8BufferParser=&u8RxBuffer[0];
     }
     if(u8DisplayIndex>11)
     {
      u8DisplayIndex=0;
     }
   }
   
   if(u8ActiveCounter>0)
   {
     //DebugPrintf(au8Display);
     u8ActiveCounter=0;
     LedToggle(GREEN);
   }
   
#endif
   if(WasButtonPressed(BUTTON0))
   {
     ButtonAcknowledge(BUTTON0);
     /*generate falling edge*/
     AT91C_BASE_PIOB->PIO_CODR=(1<<US_SREADY_PB24);
     Delayus(10);
     AT91C_BASE_PIOB->PIO_SODR=(1<<US_SREADY_PB24);
 
     
     SspWriteByte(psAvaliablesp, (u8)0x01);
     LedToggle(BLUE);
   }
   if(WasButtonPressed(BUTTON1))
   {
     ButtonAcknowledge(BUTTON1);
     
     AT91C_BASE_PIOB->PIO_CODR=(1<<US_SREADY_PB24);
     Delayus(10);
     AT91C_BASE_PIOB->PIO_SODR=(1<<US_SREADY_PB24);
     
     SspWriteByte(psAvaliablesp, (u8)0x10);
     LedToggle(RED);
   }
   if(WasButtonPressed(BUTTON2))
   {
     ButtonAcknowledge(BUTTON2);
     
     AT91C_BASE_PIOB->PIO_SODR=(1<<US_SREADY_PB24);
     Delayus(10);
     AT91C_BASE_PIOB->PIO_CODR=(1<<US_SREADY_PB24);
     
     SspWriteByte(psAvaliablesp, (u8)0x03);
     LedToggle(YELLOW);
   }
   if(WasButtonPressed(BUTTON3))
   {
     ButtonAcknowledge(BUTTON3);
     
     AT91C_BASE_PIOB->PIO_SODR=(1<<US_SREADY_PB24);
     Delayus(10);
     AT91C_BASE_PIOB->PIO_CODR=(1<<US_SREADY_PB24);
     
     SspWriteByte(psAvaliablesp, (u8)0x04);
     LedToggle(YELLOW);
   }
  UserApp1_pfStateMachine = UserApp1SM_Tic_Tac_Toe;
    
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  LedOn(RED);
  while(1);
} /* end UserApp1SM_Error() */
void SlaveRxFlowCallback(void)
{
  pu8RXNextbyte++;
  if(pu8RXNextbyte==(u8RxBuffer+psAvaliablesp->u16RxBufferSize))
  {
    pu8RXNextbyte=&u8RxBuffer[0];
  }
  
}

void SlaveTxFlowCallback(void)
{
  
}

void Delayus(u16 DelaySlot)
{
  for(u8 i=0;i<=1;i++)
  {
    for(u8 i=0;i<=DelaySlot;i++); 
  }
}
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1SM_Tic_Tac_Toe(void)

@brief :Used to  display figuration expected;
Here i use teraterm to test ,teraterm input and output the result
if you want to use the other way input,you can modify the state machine
i seperated the state from idle for the code clear,and easier find problems when
something wrong.
Requires:
-State machine function pointer points at current state
-if you want process correctly,please excute UserApp1SM_Idle 
firstly 

Promises:
- Calls the function to pointed by the state machine function pointer
-the display will follow the following  figure:
   - - - - - - - - -\n
    0  |  1  |  2   \n
   - - - - - - - - -\n
    3  |  4  |  5   \n
   - - - - - - - - -\n
    6  |  7  |  8   \n
   - - - - - - - - -\n
Notes: 
-there some apparently delay between the moment you press and the moment 
you show the result.but the show result are in order ,so dont worry. 
-i didnt design the game reset yet
*/
static void UserApp1SM_Tic_Tac_Toe(void)
{
  /*the 123456789<space> used for mark the display element for you count it  conveniently
  <space> is equal to 10
  you better check it again maybe a kind unaligned*/
  //                         1         2        3         4            5         6           7         8            9         10         11        12       13        14        15        16        17
  //                      123456789 1234567 8 9 123456789 1234567 8 9 123456789 123456 7 89 123456789 123456 7 89 123456789 12345 6 789 123456789 1234567 8 9 123456789 12345678 9 123456789 123456789 123456789
  static  u8 u8Display[]="- - - - - - - - -\n\r     |     |      \n\r- - - - - - - - -\n\r     |     |      \n\r- - - - - - - - -\n\r     |     |      \n\r- - - - - - - - -\n\r";
  //every number location is fixed,so just calcuate it,and create a pointer for each location 
  static u8* pu8ZeroPosition=&u8Display[21];//start from 0 so 24 is eual to 23
  static u8* pu8OnePosition=&u8Display[27];
  static u8* pu8TwoPosition=&u8Display[33];
  static u8* pu8ThreePosition=&u8Display[60];
  static u8* pu8FourPosition=&u8Display[66];
  static u8* pu8FivePosition=&u8Display[72];
  static u8* pu8SixPosition=&u8Display[99];
  static u8* pu8SevenPosition=&u8Display[105];
  static u8* pu8EightPosition=&u8Display[111];
  static bool bYourturn=TRUE;
  static u8 u8YourInput[1]={' '};
  
  /*assume you use O(pc use o),You use pc input,button0 change who start first
  at default your opponent first go*/
  /*DebugScanf only once use once it will clear the buffer,you cant find it  any more*/
  /*check input value*/
  if(DebugScanf(u8YourInput))
  {
    if(u8YourInput[0]>'8')
    {
      DebugPrintf("error location");
    }
    /*record used to against repetition*/
    if(u8GameRecord[u8YourInput[0]-'0']==' ')
    {
      if(bYourturn==0)
      {
        DebugPrintf("it's not your turn");
        //u8GameRecord[u8YourInput[0]-'0']='X';
        //bYourturn=TRUE;
      }
      else
      {
        /*here simulate your opponent when i just use pc*/
        u8GameRecord[u8YourInput[0]-'0']='O';
      }
      /*use the middle array,we largely reduce the length of code,cause the arry can unify*/
     if(bYourturn==TRUE)
     {
       switch(u8YourInput[0])
       {
        case '0':
          {*pu8ZeroPosition='O';bYourturn=0;}break;
        case '1':
          {*pu8OnePosition='O';bYourturn=0;}break;
        case '2':
          {*pu8TwoPosition=u8GameRecord[u8YourInput[0]-'0'];bYourturn=0;}break; 
        case '3':
          {*pu8ThreePosition=u8GameRecord[u8YourInput[0]-'0'];bYourturn=0;}break;
        case '4':
          {*pu8FourPosition=u8GameRecord[u8YourInput[0]-'0'];bYourturn=0;}break;
        case '5':
          {*pu8FivePosition=u8GameRecord[u8YourInput[0]-'0'];bYourturn=0;}break;
        case '6':
          {*pu8SixPosition=u8GameRecord[u8YourInput[0]-'0'];bYourturn=0;}break;
        case '7':
          {*pu8SevenPosition=u8GameRecord[u8YourInput[0]-'0'];bYourturn=0;}break;
        case '8':
          {*pu8EightPosition=u8GameRecord[u8YourInput[0]-'0'];bYourturn=0;}break;
        default:;      
      }
     }
      DebugPrintf(u8Display);
    }
     else if(u8GameRecord[u8YourInput[0]-'0']=='X'||u8GameRecord[u8YourInput[0]-'0']=='O')
     {
        DebugPrintf("location has been used");
     }
  }
  
  /*Data from ble*/
  /*in 1 ms ,i received 12 at most according to the ble transmit rate */
  static u8 u8counter=12;
  while(u8counter)
  {
    /*record used to against repetition*/
     if(au8BleBuffer[u8counter]-1>9)
     {
      /*0xff and 0x00 is dummy byte,dont need to response it*/
        if(au8BleBuffer[u8counter]!=0xFF&&au8BleBuffer[u8counter]!=0x00)
        DebugPrintf("error location");  
     }
    else if(u8GameRecord[au8BleBuffer[u8counter]-1]==' ')
    {
      if(bYourturn==0)
      {
        u8GameRecord[au8BleBuffer[u8counter]-1]='X';
      }
      else
      {
        /*here simulate your opponent when i just use pc*/
        DebugPrintf("it's not your turn");
        return;
      }
      switch(au8BleBuffer[u8counter])
      {
        
          case 0x01:
            if(bYourturn==0)
            {*pu8ZeroPosition=u8GameRecord[au8BleBuffer[u8counter]-1];bYourturn=1;}break;
          case 0x02:
            if(bYourturn==0)
            {*pu8OnePosition=u8GameRecord[au8BleBuffer[u8counter]-1];bYourturn=1;}break;
          case 0x03:
            if(bYourturn==0)
            {*pu8TwoPosition=u8GameRecord[au8BleBuffer[u8counter]-1];bYourturn=1;}break; 
          case 0x04:
            if(bYourturn==0)
            {*pu8ThreePosition=u8GameRecord[au8BleBuffer[u8counter]-1];bYourturn=1;}break;
          case 0x05:
            if(bYourturn==0)
            {*pu8FourPosition=u8GameRecord[au8BleBuffer[u8counter]-1];bYourturn=1;}break;
          case 0x06:
            if(bYourturn==0)
            {*pu8FivePosition=u8GameRecord[au8BleBuffer[u8counter]-1];bYourturn=1;}break;
          case 0x07:
            if(bYourturn==0)
            {*pu8SixPosition=u8GameRecord[au8BleBuffer[u8counter]-1];bYourturn=1;}break;
          case 0x08:
            if(bYourturn==0)
            {*pu8SevenPosition=u8GameRecord[au8BleBuffer[u8counter]-1];bYourturn=1;}break;
          case 0x09:
            if(bYourturn==0)
            {*pu8EightPosition=u8GameRecord[au8BleBuffer[u8counter]-1];bYourturn=1;}break;
          default:;       
      } 
      au8BleBuffer[u8counter]=0x00;
     
    }
    else if(u8GameRecord[au8BleBuffer[u8counter]-1]=='X'||u8GameRecord[au8BleBuffer[u8counter]-1]=='O')
    {
      DebugPrintf("location has been used");
    }
     u8counter--;
 
  }
  if(u8counter==0)
  {
    u8counter=12;
  }
  /*clear in case of the processed data be used again*/
  for(u8 i =0;i<=12;i++)
  {
    au8BleBuffer[i]=0x00;
  }
    
 
    
   
  /*every time i need  to clear u8YourIput in case of the last data be used*/
  u8YourInput[0]=' ';
  if(WasButtonPressed(BUTTON0))
  {
     ButtonAcknowledge(BUTTON0);
     bYourturn=!bYourturn;
     /*generate falling edge*/
     //DebugPrintf(u8Display);
  }
  UserApp1_pfStateMachine = UserApp1SM_Idle;
}
/*!----------------------------------------------------------------------------------------------------------------------
@fn void referee(void)

@brief referee is the person judge who win the game

Requires:
- 

Promises:
- 

*/
bool referee(void)
{
  /*this way has disadvantage:the length of code is too long,i think about it later*/
  return (u8GameRecord[0]==u8GameRecord[1]&&u8GameRecord[0]==u8GameRecord[2])||(u8GameRecord[3]==u8GameRecord[4]&&u8GameRecord[3]==u8GameRecord[5])||\
   (u8GameRecord[6]==u8GameRecord[7]&&u8GameRecord[6]==u8GameRecord[8])||(u8GameRecord[0]==u8GameRecord[3]&&u8GameRecord[0]==u8GameRecord[6])||\
   (u8GameRecord[1]==u8GameRecord[4]&&u8GameRecord[1]==u8GameRecord[7])||(u8GameRecord[2]==u8GameRecord[5]&&u8GameRecord[2]==u8GameRecord[8])||\
   (u8GameRecord[0]==u8GameRecord[4]&&u8GameRecord[0]==u8GameRecord[8])||  (u8GameRecord[2]==u8GameRecord[4]&&u8GameRecord[2]==u8GameRecord[6]);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
