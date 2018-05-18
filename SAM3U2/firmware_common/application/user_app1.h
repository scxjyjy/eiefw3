/*!*********************************************************************************************************************
@file user_app1.h                                                                
@brief Header file for user_app1

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/
#define RxBufferMaxSize 20
#define US_SREADY_PB24 24
#define US_CTS2_PB21 21
typedef struct
{
 //every number location is fixed,so just calcuate it,and create a pointer for each location 
  u8* pu8ZeroPosition;//start from 0 so 24 is eual to 23
  u8* pu8OnePosition;
  u8* pu8TwoPosition;
  u8* pu8ThreePosition;
  u8* pu8FourPosition;
  u8* pu8FivePosition;
  u8* pu8SixPosition;
  u8* pu8SevenPosition;
  u8* pu8EightPosition;
}u8Position;
/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/
void SlaveRxFlowCallback(void);
void SlaveTxFlowCallback(void);
/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);
void Delayus(u16 DelaySlot);
bool referee(void);
void reStart(void);
/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    
static void UserApp1SM_Error(void);         
static void UserApp1SM_Tic_Tac_Toe(void);


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/


#endif /* __USER_APP1_H */
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
