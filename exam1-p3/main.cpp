#include "mbed.h"
#include "TextLCD.h"

Thread thread_master;
Thread thread_slave;
Thread queuecall;
EventQueue queue(32 * EVENTS_EVENT_SIZE);
InterruptIn button(BUTTON1);
//master

SPI spi(D11, D12, D13); // mosi, miso, sclk
DigitalOut cs(D9);

SPISlave device(PD_4, PD_3, PD_1, PD_0); //mosi, miso, sclk, cs; PMOD pins
// I2C Communication
I2C i2c_lcd(D14, D15); // SDA, SCL

//TextLCD_SPI lcd(&spi_lcd, p8, TextLCD::LCD40x4);   // SPI bus, 74595 expander, CS pin, LCD Type
TextLCD_I2C lcd(&i2c_lcd, 0x4E, TextLCD::LCD16x2);   // I2C bus, PCF8574 Slaveaddress, LCD Type
                                                     //TextLCD_I2C lcd(&i2c_lcd, 0x42, TextLCD::LCD16x2, TextLCD::WS0010);
                                                     // I2C bus, PCF8574 Slaveaddress, LCD Type, Device Type
                                                     //TextLCD_SPI_N lcd(&spi_lcd, p8, p9);
                                                     // SPI bus, CS pin, RS pin, LCDType=LCD16x2, BL=NC, LCDTCtrl=ST7032_3V3
//TextLCD_I2C_N lcd(&i2c_lcd, ST7032_SA, TextLCD::LCD16x2, NC, TextLCD::ST7032_3V3);
// I2C bus, Slaveaddress, LCD Type, BL=NC, LCDTCtrl=ST7032_3V3
void print(){
    printf("interrupt triggered.\n");
}

void call(){
    queue.call(print);
}

void slave()
{
    queuecall.start(callback(&queue, &EventQueue::dispatch_forever));
    button.rise(&call);
   device.format(8, 3);
   device.frequency(1000000);
   //device.reply(0x00); // Prime SPI with first reply
   int v;
   while(1){
       v = device.read();
       if(v == 1){
            lcd.locate(2, 1);
            ThisThread::sleep_for(1000ms);
       }
       else if(v == 2){
            lcd.cls();
            ThisThread::sleep_for(1000ms);
       }
       else if(v == 3){
           lcd.putc('c');
           ThisThread::sleep_for(1000ms);
       }
   }
}

int main()
{
   thread_slave.start(slave);
   uint16_t command = 0x01;

   // Setup the spi for 8 bit data, high steady state clock,
   // second edge capture, with a 1MHz clock rate
   spi.format(8, 3);
   spi.frequency(1000000);
while(1){
      // Chip must be deselected
      cs = 1;
      // Select the device by seting chip select low
      cs = 0;

      spi.write(command); //Send ID
      cs = 1;                       // Deselect the device
      ThisThread::sleep_for(5000ms); //Wait for debug print
    cs = 0;
    command = 0x02;
    spi.write(command);
    cs = 1;
    ThisThread::sleep_for(5000ms);
    cs = 0;
    command = 0x03;
    spi.write(command);
    ThisThread::sleep_for(1000ms);
    cs = 1;
}
      // Select the device by seting chip select low
}
