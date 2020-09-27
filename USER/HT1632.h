#ifndef __HT1632_H
#define __HT1632_H

#include "stm8s.h"



void HT1632C_Writer(unsigned char Data,unsigned char cnt);
void HT1632C_Writer_CMD(unsigned char cmd);
void HT1632C_Writer_DATA(unsigned char Addr,unsigned char Data);
void HT1632C_Writer_AllDATA(unsigned char Addr,unsigned char *p,unsigned char cnt);
void HT1632C_clr(void);
void HT1632C_Init(void);
void display_margin(unsigned int dat);
void display_width(unsigned int dat);
void display_height(unsigned int dat);
void display_margin_error(void);
void display_margin_input(void); 
#endif
