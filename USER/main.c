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
//ȫ�ֱ���
//u8 number[4] ;
u16 eepromaddress = 0x4000;   //��EEPROM���׵�ַΪ0X4000
u16 totalHeightAddress = 0x4000;   //������Ϊֹ���ܵó��ȵ�ַ
u16 objectHeightAddress = 0x4004;  //�趨��Ŀ�곤�ȵ�ַ
u16 modeAddress = 0x4008;          //����ģʽ�ĵ�ַ

u16  g_margin = 200;
u16  g_width = 500;
u16  g_height = 160;
volatile int XXX = 36;     //�п��ƶ��ܵ�����������Ŀ
volatile int MarginPulse = 0;  //ҳ�߾�����������Ŀ Ҫ����10��
volatile int WidthPulse = 0;  //ҳ�߾�����������Ŀ Ҫ����10��
volatile  int MovePulse = 0;  //�����ƶ�����
volatile int TotalWidthPulse = 0;

volatile  int ForwardBackwardNum = 0; //�ܹ����صĴ���
volatile  int ForwardBackwardCur = 0; //���ڵĴ���

volatile  bool bCancel = FALSE; //���ڵĴ���

volatile bool bSensorON = FALSE; //���ڵĴ������Ƿ񴥷�
volatile bool bBackward = FALSE; //�Ƿ�Ϊ��ת��TURE�Ƿ�ת��False����ת
volatile bool bBackHP = FALSE; //�Ƿ���Ҫ���е�һ�ε�HPλ��

/* Private defines -----------------------------------------------------------*/
#define MOTOR_DIV   (4)

/* Private functions ---------------------------------------------------------*/
void delay(unsigned int ms)
{
    unsigned int x , y;
    for(x = ms; x > 0; x--)           /*  ͨ��һ������ѭ��������ʱ*/
        for(y = 3000 ; y > 0 ; y--);
}

void NumericDisplay_Init()
{
    //����PA1Ϊ��� ��HC164 CLK
    GPIO_Init(GPIOB , GPIO_PIN_4 , GPIO_MODE_OUT_PP_LOW_FAST);  
    
    //����PA2Ϊ��� ��HC164DAT
    GPIO_Init(GPIOB , GPIO_PIN_5 , GPIO_MODE_OUT_PP_LOW_FAST);  
    
    //����PC3Ϊ��� ��λ����
    GPIO_Init(GPIOB , GPIO_PIN_7 , GPIO_MODE_OUT_PP_LOW_FAST); 
}

/*******************************************************************************
key Init
*******************************************************************************/
/*******************************************************************************
**�������ƣ�void KEYInit()
**�����������������밴��
**��ڲ�������
**�������
*******************************************************************************/
void BeepInit()
{
    BEEP_Init(BEEP_FREQUENCY_2KHZ);
}

void KEYInit()
{
    //GPD->PIN7 ����Ϊ����ģʽ , ������������ , ��ֹ�ⲿ�ж�
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

//����Դ��ʼ��
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
    //��ŷ��ĳ�ʼ��
    GPIO_Init(GPIOE , GPIO_PIN_5 , GPIO_MODE_OUT_PP_LOW_SLOW); //solenoid valve PE5
}

/*******************************************************************************
**�������ƣ�void UpValve()
**�������������ߵ�ŷ�
**��ڲ�������
**�������
*******************************************************************************/
void UpValve()
{   
    GPIO_WriteHigh(GPIOE , GPIO_PIN_5);
    delay(500);
}
/*******************************************************************************
**�������ƣ�void DownValve()
**�������������µ�ŷ�
**��ڲ�������
**�������
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
**�������ƣ�void TIM1_PWM_Init()
**������������ʱ��1 PWM��ʼ�� Mortor1
**��ڲ�������
**�������
*******************************************************************************/
int    Fpwm = 400;            //150HZ

void TIM1_PWM_Init()
{
    TIM1_TimeBaseInit(1599 , //16Mhz / 1600 = 10000 HZ
                      TIM1_COUNTERMODE_UP , //���ϼ���
                      10000/Fpwm,      //�Զ�����ֵ
                      0
                          );
    
    TIM1_OC3Init(TIM1_OCMODE_PWM2 ,  //����ΪPWM2���ģʽ
                 TIM1_OUTPUTSTATE_ENABLE , //���ʹ��
                 TIM1_OUTPUTNSTATE_DISABLE,
                 0  ,   //������Ϊ0
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
**�������ƣ�void TIM2_PWM_Init()
**������������ʱ��1 PWM��ʼ�� Mortor2
**��ڲ�������
**�������
*******************************************************************************/
#define     Fpwm2          200            //50HZ
void TIM2_PWM_Init()
{
    TIM2_TimeBaseInit(TIM2_PRESCALER_512 , //16Mhz / 512 = 31250 HZ
                      (31250 / Fpwm2)      //�Զ�����ֵ
                          );
    
    TIM2_OC3Init(TIM2_OCMODE_PWM2 ,  //����ΪPWM2���ģʽ
                 TIM2_OUTPUTSTATE_ENABLE , //���ʹ��
                 (31250 / Fpwm2 / 2)  ,   //������Ϊ0
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
**�������ƣ�void TIM4_Init()
**������������ʱ��4������ʼ��
**��ڲ�������
**�������
*******************************************************************************/
void TIM4_Init()
{
    TIM4_TimeBaseInit(TIM4_PRESCALER_128,    //��ʱ��4Ԥ��Ƶ��Ϊ 16 ��Ƶ������ʱ��ʱ�� = ϵͳʱ�� = 16MHz/128=125 000
                      125                 //����1����ʱ���Զ����� 125
                          );
    TIM4_ITConfig(TIM4_IT_UPDATE , ENABLE); //ʹ�ܶ�ʱ��2�����ж�
    TIM4_ARRPreloadConfig(ENABLE);          //�����Զ��ٲ�
    TIM4_Cmd(ENABLE);        //������ʱ��2��ʼ����
}


/*******************************************************************************
**�������ƣ�void EEPROM_Byte_Write(unsigned int address , unsigned char date)
**������������EEPROM�й̶���ַд��һ���ֽ�����
**��ڲ�����unsigned int address , unsigned char date
address  ��Ҫд�����ݵĴ洢��ַ
date   ��һ���ֽ�����
**�������
*******************************************************************************/
void EEPROM_Byte_Write(unsigned int address , unsigned char date)
{
 //   eepromaddress = address;
    
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_TPROG);              //�趨���ʱ��Ϊ��׼���ʱ��
    
    //MASS ��Կ�����EEPROM�ı���
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    
    FLASH_ProgramByte(address , date);  //������д����Ӧ�Ĵ洢��ַ
    
    while(FLASH_GetFlagStatus(FLASH_FLAG_EOP) == SET);     //�ȴ���̽���
}

/*******************************************************************************
**�������ƣ�void Write_Total_Height()
**�����������������ܵó���д�뵽eeprom����ȥ
**��ڲ�������
**�������
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
**�������ƣ�void Write_Object_Height()
**������������Ŀ���ܵó���д�뵽eeprom����ȥ
**��ڲ�������
**�������
*******************************************************************************/
void Write_Object_Height(u16 data)
{
    u8 low,high = 0;
    low = data % 256;
    high = data / 256;
    EEPROM_Byte_Write(objectHeightAddress,low);
    EEPROM_Byte_Write(objectHeightAddress+1,high);
    
}


void main(void)
{
    u16 totalHeight = 0;
    u8 objectHeight = 0;
    u32 keeptime = 0;
    u8 mode = 0; //0��trial; 1: normal  2: error 3:input mode
    disableInterrupts();  //�ر�ϵͳ���ж�
    
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);

    totalHeight = (FLASH_ReadByte(totalHeightAddress+1) < 8) + FLASH_ReadByte(totalHeightAddress);
    objectHeight = (FLASH_ReadByte(objectHeightAddress+1) < 8) + FLASH_ReadByte(objectHeightAddress);
    mode = FLASH_ReadByte(modeAddress);
        
    //����Դ��ʼ�����������ߴ���
    PowerInit();    
    PowerOFF();
    delay(1000);  
    PowerON();
    
    NumericDisplay_Init();
    KEYInit();
    BEEP_Init(BEEP_FREQUENCY_2KHZ);
    
    //����ܳ�ʼ��
    HT1632C_Init();
    HT1632C_clr();
    
    if(mode == ERROR){ //����ģʽ����ʾ�������
        display_margin_error();
    }
    else{
        display_margin(g_margin);
    }
    
    display_width(g_width);
    display_height(g_height);
    
    //��������ʼ��
    SensInit();
    
    //����ʼ��
    MotorInit();
    
    TIM1_PWM_Init();
    
    TIM2_PWM_Init();
    
    //������ʱ���ʼ��
    TIM4_Init();
    
    //��ʼ����ŷ������ҽ��͵�ŷ�
    ValveInit();
    DownValve();
    
    enableInterrupts(); //��ϵͳ���ж�
    /* Infinite loop */
    while (1)
    {
        //margin Down
        if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_7) != RESET)      //�߾�ļ��ż�������
        {
            keeptime = 0;
            delay(10);                     //����ʱ��������
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_7) != RESET)  //�ȴ���ť���ɿ�
            {
                keeptime++;
                if(keeptime >100000){ //�ж��Ƿ�ͬʱ����ȥ
                    if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_6) != RESET){      //�߾�ļӺż������±�����       
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
            delay(10);      //�ٴ���ʱ����
            if(( mode == 0) || (mode == 1)){
                if(g_margin > 0) g_margin--;
                display_margin(g_margin);
            }
        }
        
        //margin UP
        if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_6) != RESET)      //���KEY1������
        {
            keeptime = 0;
            delay(10);                     //����ʱ��������
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_6) != RESET)    //�ȴ���ť���ɿ�
            {
                keeptime++;
                
                if(keeptime >100000){ //�ж��Ƿ�ͬʱ����ȥ
                    if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_7) != RESET){      //�߾�ļӺż������±�����       
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
                delay(10);                     //�ٴ���ʱ����
                if(g_margin < 9999) g_margin++;
                display_margin(g_margin);
            }
        }
        
        if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_5) != RESET)      //���KEY1������
        {
            keeptime = 0;
            delay(10);                     //����ʱ��������
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_5) != RESET)    //�ȴ���ť���ɿ�
            {
                keeptime++;
                if((keeptime >150000) && ( keeptime%30 == 0))
                {
                    if(g_width > 0) g_width--;
                    display_width(g_width);
                }
                
            }; 
            BEEP_Cmd(DISABLE);;
            delay(10);                     //�ٴ���ʱ����
            if(g_width > 0) g_width--;
            display_width(g_width);
        }
        
        if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_3) != RESET)      //���KEY1������
        {
            keeptime = 0;
            delay(10);                     //����ʱ��������
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_3) != RESET)    //�ȴ���ť���ɿ�
            {
                keeptime++;
                if((keeptime >150000) && ( keeptime%30 == 0))
                {
                    if(g_width < 9999) g_width++;
                    display_width(g_width);
                }
                
            };   
            BEEP_Cmd(DISABLE);;
            delay(10);                     //�ٴ���ʱ����
            if(g_width < 9999) g_width++;
            display_width(g_width);
        }
        
        if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_2) != RESET)      //���KEY1������
        {
            keeptime = 0;
            delay(10);                     //����ʱ��������
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_2) != RESET)    //�ȴ���ť���ɿ�
            {
                keeptime++;
                if((keeptime >150000) && ( keeptime%30 == 0))
                {
                    if(g_height < 9999) g_height++;
                    display_height(g_height);
                }
                
            }; 
            BEEP_Cmd(DISABLE);;
            delay(10);                     //�ٴ���ʱ����
            if(g_height < 9999) g_height++;
            display_height(g_height);
        }
        
        if(GPIO_ReadInputPin(GPIOD , GPIO_PIN_0) != RESET)      //���KEY1������
        {
            keeptime = 0;
            delay(10);                     //����ʱ��������
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOD , GPIO_PIN_0) != RESET)    //�ȴ���ť���ɿ�
            {
                keeptime++;
                if((keeptime >150000) && ( keeptime%30 == 0))
                {
                    if(g_height > 0) g_height--;
                    display_height(g_height);
                }
                
            }; 
            BEEP_Cmd(DISABLE);;
            delay(10);                     //�ٴ���ʱ����
            if(g_height > 0) g_height--;
            display_height(g_height);
        }
        
        //ȷ��
        if(GPIO_ReadInputPin(GPIOC , GPIO_PIN_7) != RESET)      //���KEY1������
        {
            delay(10);                     //����ʱ��������
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOC , GPIO_PIN_7) != RESET);    //�ȴ���ť���ɿ�            
            BEEP_Cmd(DISABLE);;
            delay(10);                     //�ٴ���ʱ����
            bCancel = FALSE;
            
            
            
            switch (mode){
            case TRIAL: //trial
            case NORMAL: //normal
                if((mode == TRIAL) && ( totalHeight > objectHeight )){ //�������ȣ��������ģʽ
                    display_margin_error(); //��ʾ����ģʽ
                    mode = ERROR;
                    EEPROM_Byte_Write(modeAddress,mode);
                    break; 
                }
                else if( mode == TRIAL ){
                    totalHeight = g_height + totalHeight;
                    Write_Total_Height(totalHeight); //�ѳ���д�뵽EEPROM����ȥ
                }
                
                //�����������Ĵ򽺳���
                //���1��ת
                MarginPulse = (int)(((g_margin + 10) * 0.3667 / 1.8) * MOTOR_DIV);
                WidthPulse = (int)(((g_width) * 0.3667 / 1.8) * MOTOR_DIV);  
                TotalWidthPulse = (int)(((g_margin + 10 + g_width) * 0.3667 / 1.8) * MOTOR_DIV);  
                //���ߵ�ŷ�
                UpValve();
                
                delay(50);
                
                ForwardBackwardNum = g_height / 16;
                
                //�жϴ������Ƿ�ΪON
                if(!bSensorON){  //���������û��ON���������ʱ��ͷ����Homeλ�ã�Ҫ���Ƶ�Homeλ��
                    
                    //��ﷴת
                    MovePulse = TotalWidthPulse; //��һ���ƶ������롣            
                    ReverseMotor1();
                    StartMotor1();     
                    bBackHP = TRUE;
                }
                else{            
                    ForwardBackwardCur = 0;
                    
                    MovePulse = MarginPulse; //��һ���ƶ������롣            
                    ForwardMotor1();
                    StartMotor1();
                    bBackHP = FALSE;
                }
                
                break;
            case ERROR: //error
                //Ҫ��������ıȶԣ��������ȷ�ģ��Ͱ�ģʽ�ĳ�trial���趨��eeprom����ȥ
                
                
                break;
            case INPUT: //input
                //��height���뵽eeprom����ȥ
                Write_Object_Height(g_height);                
                //���������ǳ������룬�Ͱ�ģʽ�ĳ�1���趨��eeprom����ȥ
                if( g_width == SUPPER_PASSWD ){
                    EEPROM_Byte_Write(modeAddress,NORMAL);
                    mode = NORMAL;
                    
                }
                
                break;
            default:
                break;
         
            
            }
            
        }
        
        //ȡ��
        if(GPIO_ReadInputPin(GPIOC , GPIO_PIN_6) != RESET)      //���KEY1������
        {
            delay(10);                     //����ʱ��������
            BEEP_Cmd(ENABLE);
            while(GPIO_ReadInputPin(GPIOC , GPIO_PIN_6) != RESET);    //�ȴ���ť���ɿ�
            BEEP_Cmd(DISABLE);;
            delay(10);                     //�ٴ���ʱ����
            
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