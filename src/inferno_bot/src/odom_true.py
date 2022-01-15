#!/usr/bin/env python

# Title:        Turtlebot Navigation Stack
# File:         odom_true.py
# Date:         2017-02-13
# Author:       Preben Jensen Hoel and Paul-Edouard Sarlin
# Description:  Publishes the ground truth state of the robot acquired from Gazebo.


import rospy
import tf
from gazebo_msgs.msg import ModelStates
from nav_msgs.msg import Odometry
from geometry_msgs.msg import PoseWithCovariance, Pose, \
                              TwistWithCovariance, Twist, TransformStamped


robot_name = "inferno_bot"


def callback(model_states):
    rospy.logdebug("Received ModelStates")
    try:
        idx = model_states.name.index(robot_name)
    except ValueError:
        rospy.logerr("[ModelStates] Could not find model with name %s",
                     robot_name)
        return
    
    model_pose = model_states.pose[idx]
    model_twist = model_states.twist[idx]
    
    msg = Odometry()
    msg.pose.pose = model_pose
    msg.twist.twist = model_twist
    pub_odom.publish(msg)
   


def initialize():
    global pub_odom, pub_tf, t, t2
    rospy.init_node("odom_correcter", anonymous=True)	
    rospy.Subscriber("/gazebo/model_states", ModelStates, callback)
    pub_odom = rospy.Publisher("/odom_true", Odometry, queue_size=1)

    #Transformation for the world frame
    t = TransformStamped()
    t.header.frame_id = "world"
    t.header.stamp = rospy.Time.now()
    t.child_frame_id = "/dummy_link"
    t.transform.translation.x = 0.0
    t.transform.translation.y = 0.0
    t.transform.translation.z = 0.0
    t.transform.rotation.x = .0
    t.transform.rotation.y = 0.0
    t.transform.rotation.z = -1.57
    t.transform.rotation.w = 1.0
    
    t2 = TransformStamped()
    t2.header.frame_id = "base_link"
    t2.child_frame_id = "base_link"
    
    rospy.spin()


if __name__ == "__main__":
    initialize()
