KST-Kuka-Sunrise-Toolbox-master文件夹为Matlab与iiwa的通讯控制程序
	以KUKA_Sunrise_server_source_code文件夹内代码通过Workbench导入控制器作为服务端，Matlab_client文件夹
	内代码于Matlab内打开为客户端。
	（Kst已经是一个完整单独的使用方式，可按照kst文件夹内说明文档按步骤操作，包括其可实现函数）





code文件夹内为C++SDK接口，系基于Matlab-IIwa进行的二次开发
	工作于linux环境下，使用时将Matlab文件夹内代码复制到KST内的Maltab_client文件夹内
	首先运行位于控制器中的服务端，再此运行Matlab_client中的m_server文件，此时机器人与matlab连接完成
	后运行code内代码通讯完成。（同一电脑）

	code文件夹内为inc为头文件，src为源文件，main文件位于此，留于使用者进行二次开发，通过Makefile编译。
	
	可完成功能详见inc内MatlabCommand.h头文件，现对其函数作出解释

	bool open();打开通讯，返回bool值

	void close();关闭所有通讯

	void show_data();获取机器人数据后可通过该函数展示所有

	void start_send();开启机器人数据的实时传输，一次传输需要3ms,时间间隔为5ms,获取数据存放在类内public:Joint[7]; Position[6]; Force[3]; Moment[3];处，可直接调用。例：printf("%f",Mtc.Joint[1]); 实时控制时需要关闭

	void stop_send();关闭实时传输

	static 	void * receive_pth(void*);

	void receive_lanch(); //数据接收模块  循环外单次  开启双线程实时接收，使用必须开启

	void movePTPJointSpace(const double* values ,double relvel); 普通角度控制，需要7轴角度，与速度

	void movePTPLineEEF(const double* values ,double relvel);普通空间控制，需要6位数据xyxrxryrz，与速度

	void realTime_startDirectServoCartesian();开启软实时空间控制

	void realTime_startDirectServoJoints();开启软实时角度控制

	void realTime_startVelControlJoints();开启软实时速度控制

	void realTime_stopDirectServoCartesian();关闭软实时空间控制

	void realTime_stopDirectServoJoints();关闭软实时角度控制

	void realTime_stopVelControlJoints();关闭软实时速度控制

	double* rel_setJointPosition(const double* values);软实时角度控制，需要首先开启软实时角度控制，完毕后需要关闭

	double* rel_setEEfPosition(const double* values);软实时空间控制，需要首先开启软实时空间控制，完毕后需要关闭

	double* rel_setJointsVelocities(const double* values);软实时速度控制，需要首先开启软实时速度控制，完毕后需要关闭

	void getJointpos(); 获取角度
	void getEEFPos(); 获取空间坐标及姿态
	void getEEF_Moment(); 获取Moment
	void getEEF_Force(); 获取Force



