/**
  ******************************************************************************
  * @file    stm8s_it.c
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service 
  *          routine.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s_it.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
    int CRR[5] = {10000/400, 10000/600, 10000/800,10000/1000,10000/1200};
    int ARR[5] = {10000/800, 10000/1200, 10000/1600,10000/2000,10000/2400};
extern int XXX;
extern int count;
extern int MovePulse;
extern int MarginPulse;  //Ò³±ß¾àÂí´ïµÄÂö³åÊýÄ¿ Òª¼ÓÉÏ10µÄ
extern int WidthPulse;  //Ò³±ß¾àÂí´ïµÄÂö³åÊýÄ¿ Òª¼ÓÉÏ10µÄ

extern int ForwardBackwardNum; //×Ü¹²À´»ØµÄ´ÎÊý
extern int ForwardBackwardCur; //ÏÖÔÚµÄ´ÎÊý ËûµÄ×ÜÊýÊÇÉÏÃæÀ´»Ø´ÎÊýµÄÁ½±¶ + 2(ÒòÎª»¹Òª¼ÓÉÏÁ½´ÎµÄÒ³±ß¾àÒÆ¶¯)

extern bool bCancel; //ÏÖÔÚµÄ´ÎÊý

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy Interrupt routine
  * @par Parameters:
  * None
  * @retval
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*_COSMIC_*/

/**
  * @brief TRAP Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Top Level Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TLI_IRQHandler, 0)

{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Auto Wake Up Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Clock Controller Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTA Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTB Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTC Interrupt routine.
  * @param  None
  * @retval None
  */

INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */

}

/**
  * @brief External Interrupt PORTD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined (STM8S903) || defined (STM8AF622x) 
/**
  * @brief External Interrupt PORTF Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined (STM8AF52Ax)
/**
  * @brief CAN RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

/**
  * @brief CAN TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S208) || (STM8AF52Ax) */

/**
  * @brief SPI Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer1 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
    /* In order to detect unexpected events during development,
      it is recommended to set a breakpoint on the following instruction.
    */
<<<<<<< HEAD
    static int count = 0;
    count++;
    if( MovePulse < 33 ){ //TODO Âö³åÊýÄ¿Ì«Ð¡À´²»¼°ÂýÆðÂýÂä
        //TODO
      BEEP_Cmd(ENABLE);
    }
    else if( count == 4 ){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                            TIM1_COUNTERMODE_UP , //ÏòÉÏ¼ÆÊý
                            CRR[1],      //×Ô¶¯ÖØÔØÖµ
=======
    static count = 0;
    int CRR[5] = {10000/400, 10000/600, 10000/800,10000/1000,10000/1200};
    int ARR[5] = {10000/800, 10000/1200, 10000/1600,10000/2000,10000/2400};
    count++;
    if( count == 4 ){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                            TIM1_COUNTERMODE_UP , //å‘ä¸Šè®¡æ•°
                            CRR[1],      //è‡ªåŠ¨é‡è½½å€¼
>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
                            0
                            );
         TIM1_SetCompare3(ARR[1]);
    }
    else if(count == 8){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
<<<<<<< HEAD
                        TIM1_COUNTERMODE_UP , //ÏòÉÏ¼ÆÊý
                        CRR[2],      //×Ô¶¯ÖØÔØÖµ
=======
                        TIM1_COUNTERMODE_UP , //å‘ä¸Šè®¡æ•°
                        CRR[2],      //è‡ªåŠ¨é‡è½½å€¼
>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
                        0
                        );
        TIM1_SetCompare3(ARR[2]);
    }
    else if(count == 12){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
<<<<<<< HEAD
                        TIM1_COUNTERMODE_UP , //ÏòÉÏ¼ÆÊý
                        CRR[3],      //×Ô¶¯ÖØÔØÖµ
=======
                        TIM1_COUNTERMODE_UP , //å‘ä¸Šè®¡æ•°
                        CRR[3],      //è‡ªåŠ¨é‡è½½å€¼
>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
                        0
                        );
        TIM1_SetCompare3(ARR[3]);
    }
    else if(count == 16){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
<<<<<<< HEAD
                        TIM1_COUNTERMODE_UP , //ÏòÉÏ¼ÆÊý
                        CRR[4],      //×Ô¶¯ÖØÔØÖµ
=======
                        TIM1_COUNTERMODE_UP , //å‘ä¸Šè®¡æ•°
                        CRR[4],      //è‡ªåŠ¨é‡è½½å€¼
>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
                        0
                        );
        TIM1_SetCompare3(ARR[4]);
    }
<<<<<<< HEAD
    else if(count == (MovePulse-16)){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                        TIM1_COUNTERMODE_UP , //ÏòÉÏ¼ÆÊý
                        CRR[3],      //×Ô¶¯ÖØÔØÖµ
=======
    else if(count == XXX-16){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                        TIM1_COUNTERMODE_UP , //å‘ä¸Šè®¡æ•°
                        CRR[3],      //è‡ªåŠ¨é‡è½½å€¼
>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
                        0
                        );
        TIM1_SetCompare3(ARR[3]);

    }
<<<<<<< HEAD
    else if(count == (MovePulse-12)){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                        TIM1_COUNTERMODE_UP , //ÏòÉÏ¼ÆÊý
                        CRR[2],      //×Ô¶¯ÖØÔØÖµ
=======
    else if(count == (XXX-12){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                        TIM1_COUNTERMODE_UP , //å‘ä¸Šè®¡æ•°
                        CRR[2],      //è‡ªåŠ¨é‡è½½å€¼
>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
                        0
                        );
        TIM1_SetCompare3(ARR[2]);
    }
<<<<<<< HEAD
    else if(count == (MovePulse-8)){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                        TIM1_COUNTERMODE_UP , //ÏòÉÏ¼ÆÊý
                        CRR[1],      //×Ô¶¯ÖØÔØÖµ
=======
    else if(count == (XXX-8){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                        TIM1_COUNTERMODE_UP , //å‘ä¸Šè®¡æ•°
                        CRR[1],      //è‡ªåŠ¨é‡è½½å€¼
>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
                        0
                        );
        TIM1_SetCompare3(ARR[1]);
    }
<<<<<<< HEAD
    else if(count == (MovePulse-4)){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                        TIM1_COUNTERMODE_UP , //ÏòÉÏ¼ÆÊý
                        CRR[0],      //×Ô¶¯ÖØÔØÖµ
=======
    else if(count == (XXX-4){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                        TIM1_COUNTERMODE_UP , //å‘ä¸Šè®¡æ•°
                        CRR[0],      //è‡ªåŠ¨é‡è½½å€¼
>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
                        0
                        );
        TIM1_SetCompare3(ARR[0]);
    }
<<<<<<< HEAD
    else if(count == MovePulse){
        count = 0;
        StopMotor1();
        ForwardBackwardCur++; //ÒÆ¶¯Íê±ÏÒÔºó£¬µ±Ç°µÄÒÆ¶¯´ÎÊý¼ÓÒ»¡£
        delay(50);     

        if(bCancel){  //È¡Ïû¼ü°´ÏÂÒÔºó
             if( ForwardBackwardCur == 1 ){ //µÚÒ»´ÎÒ³±ß¾àÒÆ¶¯ºó£¬²»Òª·ÅÏÂµç´Å·§£¬¼ÌÐø·´×ªÂí´ï1,·µ»ØÔ¶µã¾àÀëÊÇÒ³±ß¾à
                //²»·ÅÏÂµç´Å·§
               
                MovePulse = MarginPulse;
                ReverseMotor1();
                StartMotor1();
            }
            else if(ForwardBackwardCur == (ForwardBackwardNum*2 +1)){ //×îºóÒ»´ÎÖÐ¿íÒÆ¶¯ÒÔºó£¬Òª¼ÌÐøÒÆ¶¯Âí´ï1£¬¾àÀëÊÇÒ³±ß¾à
                //Ìá¸ßµç´Å·§
                MovePulse = MarginPulse;
                ReverseMotor1();
                StartMotor1();
            }
            else if(ForwardBackwardCur == (ForwardBackwardNum*2 + 2)){ //×îºóÒ»´ÎµÄÒ³±ß¾àÒÆ¶¯Íê±Ïºó£¬¾ÍÍ£Ö¹Âí´ï
                //TODO ³õÊ¼»¯
            }
            else{  //ÆäÓàµÄÇé¿öÏÂ£¬¶¼ÒªÖØÆôÂí´ï2

              //Ì§Æðµç´Å·§
                StartMotor2();
            }
            
            
        }
        else{        
            if( ForwardBackwardCur == 1 ){ //µÚÒ»´ÎÒ³±ß¾àÒÆ¶¯ºó£¬Òª·ÅÏÂµç´Å·§£¬¼ÌÐøÆô¶¯Âí´ï1,¾àÀëÊÇÖÐ¿í
                //·ÅÏÂµç´Å·§
                MovePulse = WidthPulse;
                ForwardMotor1();
                StartMotor1();
            }
            else if(ForwardBackwardCur == (ForwardBackwardNum*2 +1)){ //×îºóÒ»´ÎÖÐ¿íÒÆ¶¯ÒÔºó£¬Òª¼ÌÐøÒÆ¶¯Âí´ï1£¬¾àÀëÊÇÒ³±ß¾à
                //Ìá¸ßµç´Å·§
                MovePulse = MarginPulse;
                ReverseMotor1();
                StartMotor1();
            }
            else if(ForwardBackwardCur == (ForwardBackwardNum*2 + 2)){ //×îºóÒ»´ÎµÄÒ³±ß¾àÒÆ¶¯Íê±Ïºó£¬¾ÍÍ£Ö¹Âí´ï
                //TODO ³õÊ¼»¯
            }
            else{  //ÆäÓàµÄÇé¿öÏÂ£¬¶¼ÒªÖØÆôÂí´ï2

              //Ì§Æðµç´Å·§
                StartMotor2();
            }
        }
                       
        
=======
    else if(count == XXX){
       Disable();
>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
    }

    TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
}

/**
  * @brief Timer1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined (STM8S903) || defined (STM8AF622x)
/**
  * @brief Timer5 Update/Overflow/Break/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
 
/**
  * @brief Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

#else /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8S103) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */
/**
  * @brief Timer2 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
     static int count2 = 0;
     count2++;
     if(count2 == 36){  //Âí´ï2ÒÆ¶¯½áÊøµÄÊ±ºò
        count2 = 0;
        StopMotor2();
        delay(50);
        
        if( ForwardBackwardCur%2 == 0 ){ //Õý·½ÏòÒÆ¶¯Íê±Ï£¬ÏÂ´Î×¼±¸ÒÆ¶¯·´·½Ïò
            ReverseMotor1();
        }
        else{
            ForwardMotor1();
        }        
        MovePulse = WidthPulse;
        StartMotor1();
     
     }
   
     TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
 }

/**
  * @brief Timer2 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
/**
  * @brief Timer3 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

/**
  * @brief Timer3 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined (STM8S003) || defined(STM8S001) || defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)
/**
  * @brief UART1 TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART1 RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8S103) || (STM8S001) || (STM8S903) || (STM8AF62Ax) || (STM8AF52Ax) */

#if defined(STM8AF622x)
/**
  * @brief UART4 TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART4_TX_IRQHandler, 17)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART4 RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART4_RX_IRQHandler, 18)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8AF622x) */

/**
  * @brief I2C Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
/**
  * @brief UART2 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART2 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S105) || (STM8AF626x) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief UART3 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_TX_IRQHandler, 20)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART3 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief ADC2 interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(ADC2_IRQHandler, 22)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#else /* STM8S105 or STM8S103 or STM8S903 or STM8AF626x or STM8AF622x */
/**
  * @brief ADC1 interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
 INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined (STM8S903) || defined (STM8AF622x)
/**
  * @brief Timer6 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler, 23)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#else /* STM8S208 or STM8S207 or STM8S105 or STM8S103 or STM8AF52Ax or STM8AF62Ax or STM8AF626x */
/**
  * @brief Timer4 Update/Overflow Interrupt routine.
  * @param  None
  * @retval None
  */ 
 INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
   
  
 }
#endif /* (STM8S903) || (STM8AF622x)*/

/**
  * @brief Eeprom EEC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
