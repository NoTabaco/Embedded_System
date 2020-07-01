/* GPIO LED Driver test application
   FILE : test_led.c
   AUTH : Huins */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define LED0_ON     0x00
#define LED0_OFF    0x01
#define LED1_ON     0x02
#define LED1_OFF    0x03
#define LED2_ON     0x04
#define LED2_OFF    0x05
#define LED3_ON     0x06
#define LED3_OFF    0x07
#define LED_ALL_ON  0x08
#define LED_ALL_OFF 0x09

int main(int argc, char **argv)
{
    int led_fd;
    int get_number;
    unsigned char val[] = {0xF0, 0x70, 0xB0, 0x30, 0xD0, 0x50, 0x90, 0x10, 0xE0,0x60, 0xA0, 0x20, 0xC0, 0x40, 0x80, 0x00};

    char inputChar;
    char *pos;

    led_fd = open("/dev/led_driver", O_RDWR); // 디바이스를 오픈한다.
    if (led_fd<0){  // 만약 디바이스가 정상적으로 오픈되지 않으면 오류 처리후 료한다.
        printf("LED Driver Open Failured!\n");
        return -1;
        }

    while(1) {

    printf("Please Input # >> ");
    scanf("%c", &inputChar);
    get_number = strtol(&inputChar, &pos, 16); //  받은 인자를 숫자로 바꾼다.
    if(get_number >= 0 && get_number <= 15) {  // 숫자가 0~F 까지 포함되는지 확인한다.
        write(led_fd, &val[get_number], sizeof(unsigned char));
        sleep(3);
        }
    else {
        sleep(3);
        printf("Invalid Value : 0 thru F\n");  // 포함되지 않으면, 메시지를 출력한다.
        }
    // char must have One Character So, Assign - 
   if(inputChar == '-') {
        close(led_fd);
        printf("System exit\n");
        return 0;
    }
        }
}
