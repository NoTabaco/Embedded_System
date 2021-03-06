#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <fcntl.h> 

void stepmotor_main (int action, int direction, int speed)
{
	// index 0 is State, 1 is Direction, 2 is Speed
	unsigned char motor_state[3];
	memset(motor_state, 0, sizeof(motor_state));

	int motor_action;
	int motor_direction;
	int motor_speed;

	int fd = -1;

    fd = open("/dev/fpga_step_motor_driver", O_RDWR);
    if (fd < 0){
	__android_log_print(ANDROID_LOG_INFO, "Device Open Error", "StepMotorStart Number = %d, StepMotor Direction = %d, StepMotor Speed = %d", action, direction, speed);
    }
    else  {
        __android_log_print(ANDROID_LOG_INFO, "Device Open Success", "StepMotorStart Number = %d, StepMotor Direction = %d, StepMotor Speed = %d", action, direction, speed);

        // Set Start or Stop State
        // 0 = Stop
        // 1 = Start
        motor_action = action;
        switch(motor_action){
        case 0 :
        	motor_state[0] = (unsigned char)0;
        	break;
        case 1 :
        	motor_state[0] = (unsigned char)1;
        	break;
        }

        // Set Direction
        // 0 = Clock Direction
        // 1 = Counter Clock Direction
        // 2 = Clock => Counter Clock Direction
        // 3 = Counter Clock => Clock Direction
        motor_direction = direction;
        switch(motor_direction){
        case 0 :
        	motor_state[1] = (unsigned char)0;
        	break;
        case 1 :
        	motor_state[1] = (unsigned char)1;
        	break;
        case 2 :
        	// Clock => Counter Clock, So Initialized Clock Direction
        	motor_state[1] = (unsigned char)0;
            break;
        case 3 :
        	// Counter => Clock, So Initialized Counter Clock Direction
        	motor_state[1] = (unsigned char)1;
            break;
        }

        // initialized Speed
        motor_speed = speed;
        motor_state[2] = (unsigned char)(motor_speed);

        // if Direction is Clock -> Counter Clock, Not Speed 245 Or 255
        unsigned char tempSpeed = motor_state[2];
        if(motor_direction == 2 && motor_speed != 254 && motor_speed != 255){
        	write(fd, motor_state, 3);
        	for( ; motor_speed < 256 ; motor_speed++){
        	motor_state[2] = (unsigned char)(motor_speed);
        	write(fd, motor_state, 3);
        	// if min speed, direction is changed
        	if(motor_state[2] == 255){
        	motor_state[1]= (unsigned char)(motor_direction - 1);
        		}
        	usleep(20000);
        	}
        	// change First Input Speed
        	for( ; motor_speed > 3 ; motor_speed--){
        	motor_state[2]= (unsigned char)(motor_speed);
        	write(fd, motor_state, 3);
        	if(tempSpeed == motor_state[2]){
        	// View Final State
        	__android_log_print(ANDROID_LOG_INFO, "Device Final State",  "StepMotorStart Number = %d, StepMotor Direction = %d, StepMotor Speed = %d", motor_state[0], motor_state[1], motor_state[2]);
        	break;
        	}
        	usleep(5000);
        	}
        } else if(motor_direction == 3 && motor_speed != 254 && motor_speed != 255) {
        	// if Direction is Counter Clock -> Clock, Not Speed 245 Or 255
        	write(fd, motor_state, 3);
        	for(; motor_speed < 256; motor_speed++) {
        	motor_state[2] = (unsigned char)(motor_speed);
        	write(fd, motor_state, 3);
        	// if min speed, direction is changed
        	if(motor_state[2] == 255){
        	// time direction
        	motor_state[1] = (unsigned char)(motor_direction - 3);
        	}
        	usleep(20000);
        	}
        	// change First Input Speed
        	for(; motor_speed > 3 ; motor_speed--){
        	motor_state[2]= (unsigned char)(motor_speed);
        	write(fd, motor_state, 3);
        	if(tempSpeed == motor_state[2]){
        	// View Final State
        	__android_log_print(ANDROID_LOG_INFO, "Device Final State",  "StepMotorStart Number = %d, StepMotor Direction = %d, StepMotor Speed = %d", motor_state[0], motor_state[1], motor_state[2]);
        	break;
        	}
        	usleep(5000);
        	}
        }

        // 254 motor speed low -> fast
        // low number is fast
        if (motor_speed == 254){
        // initialized Min Speed 255
        motor_state[2] = (unsigned char)(motor_speed + 1);
        write(fd, motor_state, 3);
        // speed low -> fast
        for ( ; motor_speed > 3 ; motor_speed--){
        motor_state[2] = (unsigned char)(motor_speed);
        write(fd, motor_state, 3);
        // if motor_direction is 2, 3
        if (motor_direction == 2 && motor_speed < 50 && motor_state[1] != 1){
        motor_state[1] = (unsigned char)(motor_direction - 1);
        } else if(motor_direction == 3 && motor_speed < 50 && motor_state[1] != 0){
        motor_state[1] = (unsigned char)(motor_direction - 3);
        }
        usleep(5000);
        }
        if (motor_speed == 3){
        // View Final State
        __android_log_print(ANDROID_LOG_INFO, "Device Final State",  "StepMotorStart Number = %d, StepMotor Direction = %d, StepMotor Speed = %d", motor_state[0], motor_state[1], motor_state[2]);
        }
    } else if (motor_speed == 255){
    	// 255 motor speed fast -> low
    	// high number is low
    	// initialized Max Speed 3
    	motor_state[2] = (unsigned char)(motor_speed - 252);
    	write(fd, motor_state, 3);
    	// speed fast -> low
    	motor_speed = 3;
    	for(; motor_speed < 256; motor_speed++){
    	motor_state[2] = (unsigned char)(motor_speed);
    	write(fd, motor_state, 3);
    	// if motor_direction is 2, 3
    	if(motor_direction == 2 && motor_speed > 100 && motor_state[1] != 1){
    	motor_state[1] = (unsigned char)(motor_direction - 1);
    	} else if(motor_direction == 3 && motor_speed > 100 && motor_state[1] != 0){
    	motor_state[1] = (unsigned char)(motor_direction - 3);
    	}
    	usleep(10000);
    	}
    	if(motor_speed == 255){
    	// View Final State
        __android_log_print(ANDROID_LOG_INFO, "Device Final State",  "StepMotorStart Number = %d, StepMotor Direction = %d, StepMotor Speed = %d", motor_state[0], motor_state[1], motor_state[2]);
    	}
    	} else {
    	// another every case, common case
    	write(fd, motor_state, 3);
     	// View Final State
        __android_log_print(ANDROID_LOG_INFO, "Device Final State",  "StepMotorStart Number = %d, StepMotor Direction = %d, StepMotor Speed = %d", motor_state[0], motor_state[1], motor_state[2]);
    }
    }	

    close(fd);
}

jstring Java_achro4_huins_ex2_StepMotorJniActivity_ReceiveStepMotorValue( JNIEnv* env,jobject thiz, jint valState, jint valDirection, jint valSpeed )
{  
	__android_log_print(ANDROID_LOG_INFO, "StepMotorJni", "StepMotorStart Number = %d, StepMotor Direction = %d, StepMotor Speed = %d", valState, valDirection, valSpeed);
	stepmotor_main(valState, valDirection, valSpeed);
	return NULL;
}
