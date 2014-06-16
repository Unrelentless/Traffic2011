# MPLAB IDE generated this makefile for use with GNU make.
# Project: TrafficLightsProject.mcp
# Date: Wed Jun 01 14:03:51 2011

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

TrafficLightsProject.cof : delay.o main.o LCD.o
	"C:\Program Files\Microchip\MPASM Suite\MPLink.exe" /l"C:\MCC18\lib" "C:\MCC18\lkr\18f4520.lkr" "delay.o" "main.o" "LCD.o" /u_CRUNTIME /u_DEBUG /o"TrafficLightsProject.cof" /M"TrafficLightsProject.map" /W

delay.o : delay.c delay.h delay.c
	$(CC) -p=18F4520 /i"C:\MCC18\h" "delay.c" -fo="delay.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

main.o : main.c delay.h LCD.h C:/MCC18/h/i2c.h main.c C:/MCC18/h/p18f4520.h
	$(CC) -p=18F4520 /i"C:\MCC18\h" "main.c" -fo="main.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

LCD.o : LCD.c LCD.h delay.h LCD.c C:/MCC18/h/p18f4520.h
	$(CC) -p=18F4520 /i"C:\MCC18\h" "LCD.c" -fo="LCD.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "delay.o" "main.o" "LCD.o" "TrafficLightsProject.cof" "TrafficLightsProject.hex"

