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
#define TRIAL   (0)
#define NORMAL  (1)
#define ERROR   (2)
#define INPUT   (3)
#define SUPPER_PASSWD  (644)

/* Public value  -----------------------------------------------*/
//全局变量
//u8 number[4] ;
u16 eepromaddress = 0x4000;   //设EEPROM的首地址为0X4000
u16 totalHeightAddress = 0x4000;   //到现在为止的总得长度地址
u16 objectHeightAddress = 0x4004;  //设定的目标长度地址
u16 modeAddress = 0x4008;          //现在模式的地址

u16  g_margin = 200;
u16  g_width = 500;
u16  g_height = 160;
volatile int XXX = 36;     //中框移动总得马达的脉冲数目
volatile int MarginPulse = 0;  //页边距马达的脉冲数目 要加上10的
volatile int WidthPulse = 0;  //页边距马达的脉冲数目 要加上10的
volatile  int MovePulse = 0;  //当次移动距离
volatile int TotalWidthPulse = 0;

volatile  int ForwardBackwardNum = 0; //总共来回的次数
volatile  int ForwardBackwardCur = 0; //现在的次数

volatile  bool bCancel = FALSE; //现在的次数

volatile bool bSensorON = FALSE; //现在的传感器是否触发
volatile bool bBackward = FALSE; //是否为反转，TURE是反转，False是正转
volatile bool bBackHP = FALSE; //是否需要进行第一次到HP位置

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

void SensInit()
{
    GPIO_Init(GPIOB , GPIO_PIN_3 , GPIO_MODE_IN_PU_NO_IT);  //Sensor
}

//马达电源初始化
void PowerInit()
{
    GPIO_Init(GPIOB , GPIO_PIN_1 , GPIO_MODE_OUT_PP_LOW_SLOW); 
    GPIO_Init(GPIOB , GPIO_PIN_2 , GPIO_MODE_OUT_PP_LOW_SLOW); 
}

void PowerON()
{
    GPIO_WriteHigh(GPIOB , GPIO_PIN_1);
    GPIO_WriteHigh(GPIOB , GPIO_PIN_2);
}

void PowerOFF()
{
    GPIO_WriteLow(GPIOB , GPIO_PIN_1);
    GPIO_WriteLow(GPIOB , GPIO_PIN_2);
}

/*******************************************************************************
Solenoid Valve Define
*******************************************************************************/
void ValveInit()
{
    //电磁阀的初始化
    GPIO_Init(GPIOE , GPIO_PIN_5 , GPIO_MODE_OUT_PP_LOW_SLOW); //solenoid valve PE5
}

/*******************************************************************************
**函数名称：void UpValve()
**功能描述：拉高电磁阀
**入口参数：无
**输出：无
*******************************************************************************/
void UpValve()
{   
    GPIO_WriteHigh(GPIOE , GPIO_PIN_5);
    delay(500);
}
/*******************************************************************************
**函数名称：void DownValve()
**功能描述：放下电磁阀
**入口参数：无
**输出：无
*******************************************************************************/
void DownValve()
{    
    GPIO_WriteLow(GPIOE , GPIO_PIN_5);
    delay(500);
    
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
                      TIM1_COUNTERMODE_UP , //向上计数
                      10000/Fpwm,      //自动重载值
                      0
                          );
    
    TIM1_OC3Init(TIM1_OCMODE_PWM2 ,  //设置为PWM2输出模式
                 TIM1_OUTPUTSTATE_ENABLE , //输出使能
                 TIM1_OUTPUTNSTATE_DISABLE,
                 0  ,   //先设置为0
                 TIM1_OCPOLARITY_HIGH ,//OC1 HI
                 TIM1_OCNPOLARITY_LOW,
                 TIM1_OCIDLESTATE_SET,
                 TIM1_OCNIDLESTATE_SET
                     );
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
    delay(500);
}

void ForwardMotor1()
{
    GPIO_WriteLow(GPIOC , GPIO_PIN_4);
    bBackward = FALSE;
}

void ReverseMotor1()
{
    GPIO_WriteHigh(GPIOC , GPIO_PIN_4);
    bBackward = TRUE;
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
    TIM2_TimeBaseInit(TIM2_PRESCALER_512 , //16Mhz / 512 = 31250 HZ
                      (31250 / Fpwm2)      //自动重载值
                          );
    
    TIM2_OC3Init(TIM2_OCMODE_PWM2 ,  //设置为PWM2输出模式
                 TIM2_OUTPUTSTATE_ENABLE , //输出使能
                 (31250 / Fpwm2 / 2)  ,   //先设置为0
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
    delay(500);
    
}

/*******************************************************************************
**函数名称：void TIM4_Init()
**功能描述：定时器4参数初始化
**入口参数：无
**输出：无
*******************************************************************************/
void TIM4_Init()
{
    TIM4_TimeBaseInit(TIM4_PRESCALER_128,    //定时器4预分频数为 16 分频，即定时器时钟 = 系统时钟 = 16MHz/128=125 000
                      125                 //设置1毫秒时间自动重载 125
                          );
    TIM4_ITConfig(TIM4_IT_UPDATE , ENABLE); //使能定时器2更新中断
    TIM4_ARRPreloadConfig(ENABLE);          //允许自动仲裁
    TIM4_Cmd(ENABLE);        //启动定时器2开始计数
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
 //   eepromaddress = address;
    
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_TPROG);              //设定编程时间为标准编程时间
    
    //MASS 密钥，解除EEPROM的保护
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    
    FLASH_ProgramByte(address , date);  //把数据写入相应的存储地址
    
    while(FLASH_GetFlagStatus(FLASH_FLAG_EOP) == SET);     //等待编程结束
}

/*******************************************************************************
**函数名称：void Write_Total_Height()
**功能描述：把现在总得长度写入到eeprom里面去
**入口参数：无
**输出：无
*******************************************************************************/
void Write_Total_Height(u16 data)
{
    u8 low,high = 0;
    low = data % 256;
    high = data / 256;
    EEPROM_Byte_Write(totalHeightAddress,low);
    EEPROM_Byte_Write(totalHeightAddress+1,high);
    
}


/*******************************************************************************
**函数名称：void Write_Object_Height()
**功能描述：把目标总得长度写入到eeprom里面去
**入口参数：无
**输出：无
*******************************************************************************/
void Write_Object_Height(u16 data)
{
    u8 low,high = 0;
    low = data % 256;
    high = data / 256;
    EEPROM_Byte_Write(objectHeightAddress,low);
    EEPROM_Byte_Write(objectHeightAddress+1,high);
    
}

/*******************************************************************************
**函数名称：int get_random()
**功能描述：获取随机数
**入口参数：无
**输出：无
*******************************************************************************/
u16 get_random()
{
    u16 tem = 0;
    srand(* ( u16 * ) ( 0x000008 )); 
    tem = rand() % 10000;
    return tem;
}

/*******************************************************************************
**函数名称：int get_passdata()
**功能描述：获取密码数字,如果是0，就是0644,如果是以外的的话，就是数字*0644，然后取得前两位和后两位
**入口参数：u16
**输出：无
*******************************************************************************/
u16 get_passdata(u16 indata)
{
    u16 retData = 0;
    u32 temp = 0,tempHigh = 0,tempLow = 0,temp2 = indata;
    temp = 644 * temp2;
    if(indata == 0){
        retData = 644;
    }
    else{
        tempHigh = temp;
        while(tempHigh>=100)
            tempHigh=tempHigh/10;

        tempLow = temp%100;
        retData = tempHigh*100+tempLow;
    }
    return retData;
}



void main(void)
{
    u16 totalHeight = 0;
    u8 objectHeight = 0;
    u32 keeptime = 0;
    u8 mode = 0; //0：trial; 1: normal  2: error 3:input mode
    u16 random_data = 0;
    disableInterrupts();  //关闭系统总中断
    
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);

    totalHeight = (FLASH_ReadByte(totalHeightAddress+1) < 8) + FLASH_ReadByte(totalHeightAddress);
    objectHeight = (FLASH_ReadByte(objectHeightAddress+1) < 8) + FLASH_ReadByte(objectHeightAddress);
    mode = FLASH_ReadByte(modeAddress);
        
    //马达电源初始化，并且拉高处理
    PowerInit();    
    PowerOFF();
    delay(1000);  
    PowerON();
    
    NumericDisplay_Init();
    KEYInit();
    BEEP_Init(BEEP_FREQUENCY_2KHZ);
    
    //数码管初始化
    HT1632C_Init();
    HT1632C_clr();
    
    if(mode == ERROR){ //错误模式，显示错误编码
        display_margin_error();
        random_data = get_random();
        display_width(random_data);
        display_height(0);
    }
    else{
        display_margin(g_margin);
        display_width(g_width);
        display_height(g_height);
    }
 
    
    //传感器初始化
    SensInit();
    
    //马达初始化
    MotorInit();
    
    TIM1_PWM_Init();
    
    TIM2_PWM_Init();
    
    //传感器时间初始化
    TIM4_Init();
    
    //初始化电磁阀，并且降低电磁阀
    ValveInit();
    DownValve();
    
    enableInterrupts(); //打开系统总中断
    /* Infinite loop */
    while (1)
    {
        //margin Down
        if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_7) != RESET)      //边距的减号键被按下
        {
            keeptime = 0;
            delay(10);                     //先延时进行消抖
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_7) != RESET)  //等待按钮被松开
            {
                keeptime++;
                if(keeptime >100000){ //判断是否同时按下去
                    if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_6) != RESET){      //边距的加号键被按下被按下       
                        delay(10);
                        while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_6) != RESET){
                            keeptime++;
                            if( keeptime >200000 ){
                                display_margin_input();
                                keeptime = 0;
                                mode = INPUT;
                                
                            }
                        }
                    }                    
                }                    
                if(( mode == 0) || (mode == 1)){    
                    if((keeptime >150000) && ( keeptime%30 == 0))
                    {
                        if(g_margin > 0) g_margin--;
                        display_margin(g_margin);
                    }
                }
            };    
            BEEP_Cmd(DISABLE);
            delay(10);      //再次延时消抖
            if(( mode == 0) || (mode == 1)){
                if(g_margin > 0) g_margin--;
                display_margin(g_margin);
            }
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
                
                if(keeptime >100000){ //判断是否同时按下去
                    if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_7) != RESET){      //边距的加号键被按下被按下       
                        delay(10);
                        while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_7) != RESET){
                            keeptime++;
                            if( keeptime >200000 ){
                                display_margin_input();
                                keeptime = 0;
                                mode = INPUT;
                                
                            }
                        }
                    }                    
                }        
                
                if(( mode == 0) || (mode == 1)){    
                    if((keeptime >150000) && ( keeptime%30 == 0))
                    {
                        if(g_margin < 9999) g_margin++;
                        display_margin(g_margin);
                    }
                }
                
            }; 
            BEEP_Cmd(DISABLE);;
            if(( mode == 0) || (mode == 1)){    
                delay(10);                     //再次延时消抖
                if(g_margin < 9999) g_margin++;
                display_margin(g_margin);
            }
        }
        
        if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_5) != RESET)      //如何KEY1被按下
        {
            keeptime = 0;
            delay(10);                     //先延时进行消抖
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_5) != RESET)    //等待按钮被松开
            {
                keeptime++;
                if((keeptime >150000) && ( keeptime%30 == 0))
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
                if((keeptime >150000) && ( keeptime%30 == 0))
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
                if((keeptime >150000) && ( keeptime%30 == 0))
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
                if((keeptime >150000) && ( keeptime%30 == 0))
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
        
        //确认
        if(GPIO_ReadInputPin(GPIOC , GPIO_PIN_7) != RESET)      //如何KEY1被按下
        {
            delay(10);                     //先延时进行消抖
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOC , GPIO_PIN_7) != RESET);    //等待按钮被松开            
            BEEP_Cmd(DISABLE);;
            delay(10);                     //再次延时消抖
            bCancel = FALSE;
            
            
            
            switch (mode){
            case TRIAL: //trial
            case NORMAL: //normal
                if((mode == TRIAL) && ( totalHeight > objectHeight )){ //超出长度，进入错误模式
                    display_margin_error(); //显示错误模式
                    mode = ERROR;
                    EEPROM_Byte_Write(modeAddress,mode);
                    break; 
                }
                else if( mode == TRIAL ){
                    totalHeight = g_height + totalHeight;
                    Write_Total_Height(totalHeight); //把长度写入到EEPROM里面去
                }
                
                //下面是正常的打胶程序
                //马达1正转
                MarginPulse = (int)(((g_margin + 10) * 0.3667 / 1.8) * MOTOR_DIV);
                WidthPulse = (int)(((g_width) * 0.3667 / 1.8) * MOTOR_DIV);  
                TotalWidthPulse = (int)(((g_margin + 10 + g_width) * 0.3667 / 1.8) * MOTOR_DIV);  
                //拉高电磁阀
                UpValve();
                
                delay(50);
                
                ForwardBackwardNum = g_height / 16;
                
                //判断传感器是否为ON
                if(!bSensorON){  //如果传感器没有ON，表明这个时候胶头不在Home位置，要先移到Home位置
                    
                    //马达反转
                    MovePulse = TotalWidthPulse; //第一次移动变宽距离。            
                    ReverseMotor1();
                    StartMotor1();     
                    bBackHP = TRUE;
                }
                else{            
                    ForwardBackwardCur = 0;
                    
                    MovePulse = MarginPulse; //第一次移动变宽距离。            
                    ForwardMotor1();
                    StartMotor1();
                    bBackHP = FALSE;
                }
                
                break;
            case ERROR: //error
                //要进行密码的比对，如果是正确的，就把模式改成trial，设定到eeprom里面去,画面显示4个0
                if (get_passdata(random_data) == g_height){
                    EEPROM_Byte_Write(modeAddress,TRIAL);
                    display_height(0);
                }
                
                break;
            case INPUT: //input
                //把height输入到eeprom里面去
                Write_Object_Height(g_height);                
                //如果输入的是超级密码，就把模式改成1，设定到eeprom里面去,画面显示4个0
                if( g_width == SUPPER_PASSWD ){
                    EEPROM_Byte_Write(modeAddress,NORMAL);
                    display_width(0);                    
                }                
                break;
            default:
                break;
         
            
            }
            
        }
        
        //取消
        if(GPIO_ReadInputPin(GPIOC , GPIO_PIN_6) != RESET)      //如何KEY1被按下
        {
            delay(10);                     //先延时进行消抖
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOC , GPIO_PIN_6) != RESET);    //等待按钮被松开
            BEEP_Cmd(DISABLE);;
            delay(10);                     //再次延时消抖
            
            bCancel = TRUE;
        }
        
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
