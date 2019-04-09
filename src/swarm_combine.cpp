#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/NavSatFix.h>
#include <geometry_msgs/PoseStamped.h>
#include <swarm_combine/swarm_msg.h>

using namespace std;

swarm_combine::swarm_msg out_msg;

void global_position_cb(const sensor_msgs::NavSatFix::ConstPtr& global_msg){
    out_msg.global_latitude  = global_msg->latitude;
    out_msg.global_longitude = global_msg->longitude;
    out_msg.global_altitude  = global_msg->altitude;
}


void local_position_cb(const geometry_msgs::PoseStamped::ConstPtr& local_msg){
    out_msg.local_x = local_msg->pose.position.x;
    out_msg.local_y = local_msg->pose.position.y;
    out_msg.local_z = local_msg->pose.position.z;
}


int main(int argc, char **argv)
{
std::string vehicle_namespace, node_name;

vehicle_namespace = getenv("HOSTNAME");
node_name = vehicle_namespace + "_swarm_msgs";

ros::init(argc, argv, node_name);
ros::NodeHandle nh;
ROS_INFO("Vehicle namespace set to : %s", vehicle_namespace.c_str());

std::string s1, s2, s3, s4;
s1 = vehicle_namespace + "/global_position/global" ;
s2 = vehicle_namespace + "/local_position/pose" ;
s3 = vehicle_namespace + "/swarm_combined" ;

    ros::Subscriber gpos_sub = nh.subscribe<sensor_msgs::NavSatFix>
            (s1, 10, global_position_cb);
    ros::Subscriber lpos_sub = nh.subscribe<geometry_msgs::PoseStamped>
            (s2, 10, local_position_cb);

   
    ros::Publisher msg_pub = nh.advertise<swarm_combine::swarm_msg>
            (s4, 100);

    ros::Rate rate(5.0);
  
    out_msg.header.stamp=ros::Time::now();

    while(ros::ok()){

        msg_pub.publish(out_msg);

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
