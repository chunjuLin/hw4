#include"mbed.h"
#include "bbcar.h"


Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);
DigitalInOut ping(D12);
Timer t;

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
BBCar car(pin5, pin6, servo_ticker);
char x;
int num[3];
int i= 0, start = 0;
char pre;
int type;
int angle;

int main(){
    float val;
    pc.set_baud(9600);
   uart.set_baud(9600);
   while(1){
      if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            //pc.write(recv, sizeof(recv));
            x = *recv;
            //printf("%c ",x);
            if(x != '\n' || x!= '.'){
                num[i] = (int)x-48;
               // printf("num[%d] = %d\n", i, num[i]);
                i++;
            }
            if(x == '.'){
                start = 0;
                if(i == 1) angle = num[0];
                else if(i == 2) angle = 10*num[0] + num[1];
                else if(i == 3) angle = 100*num[0] + 10*num[1] + num[2];
                printf("%d\n",i);
                printf("num: %d %d %d\n", num[0], num[1], num[2]);
                i = 0;
                printf("\nC: %d\n",angle);
                num[0] = 0; num[1] = 0; num[2] = 0;
            }
            //printf("%c ",x1);
            if(angle < 345 && angle >= 80) car.turn(80,0.3);
            else if(angle >10 && angle < 290) car.turn(80,-0.3);
            else car.goStraight(80);
        
            ping.output();
            ping = 0; wait_us(200);
            ping = 1; wait_us(5);
            ping = 0; wait_us(5);

            ping.input();
            while(ping.read() == 0);
            t.start();
            while(ping.read() == 1);
            val = t.read();
            printf("Ping = %lf\r\n", val*17700.4f);
            t.stop();
            t.reset();
      }
   }
}