#include "ros/ros.h"
#include <matlab_kuka/t_d_m.h>
 
//回调函数，简单来说就是收到了这个消息，就做出什么反应在这里面定义。这里是打印一行东西
void chatterCallback(const matlab_kuka::t_d_m::ConstPtr & msg)
{
    ROS_INFO("I heard: [%d]", msg->n);  
}
 
int main(int argc, char **argv)
{
    ros::init(argc,argv,"listener");
    ros::NodeHandle n;
    //订阅了“chatter”这个topic
    ros::Subscriber sub = n.subscribe("chatter",1000, chatterCallback);
    ros::spin();
    return 0;
}

