#include "mbed.h"
using namespace std::chrono;
Thread thread1;
Timer t;
AnalogOut aout(PA_4);

const double pi = 3.141592653589793238462;
const double offset = 65535/2;
const float C_4=261.6;
const float B_3=246.9;
const float A_3=220.0;
const float G_3=196.0;
const float F_3=174.6;
const float E_3=164.8;
const float D_3=146.8;
const float C_3=130.8;

void timer_reset(){
    while(1){
        if(duration_cast<milliseconds>(t.elapsed_time()).count() > 1000)
            t.reset();
    }
}

float amplitude(){
    auto ms = duration_cast<milliseconds>(t.elapsed_time()).count();
    float p;
    if(ms < 50){
    p = ms;
    p=p/50;
    }
    else if(ms < 100){
    p = ms;
    p=p*(-0.08) + 5; 
    }
    else if(ms < 150){
    p = 0.6;
    }
    else if (ms < 200){
    p = ms;
    p = p*(-0.012)+2.4;
    }

    return p*0.5f;
}

int main()
{
    t.start();
    thread1.start(timer_reset);

    double rads = 0.0;
    uint16_t sample[360];
    for (int i = 0; i < 360; i++) {
            rads = (pi * i) / 180.0f;
            sample[i] = (uint16_t)amplitude()*offset+offset;
            //sample[i] = (uint16_t)(amplitude() * (offset * (cos(rads + pi))) + offset);
    }
    
    while (true) {

      for (int i = 0; i < 360; i++) {
            aout.write_u16(sample[i]);
      }
    }
}