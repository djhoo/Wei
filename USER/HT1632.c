#include "HT1632.h"

#define     CLK_Set()     GPIO_WriteHigh(GPIOA , GPIO_PIN_1) 
#define     CLK_ReSet()   GPIO_WriteLow(GPIOA , GPIO_PIN_1) 

#define     DAT_Set()     GPIO_WriteHigh(GPIOA , GPIO_PIN_2) 
#define     DAT_ReSet()   GPIO_WriteLow(GPIOA , GPIO_PIN_2) 

#define     HT_WR       GPIO_PIN_5
#define     HT_DATA     GPIO_PIN_4
#define     HT_CS       GPIO_PIN_7

#define     GPIOEX_BitSet(a)    GPIO_WriteHigh(GPIOB , a)
#define     GPIOEX_BitClr(a)    GPIO_WriteLow(GPIOB , a) 

//ָ���
#define SYS_DIS	0x00	//�ر�ϵͳ�� 
#define SYS_EN	0x01	//��ϵͳ�� 
#define LED_OFF	0x02	//�ر�LED��ʾ 
#define LED_ON	0x03	//��LED��ʾ 
#define BLINK_OFF	0x08	//�ر���˸ 
#define	BLINK_ON	0x09	//����˸ 
#define	SLAVE_MODE	0x10	//��ģʽ 
#define	RC_MASTER_MODE	0x18	//�ڲ�RCʱ�� 
#define	EXT_CLK_MASTER_MODE	0x1C //�ⲿʱ�� 
#define COM_OPTION	0x24	//16COM��NMOSģʽ 
//#define PWM_DUTY	0XA0	//PWM���ȿ��� 
#define PWM_DUTY	0xAf	//PWM���ȿ��� 
//����ܶ�ѡ dp-a��ʽ:0-9,A,b,c,u 
unsigned char tab[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x3e}; 

/**************************************** 
*������:HT1632C_Writer 
*�� ��:HT1632C�������ݺ�������λ��ǰ 
*�� ��ata,cnt 
*�� ��: 
****************************************/ 
void HT1632C_Writer(unsigned char Data,unsigned char cnt)	
{ 
    unsigned char i; 
    for(i=0;i<cnt;i++) 
    { 
        GPIOEX_BitClr(HT_WR); 
        if(Data&0x80) 
            GPIOEX_BitSet(HT_DATA); 
        else 
            GPIOEX_BitClr(HT_DATA); 
        Data<<=1; 
        GPIOEX_BitSet(HT_WR); 
    } 
} 

/******************************* 
*������:HT1632C_Writer_CMD 
*�� ��:��������� 
*�� ��:cmd 
*�� ��: 
********************************/ 
void HT1632C_Writer_CMD(unsigned char cmd)	
{ 
    GPIOEX_BitClr(HT_CS);	
    HT1632C_Writer(0x80,3);    //������100 
    HT1632C_Writer(cmd,9);	   //�������ݣ���9λΪ0 
    GPIOEX_BitSet(HT_CS); 
} 

/******************************* 
*������:HT1632C_Writer_DATA 
*�� ��:ָ����ַд������ 
*�� ��:Addr,Data 
*�� ��: 
********************************/ 
void HT1632C_Writer_DATA(unsigned char Addr,unsigned char Data) 
{ 
    GPIOEX_BitClr(HT_CS); 
    HT1632C_Writer(0xa0,3);	    //������101 
    HT1632C_Writer(Addr<<1,7);	//д��7λ��ַ 
    HT1632C_Writer(Data,4);	    //д��4λ���� 
    GPIOEX_BitSet(HT_CS);	
} 

/************************************************************ 
*������:HT1632C_Writer_AllDATA 
*�� ��:ָ����ַд���������ݣ�cntΪд�����ݸ�������8λ�� 
*�� ��:Addr,*p,cnt 
*�� ��: 
*************************************************************/ 
void HT1632C_Writer_AllDATA(unsigned char Addr,unsigned char *p,unsigned char cnt) 
{ 
    unsigned char i; 
    GPIOEX_BitClr(HT_CS);	
    HT1632C_Writer(0xa0,3); 
    HT1632C_Writer(Addr<<1,7); 
    for(i=0;i<cnt;i++) 
    { 
        HT1632C_Writer(*p,8); 
        p++; 
    } 
    GPIOEX_BitSet(HT_CS); 
} 

/******************************* 
*������:HT1632C_clr 
*�� ��:�������� 
*�� ��: 
*�� ��: 
********************************/ 
void HT1632C_clr(void)	
{ 
    unsigned char i; 
    GPIOEX_BitClr(HT_CS); 
    HT1632C_Writer(0xa0,3); 
    HT1632C_Writer(0x00<<1,7); 
    for(i=0;i<36;i++) 
    { 
        HT1632C_Writer(0,8);
	} 
    GPIOEX_BitSet(HT_CS); 
} 

/******************************* 
*������:HT1632C_Init 
*�� ��:HT1632C��ʼ������ 
*�� ��: 
*�� ��: 
********************************/ 
void HT1632C_Init(void)	
{ 
    GPIOEX_BitSet(HT_CS); 
    GPIOEX_BitSet(HT_WR); 
    GPIOEX_BitSet(HT_DATA); 
    HT1632C_Writer_CMD(SYS_DIS); 
    HT1632C_Writer_CMD(COM_OPTION); 
    HT1632C_Writer_CMD(RC_MASTER_MODE); 
    HT1632C_Writer_CMD(SYS_EN); 
    HT1632C_Writer_CMD(PWM_DUTY); 
    HT1632C_Writer_CMD(BLINK_OFF); 
    HT1632C_Writer_CMD(LED_ON); 
} 

/******************************* 
*������:display_margin 
*�� ��:�����ҳ�߾���ʾ���� 
*�� ��: 
*�� ��: 
********************************/ 
//��ʾ��ֵ����С���� 
void display_margin(unsigned int dat) 
{ 
    unsigned char i; 
    unsigned char dt;    //dtΪҪд�뵽ram��ֵ 
    dt = 0; 
    for(i=0; i<8;i++) 
    { 
        dt =( tab[dat/1000]&(1<<i))<<(7-i);//ǧ 
        if(i==7) 
        { 
            dt +=(tab[dat%1000/100 ]&(1<<i))>>(i-6);//�� 
            dt += (tab[dat%100/10 ]&(1<<i))>>(i-5);//ʮ 
            dt += (tab[dat%10]&(1<<i))>>(i-4); //�� 
        } 
        else if(i == 6) 
        { 
            dt +=(tab[dat%1000/100 ]&(1<<i))>>(i-6); 
            dt += (tab[dat%100/10]&(1<<i))>>(i-5); 
            dt += (tab[dat%10]&(1<<i))>>(i-4); 
        } 
        else if(i == 5) 
        { 
            dt+=(tab[dat%1000/100 ]&(1<<i))<<(6-i);//�� 
            dt += (tab[dat%100/10]&(1<<i))<<(5-i); 
            dt += (tab[dat%10]&(1<<i))>>(i-4); 
        } 
        else 
        { 
            dt+=(tab[dat%1000/100 ]&(1<<i))<<(6-i);//�� 
            dt += (tab[dat%100/10]&(1<<i))<<(5-i); 
            dt += (tab[dat%10]&(1<<i))<<(4-i); 
        } 
        HT1632C_Writer_DATA((0x00+i*4),dt); 
    } 
}

/******************************* 
*������:display_margin_error 
*�� ��:�����ҳ�߾���ʾError���� 
*�� ��: 
*�� ��: 
********************************/ 
//��ʾ��ֵ����С���� 
void display_margin_error() 
{ 
    unsigned char i; 
    unsigned char dt;    //dtΪҪд�뵽ram��ֵ
    unsigned char dat = 0x40;//��ʾһ�����
    dt = 0; 
    for(i=0; i<8;i++) 
    { 
        dt =( dat&(1<<i))<<(7-i);//ǧ 
        if(i==7) 
        { 
            dt +=(dat&(1<<i))>>(i-6);//�� 
            dt += (dat&(1<<i))>>(i-5);//ʮ 
            dt += (dat&(1<<i))>>(i-4); //�� 
        } 
        else if(i == 6) 
        { 
            dt +=(dat&(1<<i))>>(i-6); 
            dt += (dat&(1<<i))>>(i-5); 
            dt += (dat&(1<<i))>>(i-4); 
        } 
        else if(i == 5) 
        { 
            dt+=(dat&(1<<i))<<(6-i);//�� 
            dt += (dat&(1<<i))<<(5-i); 
            dt += (dat&(1<<i))>>(i-4); 
        } 
        else 
        { 
            dt+=(dat&(1<<i))<<(6-i);//�� 
            dt += (dat&(1<<i))<<(5-i); 
            dt += (dat&(1<<i))<<(4-i); 
        } 
        HT1632C_Writer_DATA((0x00+i*4),dt); 
    } 
}

/******************************* 
*������:display_margin_error 
*�� ��:�����ҳ�߾���ʾError���� 
*�� ��: 
*�� ��: 
********************************/ 
//��ʾ��ֵ����С���� 
void display_margin_input() 
{ 
    unsigned char i; 
    unsigned char dt;    //dtΪҪд�뵽ram��ֵ
    unsigned char dat = 0x49;//��ʾһ�����
    dt = 0; 
    for(i=0; i<8;i++) 
    { 
        dt =( dat&(1<<i))<<(7-i);//ǧ 
        if(i==7) 
        { 
            dt +=(dat&(1<<i))>>(i-6);//�� 
            dt += (dat&(1<<i))>>(i-5);//ʮ 
            dt += (dat&(1<<i))>>(i-4); //�� 
        } 
        else if(i == 6) 
        { 
            dt +=(dat&(1<<i))>>(i-6); 
            dt += (dat&(1<<i))>>(i-5); 
            dt += (dat&(1<<i))>>(i-4); 
        } 
        else if(i == 5) 
        { 
            dt+=(dat&(1<<i))<<(6-i);//�� 
            dt += (dat&(1<<i))<<(5-i); 
            dt += (dat&(1<<i))>>(i-4); 
        } 
        else 
        { 
            dt+=(dat&(1<<i))<<(6-i);//�� 
            dt += (dat&(1<<i))<<(5-i); 
            dt += (dat&(1<<i))<<(4-i); 
        } 
        HT1632C_Writer_DATA((0x00+i*4),dt); 
    } 
}

/******************************* 
*������:display_width 
*�� ��:������п�����ʾ���� 
*�� ��: 
*�� ��: 
********************************/ 
//��ʾ��ֵ����С���� 
void display_width(unsigned int dat) 
{ 
    unsigned char i; 
    unsigned char dt;    //dtΪҪд�뵽ram��ֵ 
    dt = 0; 
    for(i=0; i<8;i++) 
    { 
        dt =( tab[dat/1000]&(1<<i))<<(7-i);//ǧ 
        if(i==7) 
        { 
            dt +=(tab[dat%1000/100 ]&(1<<i))>>(i-6);//�� 
            dt += (tab[dat%100/10 ]&(1<<i))>>(i-5);//ʮ 
            dt += (tab[dat%10]&(1<<i))>>(i-4); //�� 
        } 
        else if(i == 6) 
        { 
            dt +=(tab[dat%1000/100 ]&(1<<i))>>(i-6); 
            dt += (tab[dat%100/10]&(1<<i))>>(i-5); 
            dt += (tab[dat%10]&(1<<i))>>(i-4); 
        } 
        else if(i == 5) 
        { 
            dt+=(tab[dat%1000/100 ]&(1<<i))<<(6-i);//�� 
            dt += (tab[dat%100/10]&(1<<i))<<(5-i); 
            dt += (tab[dat%10]&(1<<i))>>(i-4); 
        } 
        else 
        { 
            dt+=(tab[dat%1000/100 ]&(1<<i))<<(6-i);//�� 
            dt += (tab[dat%100/10]&(1<<i))<<(5-i); 
            dt += (tab[dat%10]&(1<<i))<<(4-i); 
        } 
        HT1632C_Writer_DATA((0x01+i*4),dt); 
    } 
}
/******************************* 
*������:display_height 
*�� ��:����ܳ�������ʾ���� 
*�� ��: 
*�� ��: 
********************************/ 
//��ʾ��ֵ����С���� 
void display_height(unsigned int dat) 
{ 
    unsigned char i; 
    unsigned char dt;    //dtΪҪд�뵽ram��ֵ 
    dt = 0; 
    for(i=0; i<8;i++) 
    { 
        dt =( tab[dat/1000]&(1<<i))<<(7-i);//ǧ 
        if(i==7) 
        { 
            dt +=(tab[dat%1000/100 ]&(1<<i))>>(i-6);//�� 
            dt += (tab[dat%100/10 ]&(1<<i))>>(i-5);//ʮ 
            dt += (tab[dat%10]&(1<<i))>>(i-4); //�� 
        } 
        else if(i == 6) 
        { 
            dt +=(tab[dat%1000/100 ]&(1<<i))>>(i-6); 
            dt += (tab[dat%100/10]&(1<<i))>>(i-5); 
            dt += (tab[dat%10]&(1<<i))>>(i-4); 
        } 
        else if(i == 5) 
        { 
            dt+=(tab[dat%1000/100 ]&(1<<i))<<(6-i);//�� 
            dt += (tab[dat%100/10]&(1<<i))<<(5-i); 
            dt += (tab[dat%10]&(1<<i))>>(i-4); 
        } 
        else 
        { 
            dt+=(tab[dat%1000/100 ]&(1<<i))<<(6-i);//�� 
            dt += (tab[dat%100/10]&(1<<i))<<(5-i); 
            dt += (tab[dat%10]&(1<<i))<<(4-i); 
        } 
        HT1632C_Writer_DATA((0x02+i*4),dt); 
    } 
}