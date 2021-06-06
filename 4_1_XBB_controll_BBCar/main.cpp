#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
//#include "mbed_rpc.h"


PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);
double ds1, ds2, direct;
DigitalIn encoder(D11);
volatile int steps;
volatile int last;

Ticker servo_ticker;
Ticker encoder_ticker;
void CarControl(Arguments *in, Reply *out);
RPCFunction rpcCar(&CarControl, "CarControl");
BBCar car(pin5, pin6, servo_ticker);

void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}

int main() {
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
   RPC::call(buf, outbuf);
   }
}

void CarControl (Arguments *in, Reply *out)   {

    // In this scenario, when using RPC delimit the two arguments with a space.
    ds1 = in->getArg<double>();
    ds2 = in->getArg<double>();
    direct = in->getArg<double>();

    // Have code here to call another RPC function to wake up specific led or close it.
   xbee.set_baud(9600);
   encoder_ticker.attach(&encoder_control, 10ms);
   steps = 0;
   last = 0;
   car.goStraight(100);
   while(steps*6.5*3.14/32 < ds1-10) {
      printf("encoder1 = %d\r\n", steps);
      ThisThread::sleep_for(100ms);
   }
   car.stop();
   ThisThread::sleep_for(5000ms);
   steps = 0;
   last = 0;
   car.turn(200,direct);
   while(steps*6.5*3.14/32 < 7) {
      printf("encoder2 = %d\r\n", steps);
      ThisThread::sleep_for(100ms);
   }
   car.stop();
   ThisThread::sleep_for(5000ms);
   steps = 0;
   last = 0;
   car.goStraight(100);
   while(steps*6.5*3.14/32 < ds2+5) {
      printf("encoder3 = %d\r\n", steps);
      ThisThread::sleep_for(100ms);
   }
   car.stop();
   ThisThread::sleep_for(100ms);
   out->putData(200);
}