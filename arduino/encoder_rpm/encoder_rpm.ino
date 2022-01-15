/*
  Gearmotor Rotary Encoder Test
  motor-encoder-rpm.ino
  Read pulses from motor encoder to calculate speed
  Control speed with potentiometer
  Displays results on Serial Monitor
  Use Cytron MD10C PWM motor controller
  DroneBot Workshop 2019
  https://dronebotworkshop.com
*/
 #include <ros.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/Twist.h>

// Motor encoder output pulse per rotation (change as required)
#define ENC_COUNT_REV 696
 
// Encoder output to Arduino Interrupt pin
#define ENC_IN 3  //A pin 
 
// MD10C PWM connected to pin 10
#define PWM 10 
// MD10C DIR connected to pin 12
#define DIR 12 

float track=0.45;
float VL;

ros::NodeHandle nh;

std_msgs::Float32 str_message;
ros::Publisher chatter("velocity", &str_message);
void Motor1L(float motion);

float speed_angular=0, speed_linear=0;
void messageCb( const geometry_msgs::Twist& msg){
  speed_angular = msg.angular.z;
  speed_linear = msg.linear.x;
  speed_angular = (speed_angular*track)/(2.0);
  //VR=speed_linear + speed_angular;
  VL= speed_linear - speed_angular;

   Motor1L(VL);

}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb );
 
// Analog pin for potentiometer
int speedcontrol = 0;
 
// Pulse count from encoder
volatile long encoderValue = 0;
 
// One-second interval for measurements
int interval = 1000;
 
// Counters for milliseconds during interval
long previousMillis = 0;
long currentMillis = 0;
 
// Variable for RPM measuerment
int rpm = 0;
 
// Variable for PWM motor speed output
int motorPwm = 0;
 
void setup()
{
  // Setup Serial Monitor
  //Serial.begin(9600); 
  
  // Set encoder as input with internal pullup  
  pinMode(ENC_IN, INPUT_PULLUP); 
 
  // Set PWM and DIR connections as outputs
  pinMode(PWM, OUTPUT);
  pinMode(DIR, OUTPUT);
  
  // Attach interrupt 
  attachInterrupt(digitalPinToInterrupt(ENC_IN), updateEncoder, RISING);
  digitalWrite(PWM, LOW);
  // Setup initial values for timer
  previousMillis = millis();

  nh.initNode();
  nh.advertise(chatter);
 nh.subscribe(sub);
}
 
void loop()
{
  
    // Control motor with potentiometer
    //motorPwm = 200;
    
    // Write PWM to controller
    //analogWrite(PWM, motorPwm);
  
  // Update RPM value every second
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
 
 
    // Calculate RPM
    rpm = (float)(encoderValue * 60 / ENC_COUNT_REV);
    str_message.data = rpm;
 
    // Only update display when there is a reading
    
      Serial.print("PWM VALUE: ");
      Serial.print(motorPwm);
      Serial.print('\t');
      Serial.print(" PULSES: ");
      Serial.print(encoderValue);
      Serial.print('\t');
      Serial.print(" SPEED: ");
      Serial.print(rpm);
      Serial.println(" RPM");
       chatter.publish( &str_message );
    
    
    encoderValue = 0;
  }

  nh.spinOnce();
 delay(1);
}
 
void updateEncoder()
{
  // Increment value for each pulse from encoder
  if(digitalRead(8)==0){
    encoderValue++;  
  }else{
    encoderValue--;
  }
}

void Motor1L(float motion){
if(motion>=0){
  analogWrite(PWM, motion);
  digitalWrite(DIR, HIGH); //clockwise
}else{
  motion=-1*motion;
  analogWrite(PWM, motion);
  digitalWrite(DIR, LOW); //anti-clockwise
}

}
