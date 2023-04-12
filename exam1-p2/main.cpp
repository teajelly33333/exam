#include "mbed.h"
PwmOut PWM1(D6);
Thread t;
AnalogIn Ain(A0);
AnalogOut Aout(D7);

void sample(){
    while(1){
    Aout=Ain;
    printf("%f\n", Aout.read());
    wait_us(1);
    }
}

// main() runs in its own thread in the OS
int main()
{
    t.start(sample);
    while (true) {
      PWM1.period_us(50);
      PWM1.pulsewidth_us(25);
      ThisThread::sleep_for(1ms);
      PWM1.period_us(50);
      PWM1.pulsewidth_us(40);
      ThisThread::sleep_for(1ms);
    }
}

