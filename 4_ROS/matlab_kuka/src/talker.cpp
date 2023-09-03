#include "ros/ros.h"
#include <matlab_kuka/t_d_m.h>
 
int main(int argc, char **argv)
{
    ros::init(argc,argv, "matlab_kuka");
    ros::NodeHandle n;
    //topic的名称为chatter,1000为缓冲区，缓冲区中的消息在大于 1000 个的时候就会开始丢弃先前发布的消息。 
    ros::Publisher chatter = n.advertise<matlab_kuka::t_d_m>("chatter",1000);
    ros::Rate loop_rate(20); //发布频率为10hz
 

    //主循环
    while(n.ok())
    {
        matlab_kuka::t_d_m msg;
        //给消息里的变量赋值
        msg.tT = 1;
        msg.n = 7;
        msg.Joint.resize(7);
        msg.Position.resize(6);
        msg.Force.resize(3);
        msg.Moment.resize(3);
        msg.MeasuredTorques.resize(7);


///////////////////////////////////
//  用户代码                     //
//////////////////////////////////


//赋值示例
        for (int i = 0;i<msg.n;i++)
        {
            msg.Joint[i] = i;
        }
//////////////////////////////////

 
        //发布消息
        chatter.publish(msg);
        ros::spinOnce(); //可用回调函数
        loop_rate.sleep();//休眠一段时间以使得发布频率为 10Hz。
    }
    return 0;
}
