/* GPIO FND Driver test application
   FILE : test_fnd.c
   AUTH : Huins */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FND0    0x00
#define FND1    0x01
#define FND2    0x02
#define FND3    0x03
#define FND4    0x04
#define FND5    0x05
#define FND6    0x06
#define FND7    0x07
#define FND8    0x08
#define FND9    0x09
#define FNDP    0x0A // DP
#define FNDA    0x0B // ALL
#define FNDX    0x0C // ALL OFF

// Define Fnd Number
#define FIRSTVALUE 0x02
#define SECONDVALUE 0x04
#define THIRDVALUE 0x10
#define FOURTHVALUE 0x80

// Define Real Number
#define PRINTZERO 0x02
#define PRINTONE 0x9F
#define PRINTTWO 0x25
#define PRINTTHREE 0x0D
#define PRINTFOUR 0x99
#define PRINTFIVE 0x49
#define PRINTSIX 0xC1
#define PRINTSEVEN 0x1F
#define PRINTEIGHT 0x01
#define PRINTNINE 0x09

int main(int argc, char **argv)
{
    int fnd_fd;
    int tempNumber;
    char get_fndnumber;
    char get_number;
    char set_fndvalue;
    char set_value;
    char splitChar[5];
    unsigned short temp;
    unsigned char temp1;
    unsigned char temp2;
    char array[] = {'1', '2', '3', '4'};

    fnd_fd = open("/dev/fnd_driver", O_WRONLY);
    if (fnd_fd<0){
        printf("FND Driver Open Failed!\n");
        return -1;
    }

    while(1){
    printf("Please Input # >> ");
    scanf("%s", splitChar);
    int i;
    int j;

    if(splitChar[0] == '-'){
        printf("System Exit\n");
        close(fnd_fd);
        return 0;
    }

    for(i = 1; i <= 4 ; i++){
    j = array[i - 1] - '0';
    get_fndnumber = (char)j;
    switch(get_fndnumber) {
        case 1 :
            set_fndvalue = FIRSTVALUE;
            break;
        case 2 :
            set_fndvalue = SECONDVALUE;
            break;
        case 3 :
            set_fndvalue = THIRDVALUE;
            break;
        case 4 :
            set_fndvalue = FOURTHVALUE;
            break;
    }

    tempNumber = splitChar[i - 1] - '0';
    get_number = (char)tempNumber;
    switch(get_number) {
        case FND0 :
            set_value = PRINTZERO;
            break;
        case FND1 :
            set_value = PRINTONE;
            break;
        case FND2 :
            set_value = PRINTTWO;
            break;
        case FND3 :
            set_value = PRINTTHREE;
            break;
        case FND4 :
            set_value = PRINTFOUR;
            break;
        case FND5 :
            set_value = PRINTFIVE;
            break;
        case FND6 :
            set_value = PRINTSIX;
            break;
        case FND7 :
            set_value = PRINTSEVEN;
            break;
        case FND8 :
            set_value = PRINTEIGHT;
            break;
        case FND9 :
            set_value = PRINTNINE;
            break;
    }

    temp1 = set_fndvalue;
    temp2 = set_value;
    temp = temp + temp1;
    temp = (temp<<8)|temp2;
    write(fnd_fd, &temp, sizeof(short));
    sleep(3);
    temp = 0;
    write(fnd_fd, &temp, sizeof(short));

    }
  }
}
                    

