//Cutting Machine
//Written by LIU Botao
//simplified
//Sep.2017
//ヽ(￣▽￣)ﾉ

#include <Servo.h>
//steppers for going up and down
#define stp 2  //motor will move by this pulse
#define dir 3  // control clockwise or anti-clockwise. LOW - up, HIGH - down
#define MS1 4  // MS1 and MS2 control the step size 
#define MS2 5 
/*1   1/2   1/4   1/8
 *LL   HL   LH    HH
*/

//stepper for the plate
#define stpa 10
#define dira 12 //HIGH - forward
#define MS1a 11
#define MS2a 9

// IR sensor to get the distence to the plate
#define sensor 0  
#define S1 23 // avoid cut plate
#define S2 22 //avoid plate over forward
#define S3 24 // avoid cut jiazi
#define force 6
//About attaching

Servo myservo;  // create servo object to control a servo
//Declare variables for functions

char user_input;
int x;
int y;
int state;
int pos = 5; //initial position for the servo moter 
int pressure;
int val1;
int val2;
int val3;
int val;   // caculate the average value of the distence to reduce error

//Function prototype
void oneslice();
void slices();
void cutandlow();
void pf1();
void pf12();
void pf14();
void pf18();
void pb1();
void pb12();
void pb14();
void pb18();
void goup();
void goup12();
void goup14();
void goup18();
void godown();
void godown12();
void godown14();
void godown18();
void cutonly();
void getdistance();
void getforce();
void resetEDPins();
void setup() {
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(stpa,OUTPUT);
  pinMode(dira, OUTPUT);
  pinMode(MS1a,OUTPUT);
  pinMode(MS2a,OUTPUT);
  resetEDPins(); //Set step, direction, microstep and enable pins to default states
  Serial.begin(9600); //Open Serial connection for debugging
  Serial.println("Begin motor control");
  Serial.println();
  //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("a. Move the plate forward 1");//1. Turn at default microstep mode.
  Serial.println("b. Move the plate forward 1/2");//Reverse direction at default microstep mode.
  Serial.println("c. Move the plate forward 1/4");
  Serial.println("d.Move the plate forward 1/8");
  Serial.println("e. Move the plate backward 1");//1. Turn at default microstep mode.
  Serial.println("f. Move the plate backward 1/2");//Reverse direction at default microstep mode.
  Serial.println("g. Move the plate backward 1/4");
  Serial.println("h. Move the plate backward 1/8");
  Serial.println("1.Go up");
  Serial.println("2.Go up 1/2");
  Serial.println("3.Go up 1/4");
  Serial.println("4.Go up 1/8");
  Serial.println("5.Go down 1");
  Serial.println("6.Go down 1/2");
  Serial.println("7.Go down 1/4");
  Serial.println("8.Go down 1/8");
  Serial.println("9.Get Force");
  Serial.println("0.Get Distance");
  Serial.println("z.Cut only");
  Serial.println("SPACE for cut into slices!");
  
  myservo.attach(7);  
}

//Main loop
void loop() {
  while(Serial.available()){
      user_input = Serial.read(); //Read user input and trigger appropriate function
      if (user_input =='a')
      {
         pf1();
      }
      else if(user_input =='b')
      {
        pf12();
      }
      else if(user_input =='c')
      {
        pf14();
      }
      else if(user_input =='d')
      {
        pf18();
      }
       else if (user_input =='e')
      {
         pb1();
      }
      else if(user_input =='f')
      {
        pb12();
      }
      else if(user_input =='g')
      {
        pb14();
      }
      else if(user_input =='h')
      {
        pb18();
      }
      else if(user_input =='1')
      {
        goup();
      }
      else if(user_input =='2')
      {
        goup12();
        }

      else if (user_input =='3')
      {
        goup14();
        }
      else if (user_input== '4')
        {
          goup18();
          }
      else if (user_input=='5')
      {
        godown();
        }
        else if (user_input=='6')
      {
        godown12();
        }
        else if (user_input=='7')
      {
        godown14();
        }
        else if (user_input=='8')
      {
        godown18();
        }
        else if (user_input=='9')
      {
        getforce();
        }
        else if (user_input=='0')
      {
        getdistance();
        }
        else if (user_input==' ')
      {
         slices();
      }
      else if(user_input =='z')
      {
        cutonly();
      }
      else 
      {
        Serial.println("Invalid option entered.");
      }
      resetEDPins();
  }
}
//Default microstep mode function



void pf1()
{
  Serial.println("Move the plate forward 1");
  digitalWrite(dira, HIGH); //Pull direction pin low to move "forward"
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stpa,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stpa,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
 
  
  Serial.println("Enter new option");
  Serial.println();
}


void pf12()
{
  Serial.println("Move the plate forward 1/2");
  digitalWrite(dira,HIGH ); 
  digitalWrite(MS1a, HIGH);
  digitalWrite(MS2a, LOW);
  for(x= 1; x<1000; x++)  //Loop the stepping enough times for motion to be visible
  {
    digitalWrite(stpa,HIGH); //Trigger one step
    delay(1);
    digitalWrite(stpa,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  Serial.println("Enter new option");
  Serial.println();
}

void pf14()
{
  Serial.println("Stepping at 1/4th microstep mode.");
  digitalWrite(dira, HIGH); //Pull direction pin low to move "forward"
  digitalWrite(MS1a, LOW); //Pull MS1, and MS2 high to set logic to 1/4th microstep resolution
  digitalWrite(MS2a, HIGH);
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stpa,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stpa,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  cutonly();
  Serial.println("Enter new option");
  Serial.println();
}

void pf18()
{
  Serial.println("Stepping at 1/8th microstep mode.");
  digitalWrite(dira, HIGH); //Pull direction pin low to move "forward"
  digitalWrite(MS1a, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
  digitalWrite(MS2a, HIGH);
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stpa,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stpa,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  cutonly();
  Serial.println("Enter new option");
  Serial.println();
}

void pb1()
{
  Serial.println("Move the plate forward 1");
  digitalWrite(dira, LOW); //Pull direction pin low to move "forward"
  digitalWrite(MS1a, LOW);
  digitalWrite(MS1a, LOW);
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stpa,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stpa,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
 
  
  Serial.println("Enter new option");
  Serial.println();
}
//Reverse default microstep mode function
void pb12()
{
  Serial.println("Move the plate forward 1/2");
  digitalWrite(dira,LOW); 
  digitalWrite(MS1a, HIGH);
  digitalWrite(MS2a, LOW);
  for(x= 1; x<1000; x++)  //Loop the stepping enough times for motion to be visible
  {
    digitalWrite(stpa,HIGH); //Trigger one step
    delay(1);
    digitalWrite(stpa,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  Serial.println("Enter new option");
  Serial.println();
}

// 1/4th microstep foward mode function
void pb14()
{
  Serial.println("Stepping at 1/4th microstep mode.");
  digitalWrite(dira, LOW); //Pull direction pin low to move "forward"
  digitalWrite(MS1a, LOW); //Pull MS1, and MS2 high to set logic to 1/4th microstep resolution
  digitalWrite(MS2a, HIGH);
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stpa,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stpa,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  cutonly();
  Serial.println("Enter new option");
  Serial.println();
}

void pb18()
{
  Serial.println("Stepping at 1/8th microstep mode.");
  digitalWrite(dira, LOW); //Pull direction pin low to move "forward"
  digitalWrite(MS1a, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
  digitalWrite(MS2a, HIGH);
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stpa,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stpa,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  cutonly();
  Serial.println("Enter new option");
  Serial.println();
}

void goup()
{
  Serial.println("Go up 1");
  digitalWrite(MS1,LOW);
  digitalWrite(dir, LOW); //Pull direction pin low to move "up"
  digitalWrite(MS2,LOW);
 for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
}
void goup12()
{
  Serial.println("Go up 1/2");
  digitalWrite(dir, LOW); //Pull direction pin low to move "up"
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2,LOW);
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
}

void goup14()
{
  Serial.println("Go up 1/4");
  digitalWrite(dir, LOW); //Pull direction pin low to move "up"
  digitalWrite(MS1, LOW);
  digitalWrite(MS2,HIGH);
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
}
void goup18()
{
  Serial.println("Go up 1/8");
  digitalWrite(dir, LOW); //Pull direction pin low to move "up"
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2,HIGH);
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
}
void godown()
{
  Serial.println("Go down 1");
  digitalWrite(dir, HIGH); //Pull direction pin low to move "down"
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
}
  void godown12()
{
  Serial.println("Go down 1/2");
  digitalWrite(dir, HIGH); //Pull direction pin low to move "down"
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, LOW);
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }}
   void godown14()
{
  Serial.println("Go down 1/4");
  digitalWrite(dir, HIGH); //Pull direction pin low to move "down"
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, HIGH);
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
}
   void godown18()
{
  Serial.println("Go down 1/8");
  digitalWrite(dir, HIGH); //Pull direction pin low to move "down"
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }  
  Serial.println("Enter new option");
  Serial.println();
}

void getdistance()
{
  val1 =analogRead(0);
  delay(10);
  val2 =analogRead(0);
  delay(10);
  val3 =analogRead(0);
  val= (val1+val2+val3)/3;
  Serial.println(val);
  }
void getforce()
{
  pressure=analogRead(6);
  Serial.println(pressure);
  delay(100);
}
void slices()
{
  while (digitalRead(S3) && digitalRead(S2))
  {
    oneslice();
    pf18();
    }
  goup12();
  pb1();
  }
void oneslice()
{
  getdistance();
  while (val>550)
  {
    godown();
    getdistance();
    }
    
  while (val>500)
  {
    godown12();
    getdistance();
    }
  while (val>435)
  {
    godown18();
    getdistance();
    }
   while (digitalRead(S1)) 
   {
    cutandlow();
    }
    goup();
    goup12();
  }
void cutandlow()
{  
  digitalWrite(dir, HIGH);
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  for(x= 0; x<=1200 && digitalRead(S1); x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
    if(x%300==0) myservo.write(30);
    else if((x+150)%300 == 0) myservo.write(5);
  }
  cutonly();
  }
    
 //reset
void resetEDPins()
{
  digitalWrite(stpa, LOW);
  digitalWrite(dira, LOW);
  digitalWrite(MS1a, LOW);
  digitalWrite(MS2a, LOW);
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  myservo.write(5);
}

//cut
void cutonly(){
for (int loopn=0; loopn<5; loopn++){
 for (pos = 5; pos <= 35; pos += 1) { 
   
    myservo.write(pos);             
    delay(8);                       
  }
  for (pos = 35; pos >= 0; pos -= 1) { 
    myservo.write(pos);              
    delay(8);                       
  }
 }
}
