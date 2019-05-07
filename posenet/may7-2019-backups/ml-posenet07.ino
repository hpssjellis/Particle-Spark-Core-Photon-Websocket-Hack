SYSTEM_MODE(SEMI_AUTOMATIC);
 
 
 
TCPClient client;
 
//Servo MyServo1;
 
 
//char server[] = "bryce-quickie-photon-brycemac.c9users.io"; // Node server needs to allow users to have read access to your url
//char server[] = "byrce01-rocksetta.c9users.io"; // Node server needs to allow users to have read access to your url
char server[] = "mss-ml-club-rocksetta.c9users.io"; // Node server needs to allow users to have read access to your url



bool myRegularDebugOn = false;      // set to false for no publish commands

bool myUsbSerialDebugOn = true;      // set to true  when hooked up to USB Will block code until keypressed
 
 
 

 
 
int mySafetyCount = 0; 
int myMotorSpeed  = 0;  

int connectToMyServer(String myNothing) {
  digitalWrite(D7, HIGH);
  String myRandWebSocket = String(rand()*10000+10000);
  if (client.connect(server, 80)) {
      client.write("GET / HTTP/1.1\r\n");
      client.write("Host: "+String(server)+"\r\n");  
      client.write("Upgrade: websocket\r\n");
      client.write("Connection: Upgrade\r\n");
      client.write("Sec-WebSocket-Key: "+String(myRandWebSocket)+String(myRandWebSocket)+"==\r\n");
      client.write("Sec-WebSocket-Version: 13\r\n");
      client.write("\r\n");
      if (myUsbSerialDebugOn){
         Serial.println("successfully connected");
       }
      if (myRegularDebugOn){ Particle.publish("Connected to server","All Good",60,PRIVATE);} 
      return 1;
  } else {
     if (myUsbSerialDebugOn){
         Serial.println("failed to connect");
      }
    if (myRegularDebugOn){ Particle.publish("No Connection","some issue",60,PRIVATE);} 
     return -1;
  }
}

int stopMyServer(String myNothing) {
    stopAll();  // stop all motors
    digitalWrite(D7, HIGH);
    while(client.read() >= 0);    
    client.stop();               
    digitalWrite(D7, LOW);
    if (myUsbSerialDebugOn){
       Serial.println("successfully disconnected");
        if (myRegularDebugOn){ Particle.publish("Success Disconnecting","from server",60,PRIVATE);} 
    }
    return 1;
}
 
 
 
 
void setup() {

    
    Particle.connect();
    // assume A0 for potentiometer reading
    // assume A4 for DC turning motor speed
    // assume D4 for direction. reverse motor wires if wrong 
    // assume A5 for DC drive motor
    // assume D6 for driving motor forward
    // assume D5 for driving motor reverse
    
    
    
   // pinMode(A0, INPUT);//potentiometer not needed to set                

    pinMode(D4, OUTPUT);//turning motor direction
    pinMode(D5, OUTPUT);//motor 
    pinMode(D6, OUTPUT);//motor
    pinMode(D7, OUTPUT);//LED
    pinMode(A4, OUTPUT);//turning motor
    pinMode(A5, OUTPUT);//Driving motor
  
    
    //MyServo1.attach(D1);//Servo
    
    Spark.function("connect", connectToMyServer);
    Spark.function("stop", stopMyServer);
    randomSeed(analogRead(A0));  
    if (myUsbSerialDebugOn){
       Serial.begin(9600);            
       while(!Serial.available()) SPARK_WLAN_Loop();
       Serial.println("Hello Computer");
    }
    
    // set direction to forward "G"
    digitalWrite(D5, 1); 
    digitalWrite(D6,0);
   // stopAll();             // make sure everything is stopped on boot
    
}  // end setup
 
 
 
//////////////////////////////////// global variables /////////////////////////////// 
 
 
 
 int myTurnSpeed = 0;
 int myTurnNormalSpeed = 100;
 int myTurnMinimum = 50;
 int myTurnMaximum = 4000;      
 int myTurnAmount = 2000;  // straight may have to tune
 int myTurnStraight = 2000;  // straight may have to tune
 
 int myDriveSpeed = 0;
 int myMinDriveSpeed = 20;   // minimum drive speed unless stopped
 int myNormalDriveSpeed = 40;
 int myMaxDriveSpeed = 70;
 
 bool myEmergencyStop = false;
 
 
 
 
 
 
 
 
 
void loop() {
 

  
  if (client.connected()) {
    if (client.available()) {
        
        
       
        char myIncoming = client.read();  // read any commands from webpage
 
    if (myEmergencyStop) {
        myDriveSpeed = 0;
        myTurnSpeed = 0;
        setDirection(myTurnAmount, myTurnSpeed); // stop turning motor
        analogWrite(A5, myDriveSpeed);   // stop drive motor
        if (myIncoming == 'Y'){ myEmergencyStop = false; myDriveSpeed = 0; myTurnAmount = myTurnStraight; myTurnSpeed = 0;}   // this must be on website
       
        
        // stopAll();  // or just call this
        
     } else {
        
        if (myIncoming == 'A'){ digitalWrite(D7, HIGH);}
        if (myIncoming == 'B'){ digitalWrite(D7, LOW);}
        if (myIncoming == 'C'){ RGB.brightness(5); }
        if (myIncoming == 'D'){ RGB.brightness(100); }
        if (myIncoming == 'E'){ RGB.brightness(250); }
        
        


        
        

 
 ///////////////////////////   version 2 starts here
 // All commands from webpage
 
         
        
        // sending these
         // p     q     r
         // s     t     u
         // v     w     x  
 
 
 
 
 
 
        if (myIncoming == 'F'){ digitalWrite(D5, 0); digitalWrite(D6,1); if (myRegularDebugOn){ Particle.publish("D5-0,D6-1","forward",60,PRIVATE);} }
        if (myIncoming == 'G'){ digitalWrite(D5, 1); digitalWrite(D6,0); if (myRegularDebugOn){Particle.publish("D5-1,D6-0","backward",60,PRIVATE); } }

 
        if (myIncoming == 'H'){ myDriveSpeed += 5;  if (myRegularDebugOn){Particle.publish("Faster motor A5 to:",String(myDriveSpeed),60,PRIVATE);} }
        if (myIncoming == 'I'){ myDriveSpeed -= 5;  if (myRegularDebugOn){Particle.publish("Slower motor A5 to:",String(myDriveSpeed),60,PRIVATE);} }
     
 
        if (myIncoming == 'Z'){ myEmergencyStop = true; }
        if (myIncoming == 'Y'){ myEmergencyStop = false; myDriveSpeed = 0; myTurnAmount = myTurnStraight; myTurnSpeed = 0;}   // this must be on website
        
    
    
    
    // 9 commands to work with the webpage
    
    
        if (myIncoming == 'p'){  myDriveSpeed -= 5; myTurnAmount += -200;             myTurnSpeed = myTurnNormalSpeed; }   // this must set on website
        if (myIncoming == 'q'){  myDriveSpeed -= 5;  myTurnAmount = myTurnStraight; myTurnSpeed = myTurnNormalSpeed; }   
        if (myIncoming == 'r'){  myDriveSpeed -= 5; myTurnAmount +=  200;             myTurnSpeed = myTurnNormalSpeed; }   
        
        if (myIncoming == 's'){  myDriveSpeed = myNormalDriveSpeed; myTurnAmount += -200;            myTurnSpeed = myTurnNormalSpeed; }   
        if (myIncoming == 't'){  myDriveSpeed = myNormalDriveSpeed; myTurnAmount = myTurnStraight; myTurnSpeed = myTurnNormalSpeed; }  
        if (myIncoming == 'u'){  myDriveSpeed = myNormalDriveSpeed; myTurnAmount +=  200;            myTurnSpeed = myTurnNormalSpeed; }   

        if (myIncoming == 'v'){  myDriveSpeed += 5; myTurnAmount += -200;            myTurnSpeed = myTurnNormalSpeed; }   
        if (myIncoming == 'w'){  myDriveSpeed += 5; myTurnAmount = myTurnStraight; myTurnSpeed = myTurnNormalSpeed; }   
        if (myIncoming == 'x'){  myDriveSpeed += 5; myTurnAmount +=  200;            myTurnSpeed = myTurnNormalSpeed; }   

   // now each loop redirect the car and set its driving speed     
        
        setDirection(myTurnAmount, myTurnSpeed);   // turning motor to the desired direction
        
     
    if (myDriveSpeed != 0 && myDriveSpeed < myMinDriveSpeed){ myDriveSpeed = myMinDriveSpeed; }     // minimum driving speed;
    if (myDriveSpeed > myMaxDriveSpeed){ myDriveSpeed = myMaxDriveSpeed; }   // maximum driving speed;
    // note: if turn speed is zero it leaves it zero to stop the motor   
        
        
        analogWrite(A5, myDriveSpeed); // set drive motor to the driving speed 
        
        if (myRegularDebugOn){
             Particle.publish("Speed:"+String(myDriveSpeed),  "turn:"+String(myTurnAmount),60,PRIVATE);
             delay(1000);
        }
         
        
        
        
        
        
        
              

        if (myUsbSerialDebugOn){
            Serial.print(myIncoming);
            Serial.print(" DS:");
            Serial.print(myDriveSpeed);
            Serial.print(" TA:");
            Serial.print(myTurnAmount);
            Serial.print(" A0:");
            Serial.println(analogRead(A0));
        }  
  
 
     }  // end emergency stop false 
        
/////////////////////////// version 2 ends here
      
      }   
    }   else {      // if no wifi
          stopAll();
       }
       
   
       
}




void stopAll(){
    myDriveSpeed = 0;
    myTurnSpeed = 0;
    setDirection(myTurnAmount, myTurnSpeed); // stop turning motor
    analogWrite(A5, myDriveSpeed);   // stop drive motor
        
    analogWrite(A4, 0); // stop the DC turn if no wifi
        
}




// must loop through several times before it sets the mark
void setDirection(int myDirection , int myTurnSpeed ){
    // assume A0 for potentiometer reading
    // assume A4 for DC turning motor speed
    // assume D4 for direction. reverse motor wires if wrong
    int myBias = 100;  // what we are OK for as straight

    

    if (myTurnSpeed != 0 && myTurnSpeed < 50){ myTurnSpeed = myTurnMinimum; }     // minimum turning speed;
    if (myTurnSpeed > 255){ myTurnSpeed = 255; }   // maximum turning speed;
    // note: if turn speed is zero it leaves it zero to stop the motor
    
    
    if (myDirection < myTurnMinimum){ myDirection = myTurnMinimum; }         // minimum turning amount
    if (myDirection > myTurnMaximum){ myDirection = myTurnMaximum; }   // maximum turning amount

    
    if (myDirection >= analogRead(A0) + myBias){
        digitalWrite(D4, 1);
        analogWrite(A4, myTurnSpeed);  // slowish speed for turning motor
    } else 
    if (myDirection <= analogRead(A0) - myBias){
        digitalWrite(D4, 0);
        analogWrite(A4, myTurnSpeed);  // slowish speed for turning motor
    }    
    
    
   // else {
   //     analogWrite(A4, 0);  // stop the turning motor  
      // if (myRegularDebugOn){ Particle.publish("Going straight", String(analogRead(A0)), 60, PRIVATE);}
  //  }
    
    
    
    
    delay(5);          // get rid of this soon
    myTurnSpeed = 0;
    analogWrite(A4, 0);  // stop turning motor
}




