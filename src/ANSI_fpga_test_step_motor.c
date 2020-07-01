#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MOTOR_ATTRIBUTE_ERROR_RANGE 4
#define FPGA_STEP_MOTOR_DEVICE "/dev/fpga_step_motor"

void usage(char* dev_info);

int main(int argc, char **argv)
{
        int i;
        int dev;
        int str_size;

        int motor_action;
        int motor_direction;
        int motor_speed;


        unsigned char motor_state[3];

        memset(motor_state,0,sizeof(motor_state));

        if(argc!=4) {
                printf("Please input the parameter! \n");
                usage(argv[0]);
                return -1;
        }

        motor_action = atoi(argv[1]);
        if(motor_action<0||motor_action>1) {
                printf("Invalid motor action!\n");
                usage(argv[0]);
                return -1;
        }

        motor_direction = atoi(argv[2]);
        // Add Direction Index
        if(motor_direction<0||motor_direction>3) {
                printf("Invalid motor direction!\n");
                usage(argv[0]);
                return -1;
        }

        motor_speed = atoi(argv[3]);
        if(motor_speed<0||motor_speed>255) {
                printf("Invalid motor speed!\n");
                usage(argv[0]);
                return -1;
        }

        motor_state[0]=(unsigned char)motor_action;
        motor_state[1]=(unsigned char)motor_direction;
        motor_state[2]=(unsigned char)motor_speed;

        dev = open(FPGA_STEP_MOTOR_DEVICE, O_WRONLY);
        if (dev<0) {
                printf("Device open error : %s\n",FPGA_STEP_MOTOR_DEVICE);
                exit(1);
        }

        // write(dev,motor_state,3);

        // use ioctl function to write motor_state
        // first parameter is device
        // second parameter is request for information
        // third parameter is buffer to storing information
        // ioctl(dev, motor_state, 3);

        unsigned char tempSpeed = motor_state[2];
        if(motor_direction == 2 && motor_speed != 254 && motor_speed != 255){
        // direction is time direction 
        motor_state[1]=(unsigned char)(motor_direction - 2);
        ioctl(dev, motor_state, 3);
        for(; motor_speed < 256 ; motor_speed++) {
        motor_state[2]=(unsigned char)(motor_speed);
        ioctl(dev, motor_state, 3);
        // if min speed, direction is changed
        if(motor_state[2] == 255){
        motor_state[1]=(unsigned char)(motor_direction - 1);
                }
        usleep(20000);
        }
        // change First Input Speed 
        for(; motor_speed > 3 ; motor_speed--){
        motor_state[2]=(unsigned char)(motor_speed);
        ioctl(dev, motor_state, 3);
        if(tempSpeed == motor_state[2]) {
        break;
        }
        usleep(5000);
                }
        } else if(motor_direction == 3 && motor_speed != 254 && motor_speed != 255) {
        // direction is counter time direction
        motor_state[1]=(unsigned char)(motor_direction - 2);
        ioctl(dev, motor_state, 3);
        for(; motor_speed < 256 ; motor_speed++) {
        motor_state[2]=(unsigned char)(motor_speed);
        ioctl(dev, motor_state, 3);
        // if min speed, direction is changed
        if(motor_state[2] == 255){
        // time direction
        motor_state[1]=(unsigned char)(motor_direction - 3);
        }
        usleep(20000);
                }
        // change First Input Speed
        for(; motor_speed > 3 ; motor_speed--){
        motor_state[2]=(unsigned char)(motor_speed);
        ioctl(dev, motor_state, 3);
        if(tempSpeed == motor_state[2]){
        break;
  	}
        usleep(5000);
        }
                }

        // 254 low -> fast
        // low number is fast
        if (motor_speed == 254){
        // initialized Min speed 255 
        motor_state[2]=(unsigned char)(motor_speed + 1);
        ioctl(dev, motor_state, 3);
        // speed low -> fast
        for( ; motor_speed > 3 ; motor_speed--){
        motor_state[2]=(unsigned char)(motor_speed);
        ioctl(dev, motor_state, 3);

        if(motor_direction == 2 && motor_speed < 50 && motor_state[1] != 1){
        motor_state[1]=(unsigned char)(motor_direction - 1);
        } else if(motor_direction == 3 && motor_speed < 50 && motor_state[1] != 0) {
        motor_state[1]=(unsigned char)(motor_direction - 3);
                }
        usleep(5000);
        }
      } else if (motor_speed == 255){
        // 255 fast -> low
        // initialized Max speed 3 
        motor_state[2]=(unsigned char)(motor_speed - 252);
        ioctl(dev, motor_state, 3);
        // speed fast -> low
        motor_speed = 3;
        for( ; motor_speed < 255; motor_speed++){
        motor_state[2]=(unsigned char)(motor_speed);
        ioctl(dev, motor_state, 3);
        if(motor_direction == 2 && motor_speed > 100 && motor_state[1] != 1){
        motor_state[1]=(unsigned char)(motor_direction - 1);
        } else if(motor_direction == 3 && motor_speed > 100 && motor_state[1] != 0) {
        motor_state[1]=(unsigned char)(motor_direction - 3);
        }

        usleep(10000);
        }
      } else {
        ioctl(dev, motor_state, 3);
        }

        close(dev);

        return 0;
}

void usage(char* dev_info)
{
        printf("<Usage> %s [Motor Action] [Motor Diretion] [Speed]\n",dev_info);
        printf("Motor Action : 0 - Stop / 1 - Start\n");
        printf("Motor Direction : 0 - Left / 1 - Right\n");
        printf("Motor Speed : 0(Fast) ~ 250(Slow)\n");
        printf("ex) %s 1 0 10\n",dev_info);
}
