#include <wiringPi.h>
#include <stdio.h>
int main (int argc, char *argv[])
{
  wiringPiSetup () ;
  pinMode (4, INPUT) ;
  pinMode(3, OUTPUT); 
 pullUpDnControl(4, PUD_UP) ;
  for (;;)
  {
    int x = digitalRead(4);
    if(x == 0){digitalWrite(3, HIGH); delay(500); digitalWrite(3,LOW);}
    printf("Got a %d\n",x);
  }
  return 0 ;
}
