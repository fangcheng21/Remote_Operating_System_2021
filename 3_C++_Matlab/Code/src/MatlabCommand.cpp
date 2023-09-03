#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include <math.h>

#include "MatlabCommand.h"


#define LEN_DOUBLE 8
#define LEN_DATA 160
#define MAXLINE 4096

MatCommand::MatCommand()
{
	init_data();
	ret = 0;
	for(int i=0; i<7; i++){
		Joint[i]=0;
	}
	for(int i=0; i<6; i++){
		Position[i]={0};
	}
	for(int i=0; i<3; i++){
		Force[i]={0};
	}
	for(int i=0; i<3; i++){
		Moment[i]={0};
	}
	for(int i=0; i<3; i++){
		EulerPoint[i]={0};
	}

}

MatCommand::~MatCommand()
{
	if(Mc.isOpen())
	{
		close();
	}
}

bool MatCommand::open()
{
	return(Mc.open());
}

void MatCommand::close()
{
	ret = 1;
	double SendData[1] = {CLOSE};
	int n = sizeof(SendData);

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline,n);


	pthread_join(pth,NULL);

	Mc.close();
}

void MatCommand::show_data()
{
	printf("关节角：  ");
	for(int i=0; i<7; i++){
		printf("%f ",Joint[i]);
	}

	printf("\n空间坐标：  ");
	for(int i=0; i<6; i++){
		printf("%f ",Position[i]);
	}

	printf("\n  Force：  ");
	for(int i=0; i<3; i++){
		printf("%f ",Force[i]);
	}

	printf("\n  Moment：  ");
	for(int i=0; i<3; i++){
		printf("%f ",Moment[i]);
	}
	printf("\n");
}

void MatCommand::start_send()
{
	double SendData[1] = {0};
	SendData[0] = STARTSEND;
	int n = sizeof(SendData);

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline,n);
}

void MatCommand::stop_send()
{
	double SendData[1] = {0};
	SendData[0] = STOPSEND;
	int n = sizeof(SendData);

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline,n);
}

void * MatCommand::receive_pth(void* __this)
{
	MatCommand* _this=(MatCommand*)__this;
	while(1)
	{
		_this->init_data();
		_this->Mc.receive(_this->recvline, 4096);
		_this->bigtosmall(_this->recvline, LEN_DATA);
		_this->chartodouble(_this->recvline, _this->data, LEN_DATA);
	
		_this->write_data();

		if(_this->ret==1)
		{break;}
	}
	return 0;
};

void MatCommand::receive_lanch()
	{
		pthread_create(&pth,NULL,receive_pth,(void*)this);
	};


void MatCommand::doubletochar(void *s,char *m,size_t n)
{
	char *ss = (char*)s;

	for (int i = 0; i < n; i++)
		m[i]=ss[i];
}

void MatCommand::bigtosmall(char *s,size_t n)
{
	char send_arr[LEN_DOUBLE];
	int length=n/LEN_DOUBLE;

	for(int j=0;j<length;j++){
		for(int i=0;i<LEN_DOUBLE;i++){
			send_arr[i]=s[i+j*LEN_DOUBLE];
		}	
		for(int i=0;i<LEN_DOUBLE;i++){
			s[LEN_DOUBLE-1-i+j*LEN_DOUBLE]=send_arr[i];
		}
	}
}

void MatCommand::chartodouble(void *s,double *m,size_t n)
{
	double *ss = (double*)s;

	for (int i = 0; i < n/LEN_DOUBLE; i++)
		m[i]=ss[i];
}

void MatCommand::init_data()
{
	for(int i = 0; i<20; i++)
	{
		data[i] = 0;
	}
}

void MatCommand::write_data()
{
		if( data[0] == 0 )
		{
		   for(int i=0; i<7; i++){
			  Joint[i]=data[i+1];
			}
			for(int i=0; i<6; i++){
				Position[i]=data[i+8];
			}
			for(int i=0; i<3; i++){
				Moment[i]=data[i+14];
			}
			for(int i=0; i<3; i++){
				Force[i]=data[i+17];
			}
		}
		else if (data[0] == 1 )
		{
			for(int i=0; i<7; i++){
				MeasuredTorques[i]=data[i+1];
			}
		}
		else if (data[0] == 2)
		{
			for (int i = 0; i < 7; i++) {
				Joint[i] = data[i + 1];
			}
		}
		else if (data[0] == 3)
		{
			for (int i = 0; i < 6; i++) {
				Position[i] = data[i + 1];
			}
		}
		else if (data[0] == 4)
		{
			for (int i = 0; i < 7; i++) {
				Moment[i] = data[i + 1];
			}
		}
		else if (data[0] == 5)
		{
			for (int i = 0; i < 7; i++) {
				Force[i] = data[i + 1];
			}
		}
		else
		{
			printf("接收数据格式错误\n");
			ret = 1;
			return ;
		}

};

double*  MatCommand::AngleConversion(const double* values)
{
	double w, x, y,z;
	double roll, pitch, yaw;

	w = values[0];
	x = values[1];
	y = values[2];
	z = values[3];

	double sinr_cosp = 2* ( w*x + y * z);
	double cosr_cosp = 1 - 2*( x*x + y*y );
	roll = std::atan2(sinr_cosp, cosr_cosp);

	double sinp = 2*(w*y - z*y);

	if(std::abs(sinp) >= 1)
		pitch = std::copysign(3.141592 / 2, sinp);
	else
		{
			pitch = std::asin(sinp);
		}

	double siny_cosp = 2*(w*z + x*y);
	double cosy_cosp = 1 - 2*(y*y + z*z);
	yaw = std::atan2(siny_cosp, cosy_cosp);

	EulerPoint[0] = roll;
	EulerPoint[1] = pitch;
	EulerPoint[2] = yaw;

	return EulerPoint;
}


//****************************************************************************************
void MatCommand::movePTPJointSpace(const double* values ,double relvel)
{
	double SendData[9] = {0};
	SendData[0] = MOVEJ;
	int n = sizeof(SendData);

	for(int i=1;i<8;i++){
		SendData[i]=values[i-1];
	}

	SendData[8] = relvel;

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline,n);

}

void MatCommand::movePTPLineEEF(const double* values ,double relvel)
{
	double SendData[8] = {0};
	SendData[0] = MOVEP;
	int n = sizeof(SendData);

	for(int i=1;i<7;i++){
		SendData[i]=values[i-1];
	}

	SendData[7] = relvel;

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline,n);
}

void MatCommand::realTime_startDirectServoCartesian()
{
	double SendData[1] = {0};
	SendData[0] = RELSTARTCARTESIAN;
	int n = sizeof(SendData);

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline,n);
}

void MatCommand::realTime_startDirectServoJoints()
{
	double SendData[1] = {0};
	SendData[0] = RELSTARTJOINTS;
	int n = sizeof(SendData);

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline,n);
}

void MatCommand::realTime_startVelControlJoints()
{
	double SendData[1] = {0};
	SendData[0] = RELSTARTVELJ;
	int n = sizeof(SendData);

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline,n);
}

void MatCommand::realTime_stopDirectServoCartesian()
{
	double SendData[1] = {0};
	SendData[0] = RELSTOPCARTESIAN;
	int n = sizeof(SendData);

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline,n);
}

void MatCommand::realTime_stopDirectServoJoints()
{
	double SendData[1] = {0};
	SendData[0] = RELSTOPJOINTS;
	int n = sizeof(SendData);

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline,n);
}

void MatCommand::realTime_stopVelControlJoints()
{
	double SendData[1] = {0};
	SendData[0] = RELSTOPVELJ;
	int n = sizeof(SendData);

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline,n);
}

double* MatCommand::rel_setJointPosition(const double* values)
{

	double SendData[8] = {0};
	SendData[0] = RELSETJOINT;
	int n = sizeof(SendData);

	for(int i=1;i<8;i++){
		SendData[i]=values[i-1];
	}

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline,n);

	return 0;
	
};

double* MatCommand::rel_setEEfPosition(const double* values)
{
	double SendData[7] = {0};
	SendData[0] = RELSETPOS;
	int n = sizeof(SendData);

	for(int i=1;i<7;i++){
		SendData[i]=values[i-1];
	}

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline,n);

	return 0;
};

double* MatCommand::rel_setJointsVelocities(const double* values)
{
	double SendData[8] = {0};
	SendData[0] = RELSETVEL;
	int n = sizeof(SendData);

	for(int i=1;i<8;i++){
		SendData[i]=values[i-1];
	}

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline,n);

	return 0;
};

void MatCommand::getJointpos()
{
	double SendData[1] = { 0 };
	SendData[0] = GETJOINTS;
	int n = sizeof(SendData);

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline, n);
};

void MatCommand::getEEFPos()
{
	double SendData[1] = { 0 };
	SendData[0] = GETEEFPOS;
	int n = sizeof(SendData);

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline, n);
};

void MatCommand::getEEF_Moment()
{
	double SendData[1] = { 0 };
	SendData[0] = GETEEFMoment;
	int n = sizeof(SendData);

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline, n);
};

void MatCommand::getEEF_Force()
{
	double SendData[1] = { 0 };
	SendData[0] = GETEEFFORCE;
	int n = sizeof(SendData);

	doubletochar(SendData, sendline, n);
	bigtosmall(sendline, n);
	Mc.sendto(sendline, n);
};
