SYSTEM_MODE(SEMI_AUTOMATIC);
 
 
 
TCPClient client;
 
//Servo MyServo1;
 
 
//char server[] = "bryce-quickie-photon-brycemac.c9users.io"; // Node server needs to allow users to have read access to your url
//char server[] = "byrce01-rocksetta.c9users.io"; // Node server needs to allow users to have read access to your url
char server[] = "mss-ml-club-rocksetta.c9users.io"; // Node server needs to allow users to have read access to your url



bool myRegularDebugOn = true;      // set to false for no publish commands

bool myUsbSerialDebugOn = false;      // set to true  when hooked up to USB Will block code until keypressed
 
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
          if (myRegularDebugOn){ Particle.publish("Connected to server","All Good",60,PRIVATE);} 
       }
      return 1;
  } else {
     if (myUsbSerialDebugOn){
         Serial.println("failed to connect");
          if (myRegularDebugOn){ Particle.publish("No Connection","some issue",60,PRIVATE);} 
      }
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
}
 
 
 int mySpeed = 0;
 int myMinSpeed = 50;
 int myMaxSpeed = 100;
 
void loop() {
 
  
  
  if (client.connected()) {
    if (client.available()) {
        
        
        if (mySpeed < myMinSpeed ){mySpeed = myMinSpeed;}
        if (mySpeed > myMaxSpeed ){mySpeed = myMaxSpeed;}
        
        
       
        char myIncoming = client.read();
 
        
        
        if (myIncoming == 'A'){ digitalWrite(D7, HIGH);}
        if (myIncoming == 'B'){ digitalWrite(D7, LOW);}
        if (myIncoming == 'C'){ RGB.brightness(5); }
        if (myIncoming == 'D'){ RGB.brightness(100); }
        if (myIncoming == 'E'){ RGB.brightness(250); }
        
        // we need to write these
        
        
        if (myIncoming == 'F'){ digitalWrite(D5, 0); digitalWrite(D6,1); if (myRegularDebugOn){ Particle.publish("D5-0,D6-1","forward",60,PRIVATE);} }
        if (myIncoming == 'G'){ digitalWrite(D5, 1); digitalWrite(D6,0); if (myRegularDebugOn){Particle.publish("D5-1,D6-0","backward",60,PRIVATE); } }
        if (myIncoming == 'H'){ mySpeed += 5; analogWrite(A5, mySpeed);  if (myRegularDebugOn){Particle.publish("Faster motor A5 to:",String(mySpeed),60,PRIVATE);} }
        if (myIncoming == 'I'){ mySpeed -= 5; analogWrite(A5, mySpeed);  if (myRegularDebugOn){Particle.publish("Slower motor A5 to:",String(mySpeed),60,PRIVATE);} }
        if (myIncoming == 'Z'){ mySpeed = 0; stopAll();  if (myRegularDebugOn){Particle.publish("Stopping both motors",String(mySpeed),60,PRIVATE);} }



      //  if (myIncoming == 'J'){ digitalWrite(D2, 0); digitalWrite(D3,1); Particle.publish("D5-0,D6-1","left",60,PRIVATE); }
      //  if (myIncoming == 'K'){ digitalWrite(D2, 1); digitalWrite(D3,0); Particle.publish("D5-1,D6-0","right",60,PRIVATE); }
      //  if (myIncoming == 'L'){ mySpeed += 5; analogWrite(A5, mySpeed);  Particle.publish("faster turn A5 to:",String(mySpeed),60,PRIVATE); }
      //  if (myIncoming == 'M'){ mySpeed -= 5; analogWrite(A5, mySpeed);  Particle.publish("Slower turn A5 to:",String(mySpeed),60,PRIVATE); }

        if (myIncoming == 'J'){   // go left
          // for (int x=0; x <= 200; x++){
               setDirection(1000, 100);
          //     delay(20);       
          // }
          if (myRegularDebugOn){ Particle.publish("setDirection(1000, 100)","left",60,PRIVATE);}
        }

        if (myIncoming == 'K'){  // go right
        //   for (int x=0; x <= 200; x++){
               setDirection(3000, 100);
          //     delay(20);       
          // }
          if (myRegularDebugOn){ Particle.publish("setDirection(3000, 100)","right",60,PRIVATE);}
        }


        if (myUsbSerialDebugOn){
            Serial.print(myIncoming);
        }  
  
 
      
      }   
    }   else { 
          stopAll();
       }
}




void stopAll(){
    analogWrite(A5, 0); // stop the DC motor if no wifi
    analogWrite(A4, 0); // stop the DC turn if no wifi
}




// must loop through several times before it sets the mark
void setDirection(int myDirection , int mySpeed ){
    // assume A0 for potentiometer reading
    // assume A4 for DC turning motor speed
    // assume D4 for direction. reverse motor wires if wrong
    int myBias = 40;  // what we are OK for as straight
    
    if (mySpeed < 50){ mySpeed = 50; }     // minimum turning speed;
    if (mySpeed > 255){ mySpeed = 255; }   // maximum turning speed;
    
    if (myDirection < 0){ myDirection = 0; }         // minimum turning amount
    if (myDirection > 4095){ myDirection = 4095; }   // maximum turning amount

    
    if (myDirection >= analogRead(A0) + myBias){
        digitalWrite(D4, 1);
        analogWrite(A4, mySpeed);  // slowish speed for turning motor
    } else 
    if (myDirection <= analogRead(A0) - myBias){
        digitalWrite(D4, 0);
        analogWrite(A4, mySpeed);  // slowish speed for turning motor
    }    else {
        analogWrite(A4, 0);  // stop the turning motor  
       if (myRegularDebugOn){ Particle.publish("Going stright", String(analogRead(A0)), 60, PRIVATE);}
    }
    
    
}




