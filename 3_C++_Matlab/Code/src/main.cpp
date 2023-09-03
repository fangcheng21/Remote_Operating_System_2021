#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <pthread.h>
#include <time.h>
#include "MatlabCommand.h"

double joint[7]={1,1,1,1,1,1,1};
double position[6]={0};

int main()
{
	MatCommand Mtc;
	Mtc.open(); //打开通讯
	Mtc.receive_lanch(); //数据接收
	//Mtc.start_send(); //开启机器人数据的实时传输


	while(1){

			int choice;

			if (choice == 1) {
				break;
			}
			else if (choice == 2) {
				Mtc.movePTPJointSpace(joint, 0.15);  
			}
			else if (choice == 3) {
				//Mtc.movePTPLineEEF(position, 0.15);
			}
			else if (choice == 4) {
				Mtc.realTime_startDirectServoCartesian();  
			}
			else if (choice == 5) {
				Mtc.realTime_startDirectServoJoints();  
			}
			else if (choice == 6) {
				Mtc.realTime_startVelControlJoints();  
			}
			else if (choice == 7) {
				Mtc.realTime_stopDirectServoCartesian();  
			}
			else if (choice == 8) {
				Mtc.realTime_stopDirectServoJoints();  
			}
			else if (choice == 9) {
				Mtc.realTime_stopVelControlJoints();  
			}
			else if (choice == 10) {
				Mtc.rel_setJointPosition(joint);  
			}
			else if (choice == 11) {
				//Mtc.rel_setEEfPosition(position);  
			}
			else if (choice == 12) {
				//Mtc.rel_setJointsVelocities(joint); 
			}
			else if (choice == 13) {
				Mtc.getJointpos();  
			}
			else if (choice == 14) {
				Mtc.getEEFPos(); 
			}
			else if (choice == 15) {
				Mtc.getEEF_Moment();  
			}
			else if (choice == 16) {
				Mtc.getEEF_Force();  
			}
			else if (choice == 17) {
				Mtc.start_send();  
			}
			else if (choice == 18) {
				Mtc.stop_send();  
			}




			Mtc.show_data();  //数据显示
	};

	Mtc.close();  //关闭

	return 0;
}
