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
extern int MarginPulse;  //页边距马达的脉冲数目 要加上10的
extern int WidthPulse;  //页边距马达的脉冲数目 要加上10的

extern int ForwardBackwardNum; //总共来回的次数
extern int ForwardBackwardCur; //现在的次数 他的总数是上面来回次数的两倍 + 2(因为还要加上两次的页边距移动)

extern bool bCancel; //现在的次数

extern bool bSensorON; //现在的传感器是否触发
extern bool bBackward; //是否为反转，TURE是反转，False是正转
extern bool bBackHP; //是否需要进行第一次到HP位置
extern bool bIsRuning; //是否在打胶运行中

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
    static int count = 0;
    static int bCancelFinished = FALSE;
    count++;
    //反转到HP位置，当传感器为ON的时候，速度就要慢慢的落下来
    if(bSensorON && bBackward){
//        count = (MovePulse-16);
        //MovePulse = count + 16;
      MovePulse = count;
        
    }
    if( MovePulse < 33 ){ //TODO 脉冲数目太小来不及慢起慢落
        //TODO
       // BEEP_Cmd(ENABLE);
    }
    else if( count == 4 ){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                          TIM1_COUNTERMODE_UP , //向上计数
                          CRR[1],      //自动重载值
                          0
                              );
        TIM1_SetCompare3(ARR[1]);
    }
    else if(count == 8){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                          TIM1_COUNTERMODE_UP , //向上计数
                          CRR[2],      //自动重载值
                          0
                              );
        TIM1_SetCompare3(ARR[2]);
    }
    else if(count == 12){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                          TIM1_COUNTERMODE_UP , //向上计数
                          CRR[3],      //自动重载值
                          0
                              );
        TIM1_SetCompare3(ARR[3]);
    }
    else if(count == 16){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                          TIM1_COUNTERMODE_UP , //向上计数
                          CRR[4],      //自动重载值
                          0
                              );
        TIM1_SetCompare3(ARR[4]);
    }
    else if(count == (MovePulse-16)){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                          TIM1_COUNTERMODE_UP , //向上计数
                          CRR[3],      //自动重载值
                          0
                              );
        TIM1_SetCompare3(ARR[3]);
        
    }
    else if(count == (MovePulse-12)){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                          TIM1_COUNTERMODE_UP , //向上计数
                          CRR[2],      //自动重载值
                          0
                              );
        TIM1_SetCompare3(ARR[2]);
    }
    else if(count == (MovePulse-8)){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                          TIM1_COUNTERMODE_UP , //向上计数
                          CRR[1],      //自动重载值
                          0
                              );
        TIM1_SetCompare3(ARR[1]);
    }
    else if(count == (MovePulse-4)){
        TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                          TIM1_COUNTERMODE_UP , //向上计数
                          CRR[0],      //自动重载值
                          0
                              );
        TIM1_SetCompare3(ARR[0]);
    }
    else if(count >= MovePulse){
        count = 0;
        StopMotor1();
        ForwardBackwardCur++; //移动完毕以后，当前的移动次数加一。
        delay(50);     
        if(bBackHP){ //不在HP位置的情况下，第一次回到HP位置，这是特殊情况
            
            if( bCancel ){ //刚回到HP位置就被取消了，那么就停下来
                //取消的最后返回结束以后，就什么都不做了。
                bCancelFinished = FALSE;
                bCancel = FALSE;
                bIsRuning = FALSE; //运行结束了。
            }
            else{ //否则继续正常的打胶
                ForwardBackwardCur = 0;
                MovePulse = MarginPulse; //第一次移动变宽距离。            
                ForwardMotor1();
                StartMotor1();
                bBackHP = FALSE; 
                bCancelFinished = FALSE;
                bCancel = FALSE;
            }
        }
        else {  //不是第一次回到HP位置，这是一般情况      
        
            if(bCancel){  //取消键按下以后，改移动完毕以后，才进行取消的返回处理
                if( bCancelFinished ){ //第二回，取消按钮按下后，剩余的移动动作做完了
                   
                    //取消的最后返回结束以后，就什么都不做了。
                    bCancelFinished = FALSE;
                    bCancel = FALSE;
                    bIsRuning = FALSE; //运行结束了。
                }
                else{ //第一回，取消按钮以后，要进行剩余的移动动作
                    
                    if( ForwardBackwardCur == 1 ){ //第一次页边距移动后，不要放下电磁阀，继续反转马达1,返回远点距离是页边距
                        //不放下电磁阀
                        
                        MovePulse = MarginPulse;
                        ReverseMotor1();
                        StartMotor1();
                        bCancelFinished = TRUE;
                    }
                    else if(ForwardBackwardCur == (ForwardBackwardNum*2 +1)){ //最后一次中宽移动以后，要继续移动马达1，距离是页边距
                        
                        //提高电磁阀，移动到最旁边
                        UpValve();
                        delay(50);
                        
                        MovePulse = MarginPulse;
                        ReverseMotor1();
                        StartMotor1();
                        bCancelFinished = TRUE;
                    }
                    else if(ForwardBackwardCur == (ForwardBackwardNum*2 + 2)){ //最后一次的页边距移动完毕后，就停止马达
                        
                       bCancel = FALSE;
                       bCancelFinished = FALSE;
                       count = 0;
                       //最后一次，就放下电磁阀
                       DownValve();
                       bIsRuning = FALSE；//运行结束了
                    }
                    else if(ForwardBackwardCur%2 == 0){  //已经移到最右边了，需要移动中宽+页边距的距离回去

                        //提高电磁阀
                        UpValve();
                        delay(50);
                        
                        MovePulse = MarginPulse + WidthPulse;
                        ReverseMotor1();
                        StartMotor1();
                        bCancelFinished = TRUE;                   
                    }
                    else{  //在左边，只需要移动页边距回去就行了
                        
                        //提高电磁阀
                        UpValve();
                        delay(50);
                        
                        MovePulse = MarginPulse;
                        ReverseMotor1();
                        StartMotor1();
                        bCancelFinished = TRUE;
                    
                    }
                }//if cancelfinished
                
            }
            else{        
                if( ForwardBackwardCur == 1 ){ //第一次页边距移动后，要放下电磁阀，继续启动马达1,距离是中宽
                    //放下电磁阀，进行打胶
                    DownValve();
                    delay(50);
                    
                    MovePulse = WidthPulse;
                    ForwardMotor1();
                    StartMotor1();
                }
                else if(ForwardBackwardCur == (ForwardBackwardNum*2 + 1)){ //最后一次中宽移动以后，要继续移动马达1，距离是页边距
                    //提高电磁阀，移动到最旁边
                    UpValve();
                    delay(50);
                    
                    MovePulse = MarginPulse;
                    ReverseMotor1();
                    StartMotor1();
                }
                else if(ForwardBackwardCur == (ForwardBackwardNum*2 + 2)){ //最后一次的页边距移动完毕后，就停止马达
                    //TODO 初始化
                    bCancelFinished = FALSE;
                    bCancel = FALSE;
                    count = 0;
                    
                    //放下电磁阀，最后一次，停止马达
                    DownValve();
                    bIsRuning = FALSE;
                    
                }
                else{  //其余的情况下，都要重启马达2
                    
                    //提高电磁阀，移动胶布
                    UpValve();
                    delay(50);
                    
                    StartMotor2();
                }
            }//if bcancel
            
        }//if bBackHP
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
    if(count2 == 36){  //马达2移动结束的时候
        count2 = 0;
        StopMotor2();
        delay(50);
        
        //放下电磁阀,进行打胶
        DownValve();
        
        delay(50);
        if( ForwardBackwardCur%2 == 0 ){ //正方向移动完毕，下次准备移动反方向
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
    static s8 SetCount = 0,ResetCount = 0; //Reset表示触发，Set表示没有触发
        
    if(TIM4_GetITStatus(TIM4_IT_UPDATE) == SET )
    {
        TIM4_ClearFlag(TIM4_FLAG_UPDATE);   //清除中断标志
        if(RESET == GPIO_ReadInputPin(GPIOB , GPIO_PIN_3)){
            ResetCount++;
            SetCount=0;
        }
        else{
            ResetCount=0;
            SetCount++;
            
        }
        if(ResetCount == 3){
            SetCount = 0;
            ResetCount = 0;
            bSensorON = TRUE;
        }
        if(SetCount == 3){
            SetCount = 0;
            ResetCount = 0;
            bSensorON = FALSE;
        }
    }
    
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
