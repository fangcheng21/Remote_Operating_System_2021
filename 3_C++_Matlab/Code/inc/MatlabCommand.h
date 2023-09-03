#ifndef _MATCOMMAND_H_
#define _MATCOMMAND_H_

#include <pthread.h>

#include "Matlabconnection.h"


#define MOVEJ 1  //movej
#define MOVEP 2  //movep
#define RELSTARTCARTESIAN 3  
#define RELSTARTJOINTS 4
#define RELSTARTVELJ 5
#define RELSTOPCARTESIAN 6
#define RELSTOPJOINTS 7
#define RELSTOPVELJ 8
#define RELSETJOINT 9
#define RELSETPOS 10
#define RELSETVEL 11
#define STARTSEND 12
#define STOPSEND 13
#define GETJOINTS 14
#define GETEEFPOS 15
#define GETEEFMoment 16
#define GETEEFFORCE 17
#define CLOSE 18


class MatCommand
{
public:
	MatCommand();
	~MatCommand();

	bool open();

	void close();

	void show_data();

	void start_send();

	void stop_send();

	static 	void * receive_pth(void*);

	void receive_lanch(); //数据接收模块  循环外单次

	void movePTPJointSpace(const double* values ,double relvel);

	void movePTPLineEEF(const double* values ,double relvel);

	void realTime_startDirectServoCartesian();

	void realTime_startDirectServoJoints();

	void realTime_startVelControlJoints();

	void realTime_stopDirectServoCartesian();

	void realTime_stopDirectServoJoints();

	void realTime_stopVelControlJoints();

	double* rel_setJointPosition(const double* values);

	double* rel_setEEfPosition(const double* values);

	double* rel_setJointsVelocities(const double* values);

	void getJointpos();
	void getEEFPos();
	void getEEF_Moment();
	void getEEF_Force();
	
	double* AngleConversion(const double* values); //四元数到欧拉角的转换（输入[w,x,y,z]，输出[r,p,y]）

private:
	void init_data();

	void doubletochar(void *s,char *m,size_t n);

	void bigtosmall(char *s,size_t n); //小端模式：低字节在低位，高字节在高地址

	void chartodouble(void *s,double *m,size_t n);

	void write_data();



	
private:
	double data[20];  //data[0]为标识符，判断返回类型
	char recvline[4096];
	char sendline[4096];



public:
	double Joint[7];
	double Position[6];
	double Force[3];
	double Moment[3];
	double MeasuredTorques[7];
	double EulerPoint[3];  //欧拉角
	int ret;
	pthread_t pth;
	MatConnection Mc;
};

#endif
