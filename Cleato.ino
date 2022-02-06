#include <IRremote.h>
int RECV_PIN = A5;
IRrecv irrecv(RECV_PIN);
decode_results results;


#include <Servo.h>
Servo myservo;
int pos = 0;


#define motor_speed_1 5// Enable L298 Pin  
#define motor_speed_2 6
#define in1 13 //Motor1  L298 Pin in1 
#define in2 12 //Motor1  L298 Pin in1 
#define in3 11 //Motor2  L298 Pin in1 
#define in4 10 //Motor2  L298 Pin in1 
// #define enB 5 //Enable2 L298 Pin enB 
#define buzz 2
#define pir 3
#define echo A2    //Echo pin
#define trigger A3 //Trigger pin


int left= 1100;//335
int right=1050;//325

int speed=205; //120
int speed_=255; //200
int IR_data;
int ir_data;
int mode;
int left_distance, forword_distance = 30, right_distance;
int limit=40;
int distance;
int direction=1;
int temp=0;



void setup(){
    irrecv.enableIRIn();
    myservo.attach(A4);
    Serial.begin(9600);

    pinMode(buzz,OUTPUT);
    pinMode(pir,INPUT);


    pinMode(echo, INPUT );// declare ultrasonic sensor Echo pin as input
    pinMode(trigger, OUTPUT); // declare ultrasonic sensor Trigger pin as Output  

    pinMode(motor_speed_1, OUTPUT);
    pinMode(motor_speed_2, OUTPUT);
    pinMode(in1, OUTPUT); // declare as output for L298 Pin in1 
    pinMode(in2, OUTPUT); // declare as output for L298 Pin in2 
    pinMode(in3, OUTPUT); // declare as output for L298 Pin in3   
    pinMode(in4, OUTPUT); // declare as output for L298 Pin in4

}
void loop(){

if (irrecv.decode(&results)) {
    Serial.println(results.value);
    ir_data = IRremote_data();
    Serial.println(ir_data); 
    irrecv.resume(); // Receive the next value
    delay(100);
    }

if(ir_data == 5){mode=0; stop();}  // stop
else if(ir_data == 9){mode=1;} // Remote control mode
else if(ir_data ==10){mode=2;} // Automatic mode

analogWrite(motor_speed_1, speed);
analogWrite(motor_speed_2, speed_);

forword_distance = Ultrasonic_read();

if (digitalRead(pir)==HIGH && forword_distance < 40)
                {
                    stop();
                    digitalWrite(buzz,HIGH);
                    delay(100);
                    digitalWrite(buzz,LOW);
                }

else if(mode==1){     // Remote controlled mode 

    if(ir_data == 1){forword_distance = Ultrasonic_read();

           for (forword_distance;forword_distance > 40;forword_distance = Ultrasonic_read())
            {
                
            forword();
            delay(100);
            temp=1;
            }
            
            backword();
            delay(70);
            if (temp==1)
            {
                ir_data=8;
            }
            
            stop();

            }  // if the ir_data is '1' the DC motor will go forward
    else if(ir_data == 2){backword();}  // if the ir_data is '2' the motor will Reverse
    else if(ir_data == 3){turnright(); delay(270); ir_data = 8;analogWrite(motor_speed_1, speed);analogWrite(motor_speed_2, speed_);}  // if the ir_data is '3' the motor will turn right
    else if(ir_data == 4){turnleft(); delay(280); ir_data = 8;analogWrite(motor_speed_1, speed);analogWrite(motor_speed_2, speed_);} // if the ir_data is '4' the motor will turn left
    if(ir_data == 8){stop(); }     // if the ir_data '5' the motor will Stop
}

else if(mode==2){  
    delay(500);

 forword_distance = Ultrasonic_read();
 Serial.print("S=");Serial.println(forword_distance);
  if (forword_distance > limit){


        analogWrite(motor_speed_1, speed);
        analogWrite(motor_speed_2, speed_);
        

      if (direction==1)
      {
        analogWrite(motor_speed_1, speed);
        analogWrite(motor_speed_2, speed_);

        //   forword();
            forword_distance = Ultrasonic_read();

           for (forword_distance;forword_distance > 60;forword_distance = Ultrasonic_read())
            {
                forword();
                delay(100);
            }
            backword();
            delay(100);
        stop();
        delay(200);
        direction=0;
      }
      

      if (direction==5 || direction==6)
      {
          analogWrite(motor_speed_1, speed);
        analogWrite(motor_speed_2, speed_);

        //   forword();
            forword_distance = Ultrasonic_read();

           for (forword_distance;forword_distance > limit;forword_distance = Ultrasonic_read())
            {
                forword();
                delay(100);
            }
            backword();
            delay(50);
        stop();
        delay(200);
        // direction=0;
      }
      
      
      if (direction == 3)
      {
          analogWrite(motor_speed_2, speed_);
          analogWrite(motor_speed_1, speed);

          forword();
          delay(350);
          stop();
          delay(350);
          turnright();
          delay(right+20);
          stop();
          delay(100);
          analogWrite(motor_speed_2, speed_);
          analogWrite(motor_speed_1, speed);

          direction=5;
        //   forword();
      }
      if (direction == 4)
      {
          analogWrite(motor_speed_2, speed_);
          analogWrite(motor_speed_1, speed);

          forword();
          delay(350);
          stop();
          delay(350);
          turnleft();
          delay(left+20);
          stop();
          delay(100);
          analogWrite(motor_speed_2, speed_);
          analogWrite(motor_speed_1, speed);

          direction=6;
        //   forword();
      }

      



  }
    else if(forword_distance<limit){

        stop();
        delay(500);
        backword();
        delay(150);
        if (direction==0)
        {
            check_side();
        }
        else if (direction==5)
        {
            turnleft();
            delay(left);
            analogWrite(motor_speed_2, speed_);
            analogWrite(motor_speed_1, speed);

            direction=4;
            stop();
        }
        else if (direction==6)
        {
            turnright();
            delay(right);
            analogWrite(motor_speed_2, speed_);
            analogWrite(motor_speed_1, speed);

            direction=3;
            stop();
        }
        else{
            ir_data=5;
        }
        
        


    }
    // else{
    //     forword_distance = Ultrasonic_read();
    //     Serial.print("S=");Serial.println(forword_distance);
    // }
}
    
    
    
    }
/*******************************************************************************************************/
// Functions



long IRremote_data(){
         if(results.value==2719332322){IR_data=1;}  //frount
    else if(results.value==2825787859){IR_data=2;} //back
    else if(results.value==3463484474){IR_data=3;} //right 
    else if(results.value==3480262093){IR_data=4;} //left
    else if(results.value==2659543774){IR_data=5;} //select
    else if(results.value==535975662){IR_data=8;} //4 no
    else if(results.value==3757616022){IR_data=9;} //5 no
    else if(results.value==1557726997){IR_data=10;} //6 no
    return IR_data;
}

long Ultrasonic_read(){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  distance = pulseIn (echo, HIGH);
  return distance / 29 / 2;
}


void forword(){  //forword
digitalWrite(in1, HIGH); //Right Motor forword Pin 
digitalWrite(in2, LOW);  //Right Motor backword Pin 
digitalWrite(in3, LOW);  //Left Motor backword Pin 
digitalWrite(in4, HIGH); //Left Motor forword Pin 
}

void backword(){ //backword
digitalWrite(in1, LOW);  //Right Motor forword Pin 
digitalWrite(in2, HIGH); //Right Motor backword Pin 
digitalWrite(in3, HIGH); //Left Motor backword Pin 
digitalWrite(in4, LOW);  //Left Motor forword Pin 
}

void turnright(){ //Right

analogWrite(motor_speed_2,255);
analogWrite(motor_speed_1,255);
delay(100);

digitalWrite(in1, HIGH); //Right Motor forword Pin 
digitalWrite(in2, LOW);  //Right Motor backword Pin 
digitalWrite(in3, HIGH); //Left Motor backword Pin 
digitalWrite(in4, LOW);  //Left Motor forword Pin 

}

void turnleft(){ //Left

analogWrite(motor_speed_2,255);
analogWrite(motor_speed_1,255);
delay(100);

digitalWrite(in1, LOW);  //Right Motor forword Pin 
digitalWrite(in2, HIGH); //Right Motor backword Pin  
digitalWrite(in3, LOW);  //Left Motor backword Pin 
digitalWrite(in4, HIGH); //Left Motor forword Pin 

}

void stop(){ //stop
digitalWrite(in1, LOW); //Right Motor forword Pin 
digitalWrite(in2, LOW); //Right Motor backword Pin 
digitalWrite(in3, LOW); //Left Motor backword Pin 
digitalWrite(in4, LOW); //Left Motor forword Pin 
}


void check_side(){
    stop();
    delay(100);
 for (int angle = 90; angle <= 175; angle += 5)  {
     myservo.write(angle);
     delay(15);
    }
    delay(300);
    left_distance = Ultrasonic_read();
    delay(100);
  for (int angle = 175; angle >= 5; angle -= 5)  {
     myservo.write(angle);
     delay(15);  
    }
    delay(500);
    right_distance = Ultrasonic_read();
    delay(100);
 for (int angle = 0; angle <= 90; angle += 5)  {
     myservo.write(angle);
     delay(15);
    }
    delay(300);
    compareDistance();
}

void compareDistance(){
       if (left_distance >= right_distance && left_distance > limit){
  turnleft();
  delay(left);
  analogWrite(motor_speed_2, speed_);
  analogWrite(motor_speed_1, speed);

  direction=4;
  stop();
  }
  else if (right_distance > left_distance && right_distance > limit){
  turnright();
  delay(right);
  analogWrite(motor_speed_2, speed_);
  analogWrite(motor_speed_1, speed);

  direction=3;
  stop();
  }
  else{
  backword();
  delay(300);
  turnright();
  analogWrite(motor_speed_2, speed_);
  analogWrite(motor_speed_1, speed);

  delay(600);
  direction=0;
  stop();
  }
}