

#include <ros.h>
#include <std_msgs/UInt16.h>
#include <geometry_msgs/Twist.h>



#define PWM1L 9
#define PWM1R 10
#define PWM2L 5
#define PWM2R 6
#define DIR1L 7
#define DIR1R 8
#define DIR2L 3
#define DIR2R 4

float track=0.45;
float VL,VR;

ros::NodeHandle nh;
void Motor1L(float motion);
void Motor1R(float motion);
void Motor2L(float motion);
void Motor2R(float motion);
float speed_angular=0, speed_linear=0;
void messageCb( const geometry_msgs::Twist& msg){
  speed_angular = msg.angular.z;
  speed_linear = msg.linear.x;
  speed_angular = (speed_angular*track)/(2.0);
  VR=speed_linear + speed_angular;
  VL= speed_linear - speed_angular;

   Motor1L(VL);
   Motor1R(VR);
   Motor2L(VL);
   Motor2R(VR);
}


ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb );



void setup(){
 pinMode(PWM1L, OUTPUT);
 pinMode(PWM1R, OUTPUT);
 pinMode(PWM2L, OUTPUT);
 pinMode(PWM2R, OUTPUT);
 pinMode(DIR1L, OUTPUT);
 pinMode(DIR1R, OUTPUT);
 pinMode(DIR2L, OUTPUT);
 pinMode(DIR2R, OUTPUT);
 digitalWrite(PWM1L, LOW);
 digitalWrite(PWM1R, LOW);
 digitalWrite(PWM2L, LOW);
 digitalWrite(PWM2R, LOW);

 nh.initNode();
 nh.subscribe(sub);
}
void loop(){

 nh.spinOnce();
 delay(1);
}

 

void Motor1L(float motion){
if(motion>=0){
  analogWrite(PWM1L, motion);
  digitalWrite(DIR1L, HIGH); //clockwise
}else{
  motion=-1*motion;
  analogWrite(PWM1L, motion);
  digitalWrite(DIR1L, LOW); //anti-clockwise
}

}

void Motor1R(float motion){
if(motion>=0){
  analogWrite(PWM1R, motion);
  digitalWrite(DIR1R, HIGH); //clockwise
}else{
  motion=-1*motion;
  analogWrite(PWM1R, motion);
  digitalWrite(DIR1R, LOW); //anti-clockwise
}
   
}

void Motor2L(float motion){
if(motion>=0){
  analogWrite(PWM2L, motion);
  digitalWrite(DIR2L, HIGH); //clockwise
}else{
  motion=-1*motion;
  analogWrite(PWM2L, motion);
  digitalWrite(DIR2L, LOW); //anti-clockwise
}

}

void Motor2R(float motion){
if(motion>=0){
  analogWrite(PWM2R, motion);
  digitalWrite(DIR2R, HIGH); //clockwise
}else{
  motion=-1*motion;
  analogWrite(PWM2R, motion);
  digitalWrite(DIR2R, LOW); //anti-clockwise
}
   
}
