#include "mbed.h"

DigitalOut led1(LED1);

int main() {

#if 0
    Serial pc(USBTX, USBRX, 9600);
    printf("Hello World\r\n");
#endif


    while (true) {
        led1 = !led1;
        wait(0.5);
    }
}
