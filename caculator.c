#include "common.h"
#include "timer.h"

#define keypad P1
#define lcd P2

sbit RS = P3^0; sbit E = P3^1;

char get_key(unsigned int* cus_pos, int* determine,int* result, int* number,char* sign,int* ans, int* error);
void command_lcd( char command);
void display(char string);
void display_string(char* string);
void convert_string(int result, char* str);
void caculate(int* result, int* number,char* sign, int* error);
void clear(int* ans,unsigned int* cus_pos);

void main()
{
	int result = 0; int number = 0; int determine = 0; char str[16]; char sign[1]; int ans = 0; int error = 0;
	unsigned int cus_pos = 0x80;
	command_lcd(0x38); command_lcd(0x0C);
	while(1) 
		{
			command_lcd(cus_pos);
			display(get_key(&cus_pos,&determine,&result,&number,sign,&ans, &error));
			command_lcd(0xC0);
			if(error == 1) 
			{
				command_lcd(0x01);
				determine = 0;
				command_lcd(0xC5);
				display_string("Error");
			}
			else if(determine == 1)
			{
				convert_string(result, str); 
				display_string(str);
			}
		}
}

char get_key(unsigned int* cus_pos, int* determine,int* result, int* number,char* sign, int* ans, int* error)
{
	unsigned char key = 0; 
	keypad = 0xFE;
	if(keypad == 0xEE  )
	{
		delay(50);
		key =  '7'; 
		*number =  (*number * 10) +7;
		*cus_pos += 1; 
	}
	if(keypad == 0xDE  )
	{
		delay(50);
		key = '8';
		*number =  (*number * 10) +8;
		*cus_pos += 1 ;
	}
	if(keypad == 0xBE) 
	{
		delay(50);
		key = '9';
		*number =  (*number * 10) +9;
		*cus_pos += 1;
	}
	if(keypad == 0x7E) 
	{
		delay(50);
		key = '/';
		caculate(result,number,sign, error);
		sign[0] = '/';
		*number = 0; 
		clear(ans,cus_pos);
		*cus_pos +=1;
	}
	keypad = 0xFC;
	if(keypad == 0xEC )
	{
		delay(50);
		key = '4';
		*number =  (*number * 10) +4;
		*cus_pos += 1 ;
	}
	if(keypad == 0xDC )
	{
		delay(50);
		key = '5';
		*number =  (*number * 10) +5;
		*cus_pos += 1 ;
	}
	if(keypad == 0xBC)
	{
		delay(50);
		key = '6';
		*number =  (*number * 10) +6;
		*cus_pos += 1 ;
	}
	if(keypad == 0x7C )
	{
		delay(50); 
		key = 'x';
		caculate(result,number,sign,error);
		sign[0] = 'x';
		*number = 0;
		clear(ans,cus_pos);
		*cus_pos +=1;
	}
	keypad = 0xFB;
	if(keypad == 0xEB )
	{
		delay(50);
		key = '1';
		*number =  (*number * 10) +1;
		*cus_pos += 1 ;
	}
	if(keypad == 0xDB )
	{
		delay(50);
		key = '2';
		*number =  (*number * 10) +2;
		*cus_pos += 1 ;
	}
	if(keypad == 0xBB )
	{
		delay(50);
		key = '3';
		*number =  (*number * 10) +3;
		*cus_pos += 1 ;
	}
	if(keypad == 0x7B )
	{
		delay(50); 
		key = '-';
		caculate(result,number,sign,error);
		sign[0] = '-';
		*number = 0;
		clear(ans,cus_pos);
		*cus_pos +=1;
	}
	keypad = 0xF7;
	if(keypad == 0xE7 )
	{
		delay(50);
		command_lcd(0x01);
		*determine = 0;
		*result = 0; *number = 0; *ans = 0; *error = 0;
		*cus_pos = 0x80;
	}
	if(keypad == 0xD7)
	{
		delay(50);
		key = '0';
		*number =  (*number * 10);
		*cus_pos += 1 ;
	}
	if(keypad == 0xB7 )// = 
	{
		delay(50);
		*determine = 1;
		caculate(result,number,sign, error);
		*number = 1;
		*ans = 1;
	}
	if(keypad == 0x77 )
	{
		delay(50); 
		key = '+'; 
		caculate(result,number,sign, error);
		sign[0] = '+';
		*number = 0; 
		clear(ans,cus_pos);
		*cus_pos += 1 ;
	}
	return key;
}


void command_lcd(unsigned char command)
{
	RS = 0;
	E = 1;
	lcd = command;
	delay(1);
	E = 0;
}

void display(unsigned char string)
{
	RS = 1;
	E = 1;
	lcd = string;
	delay(1);
	E = 0;
}

void display_string(unsigned char* string)
{
	unsigned int i;
	for(i=0;string[i] != '\0';++i)
	{
		display(string[i]);
	}
}

void convert_string(int result, char* str)
{
		sprintf(str, "%d", result);
}

void caculate(int* result, int* number,char* sign, int* error)
{
	if(*number == 0 && sign[0] != 'x' && sign[0] != '/') {*result += *number;}
	else if(*result == 0 && sign[0] != 'x')   {*result += *number ;}
	else if(*result != 0 && sign[0] == '/' && *number == 0) {*error = 1;}
	else if(sign[0] == '/') {*result /= *number;} 
	else if(sign[0] == '+') {*result += *number;} 
	else if(sign[0] == 'x') {*result *= *number;} 
	else if(sign[0] == '-')	{*result -= *number;}; 
	
}

void clear(int* ans, unsigned int* cus_pos)
{
	if(*ans == 1)
	{
		command_lcd(0x01);
		*ans = 0;
		*cus_pos = 0x80;
	}
}

