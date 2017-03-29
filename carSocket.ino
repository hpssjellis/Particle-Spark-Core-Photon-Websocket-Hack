


TCPClient client;



Servo myServo1;   // create servo object to control a servo


char server[] = "socket01-e2teacher.c9users.io";  // // Node server needs to allow users to have read access to your url



// motor connected to A5
// Motor direction    D5.. D6  sy D5 high forward, D6 high backward
// Servo connected to D1


bool myUsbSerialDebugOn = false;      // set to false when not hooked up to USB
bool myGoNow = false;      // set to false, true to stop motors
int mySafetyCount = 0;    // so car stops if no interent
int myMotorSpeed  = 0;  //0 stop, 255 fastest -255 fast reverse

int myMinDegrees = 60;   // adjust for your turn left
int myStraightDegrees = 110;   // adjust for your turn left
int myMaxDegrees = 140;   // adjust for your turn right


int myServoDegree = myStraightDegrees; // what ever goes straight  from 0 - 180




void mySetMotors(){
    digitalWrite(D7, HIGH);
    mySafetyCount = 0;



   if (myServoDegree < myMinDegrees){ myServoDegree = myMinDegrees;} // limits for servo
   if (myServoDegree > myMaxDegrees){ myServoDegree = myMaxDegrees;}
   
   myServo1.write(myServoDegree);
   

   

    if (myMotorSpeed < -255 ) { myMotorSpeed = -255; } // limits
    if (myMotorSpeed > 255  ) { myMotorSpeed =  255; } 
    
    if (myMotorSpeed >= 0) { digitalWrite(D6, 0); digitalWrite(D5, 1);   }  // go forward
    else {digitalWrite(D5, 0); digitalWrite(D6, 1); }  // backward
    

    analogWrite(A5, abs(myMotorSpeed));
    delay(2);
    digitalWrite(D7, LOW);
}




int connectToMyServer(String myNothing) {

  digitalWrite(D7, HIGH);

  String myRandWebSocket = String(rand()*10000+10000); //attempt at random security

  
  if (client.connect(server, 80)) {
      client.write("GET / HTTP/1.1\r\n");
     // client.write("Host: socket01-e2teacher.c9users.io\r\n");  // note easiest to copy server here
      client.write("Host: "+String(server)+"\r\n");  
      client.write("Upgrade: websocket\r\n");
      client.write("Connection: Upgrade\r\n");
     // client.write("Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n");
      client.write("Sec-WebSocket-Key: "+String(myRandWebSocket)+String(myRandWebSocket)+"==\r\n");
      client.write("Sec-WebSocket-Version: 13\r\n");
      client.write("\r\n");


      if (myUsbSerialDebugOn){
         Serial.println("successfully connected");
       }
       
      myGoNow = false;
      return 1; // successfully connected

  } else {
     if (myUsbSerialDebugOn){
         Serial.println("failed to connect");
      }
     return -1; // failed to connect
  }




}



int stopMyServer(String myNothing) {

    digitalWrite(D7, HIGH);
    mySafetyCount = 4001; 
    myGoNow = false;
    while(client.read() >= 0);    // ignore the rest of the http request
    client.stop();                // shut down the client for next connection
    

    
    digitalWrite(D7, LOW);
    if (myUsbSerialDebugOn){
       Serial.println("successfully disconnected");
    }
    
    
    
    return 1;
    
    
    

    
}





int myGo(String myNothing) {

    digitalWrite(D7, HIGH);
    myGoNow = true;
    
    while(client.read() >= 0){};    // ignore the rest of the http request

    myMotorSpeed = 0;   // set car to stopped
    mySetMotors();
    mySafetyCount = 0;
    
    delay(5);
    digitalWrite(D7, LOW);
    return 1;

}

int myNotGo(String myNothing) {

    digitalWrite(D7, HIGH);
    myGoNow = false;
    delay(50);
    digitalWrite(D7, LOW);
    return 1;
}



void setup() {

    
      myServo1.attach(D1); 
      pinMode(D7, OUTPUT);
      
      pinMode(D3, OUTPUT); // left
      pinMode(D4, OUTPUT); // left
      pinMode(D5, OUTPUT); // right
      pinMode(D6, OUTPUT); // right
      
      pinMode(A4, OUTPUT); // left analog
      pinMode(A5, OUTPUT); // right analog
      
      
      randomSeed(analogRead(A0));

      Particle.function("connect", connectToMyServer);
      Particle.function("stop", stopMyServer);
      Particle.function("myGo1", myGo);
      Particle.function("myNotGo1", myNotGo);

      
      
    if (myUsbSerialDebugOn){
       Serial.begin(9600);             // Initalize the USB Serial port
       while(!Serial.available()) SPARK_WLAN_Loop();
       Serial.println("Hello Computer");
    }

}

void loop() {
 

   mySafetyCount += 1;
   

   if (mySafetyCount >= 1500){  // no connection for a while stop motors
        myMotorSpeed = 0;
        mySetMotors();
        mySafetyCount = 0;
    }
  if (myGoNow){
    if (client.connected()) {
      if (client.available()) {
       

        
        char myIncoming = client.read();

        
        
        if (myIncoming == 'A'){ digitalWrite(D7, HIGH);}
        if (myIncoming == 'B'){ digitalWrite(D7, LOW);}
        if (myIncoming == 'C'){ RGB.brightness(5); }
        if (myIncoming == 'D'){ RGB.brightness(100); }
        if (myIncoming == 'E'){ RGB.brightness(250); }
   
   
   
        if (myIncoming == 'H'){ myMotorSpeed = 0; mySetMotors(); }  // H stop
        if (myIncoming == 'c'){ myMotorSpeed = 110; mySetMotors(); }  // c slow forward
        if (myIncoming == 'g'){ myMotorSpeed = -110; mySetMotors(); }  // g back slow
        
       
        if (myIncoming == '6'){ myMotorSpeed += 10; mySetMotors(); }  // 6 faster
        if (myIncoming == 'a'){ myMotorSpeed += 30; mySetMotors(); }  // 6 more faster 
        
        if (myIncoming == 'e'){ myMotorSpeed -= 10; mySetMotors(); }  // e slower
        if (myIncoming == 'f'){ myMotorSpeed -= 30; mySetMotors(); }  // e more slower possible reverse
        
        
        
        
        
        if (myIncoming == '7'){  myServoDegree = myMinDegrees; mySetMotors(); }  // 7 go left max
        if (myIncoming == '8'){  myServoDegree = myStraightDegrees; mySetMotors(); }  // 8 go straight 
        if (myIncoming == '9'){  myServoDegree = myMaxDegrees; mySetMotors(); }  // 9 go right max
        
        if (myIncoming == '1'){  myServoDegree -= 5; mySetMotors(); }  // 1 more left 
        if (myIncoming == '3'){  myServoDegree += 5; mySetMotors(); }  // 3 more right
        
        if (myIncoming == 'd'){  myServoDegree -= 20; mySetMotors(); }  // 1 more right
        if (myIncoming == 'b'){  myServoDegree += 20; mySetMotors(); }  // 3 more left
        
        
        if (myIncoming == 'z'){  mySetMotors();  }  // continue
  
  
  

  
  
  
  
  
     
        
        if (myUsbSerialDebugOn){
            Serial.print(myIncoming);
        }  
  

      
      }   
    }
  }   //mygo
}
