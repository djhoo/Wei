/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   Main program body
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
#include "stm8s.h"
#include "HT1632.h"

/* Private function prototypes -----------------------------------------------*/

/* Public value  -----------------------------------------------*/
//全局变量
u8 number[4] ;
u16 eepromaddress = 0x4000;   //设EEPROM的首地址为0X4000

u16  g_margin = 200;
u16  g_width = 1000;
u16  g_height = 160;
volatile int XXX = 36;     //�п��ƶ��ܵ������������Ŀ
volatile int MarginPulse = 0;  //ҳ�߾������������Ŀ Ҫ����10��
volatile int WidthPulse = 0;  //ҳ�߾������������Ŀ Ҫ����10��
volatile  int MovePulse = 0;  //�����ƶ�����
volatile int TotalWidthPulse = 0;

volatile  int ForwardBackwardNum = 0; //�ܹ����صĴ���
volatile  int ForwardBackwardCur = 0; //���ڵĴ���

volatile  bool bCancel = false; //���ڵĴ���


/* Private defines -----------------------------------------------------------*/
#define MOTOR_DIV   (4)

/* Private functions ---------------------------------------------------------*/
void delay(unsigned int ms)
{
  unsigned int x , y;
  for(x = ms; x > 0; x--)           /*  通过一定周期循环进行延时*/
    for(y = 3000 ; y > 0 ; y--);
}



void NumericDisplay_Init()
{
  //设置PA1为输出 ，HC164 CLK
  GPIO_Init(GPIOB , GPIO_PIN_4 , GPIO_MODE_OUT_PP_LOW_FAST);  
    
  //设置PA2为输出 ，HC164DAT
  GPIO_Init(GPIOB , GPIO_PIN_5 , GPIO_MODE_OUT_PP_LOW_FAST);  
    
  //设置PC3为输出 ，位码数
  GPIO_Init(GPIOB , GPIO_PIN_7 , GPIO_MODE_OUT_PP_LOW_FAST); 
}

/*******************************************************************************
key Init
*******************************************************************************/
/*******************************************************************************
**函数名称：void KEYInit()
**功能描述：配置输入按键
**入口参数：无
**输出：无
*******************************************************************************/
void BeepInit()
{
   BEEP_Init(BEEP_FREQUENCY_2KHZ);
}

void KEYInit()
{
    //GPD->PIN7 设置为输入模式 , 上拉电阻输入 , 禁止外部中断
    GPIO_Init(GPIOD , GPIO_PIN_7 , GPIO_MODE_IN_FL_NO_IT);  //key1 
    GPIO_Init(GPIOD , GPIO_PIN_6 , GPIO_MODE_IN_FL_NO_IT);  //key2
    GPIO_Init(GPIOD , GPIO_PIN_5 , GPIO_MODE_IN_FL_NO_IT);  //key3
    GPIO_Init(GPIOD , GPIO_PIN_3 , GPIO_MODE_IN_FL_NO_IT);  //key4
    GPIO_Init(GPIOD , GPIO_PIN_2 , GPIO_MODE_IN_FL_NO_IT);  //key5
    GPIO_Init(GPIOD , GPIO_PIN_0 , GPIO_MODE_IN_FL_NO_IT);  //key6
    GPIO_Init(GPIOC , GPIO_PIN_7 , GPIO_MODE_IN_FL_NO_IT);  //key7
    GPIO_Init(GPIOC , GPIO_PIN_6 , GPIO_MODE_IN_FL_NO_IT);  //key8

}

/*******************************************************************************
MOTOR Init
*******************************************************************************/
void MotorInit()
{
    GPIO_Init(GPIOC , GPIO_PIN_5 , GPIO_MODE_OUT_PP_LOW_SLOW); //motor ena PC5
    GPIO_Init(GPIOC , GPIO_PIN_4 , GPIO_MODE_OUT_PP_LOW_SLOW); //motor dir PC4
    GPIO_Init(GPIOC , GPIO_PIN_3 , GPIO_MODE_OUT_PP_LOW_SLOW); //motor CP  PC3  Tim1_CH3 
    
    GPIO_Init(GPIOC , GPIO_PIN_2 , GPIO_MODE_OUT_PP_LOW_SLOW); //motor2 ena PC2
    GPIO_Init(GPIOC , GPIO_PIN_1 , GPIO_MODE_OUT_PP_LOW_SLOW); //motor2 dir PC1
    GPIO_Init(GPIOA , GPIO_PIN_3 , GPIO_MODE_OUT_PP_LOW_SLOW); //motor2 CP  PA3 Tim2_CH3
}

/*******************************************************************************
**函数名称：void TIM1_PWM_Init()
**功能描述：定时器1 PWM初始化 Mortor1
**入口参数：无
**输出：无
*******************************************************************************/
int    Fpwm = 400;            //150HZ

void TIM1_PWM_Init()
{
    TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
<<<<<<< HEAD
                        TIM1_COUNTERMODE_UP , //���ϼ���
                        10000/Fpwm,      //�Զ�����ֵ
                        0
                        );
    
    TIM1_OC3Init(TIM1_OCMODE_PWM2 ,  //����ΪPWM2���ģʽ
                TIM1_OUTPUTSTATE_ENABLE , //���ʹ��
                TIM1_OUTPUTNSTATE_DISABLE,
                0  ,   //������Ϊ0
=======
                        TIM1_COUNTERMODE_UP , //向上计数
                        10000/Fpwm,      //自动重载值
                        0
                        );
    
    TIM1_OC3Init(TIM1_OCMODE_PWM2 ,  //设置为PWM2输出模式
                TIM1_OUTPUTSTATE_ENABLE , //输出使能
                TIM1_OUTPUTNSTATE_DISABLE,
                0  ,   //先设置为0
>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
                TIM1_OCPOLARITY_HIGH ,//OC1 HI
                TIM1_OCNPOLARITY_LOW,
                TIM1_OCIDLESTATE_SET,
                TIM1_OCNIDLESTATE_SET
                );
<<<<<<< HEAD
    TIM1_OC3PreloadConfig(ENABLE);
    TIM1_ARRPreloadConfig(ENABLE);
    TIM1_CtrlPWMOutputs(DISABLE);
    TIM1_CCxCmd(TIM1_CHANNEL_3 , DISABLE);
    TIM1_Cmd(DISABLE);
        
    TIM1_SetCompare3(10000/Fpwm/2);

}
    

void StartMotor1()
{
    GPIO_WriteHigh(GPIOC , GPIO_PIN_5);
    delay(10);
 
    TIM1_CCxCmd(TIM1_CHANNEL_3 , ENABLE);
    TIM1_Cmd(ENABLE);
  
    TIM1_CtrlPWMOutputs(ENABLE);
    TIM1_ITConfig(TIM1_IT_UPDATE , ENABLE);
}

void StopMotor1()
{
    GPIO_WriteLow(GPIOC , GPIO_PIN_5);
    TIM1_CtrlPWMOutputs(DISABLE);
    TIM1_ITConfig(TIM1_IT_UPDATE , DISABLE);
    TIM1_CCxCmd(TIM1_CHANNEL_3 , DISABLE);
    TIM1_Cmd(DISABLE);
}

void ForwardMotor1()
{
    GPIO_WriteLow(GPIOC , GPIO_PIN_4);
}

void ReverseMotor1()
{
    GPIO_WriteHigh(GPIOC , GPIO_PIN_4);
=======
    TIM1_CCxCmd(TIM1_CHANNEL_3 , ENABLE);
    TIM1_OC3PreloadConfig(ENABLE);
    
    TIM1_CtrlPWMOutputs(ENABLE);
    TIM1_Cmd(ENABLE);
    
    TIM1_SetCompare3(10000/Fpwm/2);

    TIM1_ITConfig(TIM1_IT_UPDATE , ENABLE);


>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
}

/*******************************************************************************
**函数名称：void TIM2_PWM_Init()
**功能描述：定时器1 PWM初始化 Mortor2
**入口参数：无
**输出：无
*******************************************************************************/
#define     Fpwm2          200            //50HZ
void TIM2_PWM_Init()
{
<<<<<<< HEAD
    TIM2_TimeBaseInit(TIM2_PRESCALER_512 , //16Mhz / 512 = 31250 HZ
                     (31250 / Fpwm2)      //�Զ�����ֵ
                    );
  
    TIM2_OC3Init(TIM2_OCMODE_PWM2 ,  //����ΪPWM2���ģʽ
               TIM2_OUTPUTSTATE_ENABLE , //���ʹ��
               (31250 / Fpwm2 / 2)  ,   //������Ϊ0
=======
  TIM2_TimeBaseInit(TIM2_PRESCALER_512 , //16Mhz / 512 = 31250 HZ
                     (31250 / Fpwm2)      //自动重载值
                    );
  
  TIM2_OC3Init(TIM2_OCMODE_PWM2 ,  //设置为PWM2输出模式
               TIM2_OUTPUTSTATE_ENABLE , //输出使能
               0  ,   //先设置为0
>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
               TIM2_OCPOLARITY_HIGH //OC1 HI
               );

    TIM2_OC3PreloadConfig(ENABLE);
    TIM2_ARRPreloadConfig(ENABLE);
    TIM2_CCxCmd(TIM2_CHANNEL_3 , DISABLE);  

   
}
                 


void StartMotor2()
{
    GPIO_WriteHigh(GPIOC , GPIO_PIN_2);
    delay(10);

    TIM2_CCxCmd(TIM2_CHANNEL_3 , ENABLE);  
    TIM2_Cmd(ENABLE);
    TIM2_ITConfig(TIM2_IT_UPDATE , ENABLE);

}


void StopMotor2()
{
    GPIO_WriteLow(GPIOC , GPIO_PIN_2);
    TIM2_CCxCmd(TIM2_CHANNEL_3 , DISABLE);  
    TIM2_Cmd(DISABLE);
    TIM2_ITConfig(TIM2_IT_UPDATE , DISABLE);

}

/*******************************************************************************
**函数名称：void EEPROM_Byte_Write(unsigned int address , unsigned char date)
**功能描述：向EEPROM中固定地址写入一个字节数据
**入口参数：unsigned int address , unsigned char date
            address  ：要写入数据的存储地址
              date   ：一个字节数据
**输出：无
*******************************************************************************/
void EEPROM_Byte_Write(unsigned int address , unsigned char date)
{
  eepromaddress = address;
  
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_TPROG);              //设定编程时间为标准编程时间
  
  //MASS 密钥，解除EEPROM的保护
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  
  FLASH_ProgramByte(address , date);  //把数据写入相应的存储地址
 
 while(FLASH_GetFlagStatus(FLASH_FLAG_EOP) == SET);     //等待编程结束
}





void main(void)
{
  u32 keeptime = 0;
  disableInterrupts();  //关闭系统总中断

  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
#if 0
   number[3] = FLASH_ReadByte(eepromaddress);         
#endif
    
    NumericDisplay_Init();
    KEYInit();
    BEEP_Init(BEEP_FREQUENCY_2KHZ);
    
    HT1632C_Init();
    HT1632C_clr();
    display_margin(g_margin);
    display_width(g_width);
    display_height(g_height);
    
    
    MotorInit();

    TIM1_PWM_Init();
  //  ForwardMotor1();
  //  StartMotor1();
    
    
    TIM2_PWM_Init();
    
    //StartMotor2();
    
  
    enableInterrupts(); //打开系统总中断
  /* Infinite loop */
    while (1)
    {

      
        //margin Down
        if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_7) != RESET)      //如何KEY1被按下
        {
            keeptime = 0;
            delay(10);                     //先延时进行消抖
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_7) != RESET)  //等待按钮被松开
            {
                keeptime++;
                if((keeptime >100000) && ( keeptime%10 == 0))
                {
                    if(g_margin > 0) g_margin--;
                    display_margin(g_margin);
                }
            };    
            BEEP_Cmd(DISABLE);;
            delay(10);                     //再次延时消抖
            if(g_margin > 0) g_margin--;
            display_margin(g_margin);
        }
   
        //margin UP
        if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_6) != RESET)      //如何KEY1被按下
        {
            keeptime = 0;
            delay(10);                     //先延时进行消抖
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_6) != RESET)    //等待按钮被松开
            {
                keeptime++;
                if((keeptime >100000) && ( keeptime%10 == 0))
                {
                    if(g_margin < 9999) g_margin++;
                    display_margin(g_margin);
                }
                
            }; 
            BEEP_Cmd(DISABLE);;
            delay(10);                     //再次延时消抖
            if(g_margin < 9999) g_margin++;
            display_margin(g_margin);
        }
     
        if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_5) != RESET)      //如何KEY1被按下
        {
            keeptime = 0;
            delay(10);                     //先延时进行消抖
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_5) != RESET)    //等待按钮被松开
            {
                keeptime++;
                if((keeptime >100000) && ( keeptime%10 == 0))
                {
                    if(g_width > 0) g_width--;
                    display_width(g_width);
                }
                
           }; 
          BEEP_Cmd(DISABLE);;
          delay(10);                     //再次延时消抖
          if(g_width > 0) g_width--;
          display_width(g_width);
        }
     
        if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_3) != RESET)      //如何KEY1被按下
        {
            keeptime = 0;
            delay(10);                     //先延时进行消抖
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_3) != RESET)    //等待按钮被松开
            {
                keeptime++;
                if((keeptime >100000) && ( keeptime%10 == 0))
                {
                    if(g_width < 9999) g_width++;
                    display_width(g_width);
                }
                
            };   
            BEEP_Cmd(DISABLE);;
            delay(10);                     //再次延时消抖
            if(g_width < 9999) g_width++;
            display_width(g_width);
        }
      
        if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_2) != RESET)      //如何KEY1被按下
        {
            keeptime = 0;
            delay(10);                     //先延时进行消抖
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_2) != RESET)    //等待按钮被松开
            {
                keeptime++;
                if((keeptime >100000) && ( keeptime%10 == 0))
                {
                    if(g_height < 9999) g_height++;
                    display_height(g_height);
                }
                
            }; 
            BEEP_Cmd(DISABLE);;
            delay(10);                     //再次延时消抖
            if(g_height < 9999) g_height++;
            display_height(g_height);
        }
   
        if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_0) != RESET)      //如何KEY1被按下
        {
            keeptime = 0;
            delay(10);                     //先延时进行消抖
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_0) != RESET)    //等待按钮被松开
            {
                keeptime++;
                if((keeptime >100000) && ( keeptime%10 == 0))
                {
                    if(g_height > 0) g_height--;
                    display_height(g_height);
                }
                
            }; 
            BEEP_Cmd(DISABLE);;
            delay(10);                     //再次延时消抖
            if(g_height > 0) g_height--;
            display_height(g_height);
        }
<<<<<<< HEAD
        
        //ȷ��
        if(GPIO_ReadInputPin(GPIOC , GPIO_PIN_7) != RESET)      //���KEY1������
=======
#if 0        
        if(GPIO_ReadInputPin(GPIOC , GPIO_PIN_7) != RESET)      //如何KEY1被按下
>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
        {
            delay(10);                     //先延时进行消抖
            BEEP_Cmd(ENABLE);
<<<<<<< HEAD
            while(GPIO_ReadInputPin(GPIOC , GPIO_PIN_7) != RESET);    //�ȴ���ť���ɿ�            
            BEEP_Cmd(DISABLE);;
            delay(10);                     //�ٴ���ʱ����

            //����1��ת
            MarginPulse = (int)(((g_margin + 10) * 0.3667 / 1.8) * MOTOR_DIV);
            WidthPulse = (int)(((g_width) * 0.3667 / 1.8) * MOTOR_DIV);  
            TotalWidthPulse = (int)(((g_margin + 10 + g_width) * 0.3667 / 1.8) * MOTOR_DIV);            
            
            ForwardBackwardNum = g_height / 16;
            ForwardBackwardCur = 0;

            MovePulse = MarginPulse; //��һ���ƶ�������롣            
            ForwardMotor1();
            StartMotor1();
            
        }
     
        //ȡ��
        if(GPIO_ReadInputPin(GPIOC , GPIO_PIN_6) != RESET)      //���KEY1������
=======
            while(GPIO_ReadInputPin(GPIOC , GPIO_PIN_7) != RESET);    //等待按钮被松开
            BEEP_Cmd(DISABLE);;
            delay(10);                     //再次延时消抖
        }
      
        if(GPIO_ReadInputPin(GPIOC , GPIO_PIN_6) != RESET)      //如何KEY1被按下
>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
        {
            delay(10);                     //先延时进行消抖
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOC , GPIO_PIN_6) != RESET);    //等待按钮被松开
            BEEP_Cmd(DISABLE);;
<<<<<<< HEAD
            delay(10);                     //�ٴ���ʱ����
            
            bCancel = true;
=======
            delay(10);                     //再次延时消抖
>>>>>>> b96a279dd7bbb5a2e6255f032afe2d28e7c8dc8b
        }
#if 0 
#endif
        
    }
    

  
}




#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/