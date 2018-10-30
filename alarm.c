#include <stdio.h>
#include <wiringPi.h>
#include "ifttt.h"
#include <time.h>

#define ALARM_OFF 0
#define ALARM_ARMING 1
#define ALARM_ARMED 2
#define ALARM_TRIGGERED 3
#define ALARM_SOUNDING 4

/*
pin 0: motion detector
pin 1: green led
pin 2: red led
pin 3: buzzer
pin 4: button
*/
int main(int argc, char *argv[])
{
 int i;
 int state;
 int buttonInput;
 wiringPiSetup () ;
 pinMode(0, INPUT);
 pinMode(1, OUTPUT);
 pinMode(2, OUTPUT);
 pinMode(3, OUTPUT);
 pinMode (4, INPUT);
 pullUpDnControl(4, PUD_UP);
 state = ALARM_OFF;
 while(1) {
     
     switch (state)
     {      
         case ALARM_OFF:{
			 printf("ALARM_OFF\n");
             digitalWrite(1, LOW);
             digitalWrite(2, LOW);
             digitalWrite(3, LOW);
             while(digitalRead(4) == 1){}
			 state = ALARM_ARMING;
             break;
		 }
        case ALARM_ARMING:{
			double tenSecondStart = time(NULL);
			double currentTime = time(NULL);
			printf("ALARM_ARMING\n");
			while(currentTime - tenSecondStart < 10){
				double ledStart = time(NULL);
				double currentLed = time(NULL);
				while(currentLed - ledStart < 1){
					digitalWrite(1, HIGH);
					currentLed = time(NULL);
				}
				ledStart = time(NULL);
				while(currentLed - ledStart < 1){
					digitalWrite(1, LOW);
					currentLed = time(NULL);
				}
				currentTime = time(NULL);
			}
			digitalWrite(2, HIGH);
			state = ALARM_ARMED;
			break;
	 	}
		case ALARM_ARMED:{
			int hasExited = 0;
			printf("ALARM_ARMED\n");
			printf("waitng for sensor\n");
			while(digitalRead(0) == 1 && !hasExited){
				if(digitalRead(4) == 0){
					hasExited = 1;
					state = ALARM_OFF;
					delay(1000);
				}
			}
			while(digitalRead(0) == 0 && !hasExited){
				if(digitalRead(4) == 0){
					hasExited = 1;
					state = ALARM_OFF;
					delay(1000);
				}
			}
			if(digitalRead(0) == 1 && !hasExited){
				state = ALARM_TRIGGERED;
			}
			break;
		}
		case ALARM_TRIGGERED:{
			double tenSecondStart = time(NULL);
			double currentTime = time(NULL);
			printf("ALARM_TRIGGERED\n");
			while(currentTime - tenSecondStart < 10 && state != ALARM_OFF){
				double ledStart = time(NULL);
				double currentLed = time(NULL);
				while(currentLed - ledStart < 1){
					digitalWrite(1, HIGH);
					digitalWrite(2, HIGH);
					currentLed = time(NULL);
					if(digitalRead(4) == 0){
						state = ALARM_OFF;
						delay(1000);
					}
				}
				ledStart = time(NULL);
				while(currentLed - ledStart < 2 && state != ALARM_OFF){
					digitalWrite(1, LOW);
					digitalWrite(2,LOW);
					currentLed = time(NULL);
					if(digitalRead(4) == 0){
						state = ALARM_OFF;
						delay(1000);
					}
				}
				currentTime = time(NULL);
			}
			if(state != ALARM_OFF){
				state = ALARM_SOUNDING;
			}
			break;
		}
		case ALARM_SOUNDING:{
			double alarmStart = time(NULL);
			double currentTime = time(NULL);
			printf("ALARM_SOUNDING\n");
			while(currentTime - alarmStart < 2){
				digitalWrite(1, HIGH);
				digitalWrite(2, HIGH);
				digitalWrite(3, HIGH);
				currentTime = time(NULL);
			}
			digitalWrite(1, LOW);
			digitalWrite(2, LOW);
			digitalWrite(3, LOW);
			ifttt("https://maker.ifttt.com/trigger/alarm_triggered/with/key/Cnfhg7_qaNykhn6Du0ekJ", "alarm", "Reza", "Tuesday");
			printf("message sent \n");
			while(digitalRead(4) == 1){}
			if(digitalRead(4) == 0){
				state = ALARM_OFF;
				delay(1000);
			}
			break;
		}
         default:
             break;
     }
    /*printf("Waiting for reset\n");
    digitalWrite(1, LOW);
    digitalWrite(2,HIGH);
    while(digitalRead(0) == 1){
        digitalWrite(2, HIGH); delay(500);
        digitalWrite(2, LOW); delay(300);
    }
    digitalWrite(1, HIGH);
    digitalWrite(2, LOW);
    printf("Waiting for event\n");
    while(digitalRead(0) == 0){
        digitalWrite(1, HIGH); delay(500);
        digitalWrite(1, LOW); delay(300);
    }
    printf("Sending data to server\n");
    printf("Alarm\n");
    digitalWrite(2, HIGH);
    ifttt("http://red.eecs.yorku.ca:8080/trigger/event/with/key/radio", "alarm", "Reza", "Tuesday");
 */
 }
 /*NOTREACHED*/
 return 0 ;
}
