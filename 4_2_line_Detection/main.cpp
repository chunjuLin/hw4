#include"mbed.h"
#include "bbcar.h"


Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
BBCar car(pin5, pin6, servo_ticker);
char x;
int num[3];
int i= 0, start = 0;
char pre;
int type;
int coordination[4];

int main(){
    pc.set_baud(9600);
   uart.set_baud(9600);
   while(1){
      if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            //pc.write(recv, sizeof(recv));
            x = *recv;
            //printf("%c ",x);
            if(start == 1 && x!= '.'){
                num[i] = (int)x-48;
               // printf("num[%d] = %d\n", i, num[i]);
                i++;
            }
            if(x == ':'){
                start = 1;
                switch(pre){
                    case '1': type = 0; break;
                    case '2': type = 1; break;
                    case '3': type = 2; break;
                    case '4': type = 3; break;
                    default: type = 4; break; 
                }
            }
            if(x == '.'){
                start = 0;
                if(i == 1) coordination[type] = num[0];
                else if(i == 2) coordination[type] = 10*num[0] + num[1];
                else if(i == 3) coordination[type] = 100*num[0] + 10*num[1] + num[2];
                printf("%d\n",i);
                printf("num: %d %d %d\n", num[0], num[1], num[2]);
                i = 0;
                printf("\nC%d: %d\n",type,coordination[type]);
                num[0] = 0; num[1] = 0; num[2] = 0;
            }
            //printf("%c ",x1);
            pre = x;
            int xd = coordination[0] - coordination[1];
            if(xd >= 7) car.turn(20,0.1);
            else if(xd <= -7) car.turn(15,-0.1);
            else car.goStraight(40);
      }
   }
}