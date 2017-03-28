TCPClient client;



char server[] = "socket01-e2teacher.c9users.io";  // set again later!!!


bool myUsbSerialDebugOn = false;      // set to false when not hooked up to USB
bool myGoNow = false;      // set to false, true to stop motors

int mySafetyCount = 0;

int myLeftMotorDirection  = 1;  // 1 forward, 2 backward
int myRightMotorDirection = 1;  // 1 forward, 2 backward
int myBrake = 0; // 0 no brake, 1 coast, 2 power brake;

int myLeftMotorSpeed  = 0;  //0 stop, 255 fastest
int myRightMotorSpeed = 0;  //0 stop, 255 fastest

void mySetMotors(){
    digitalWrite(D7, HIGH);
    mySafetyCount = 0;
    if (myLeftMotorDirection  == 1){ digitalWrite(D3, 1); digitalWrite(D4, 0);} //left forward
    if (myLeftMotorDirection  == 2){ digitalWrite(D3, 0); digitalWrite(D4, 1);} 
   
    if (myRightMotorDirection  == 1){ digitalWrite(D5, 1); digitalWrite(D6, 0);} // right forward
    if (myRightMotorDirection  == 2){ digitalWrite(D5, 0); digitalWrite(D6, 1);}  
   
   
   
    if (myBrake == 1) {digitalWrite(D3, 0); digitalWrite(D4, 0); digitalWrite(D5, 0); digitalWrite(D6, 0);} // coast
    if (myBrake == 2) {digitalWrite(D3, 1); digitalWrite(D4, 1); digitalWrite(D5, 1); digitalWrite(D6, 1);} // power brake
   
   
   

    if (myLeftMotorSpeed < 0   ){myLeftMotorSpeed = 0;   } // limits
    if (myLeftMotorSpeed > 255 ){myLeftMotorSpeed = 255; } 
    
    if (myRightMotorSpeed < 0  ){myRightMotorSpeed = 0;  }
    if (myRightMotorSpeed > 255){myRightMotorSpeed = 255;}
    
    analogWrite(A4, myLeftMotorSpeed); // set motor speeds
    analogWrite(A5, myRightMotorSpeed);
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

    myBrake=0;    // reset everything
    myLeftMotorDirection=1; 
    myRightMotorDirection=1;
    myLeftMotorSpeed = 0;
    myRightMotorSpeed = 0;
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
      digitalWrite(D7, HIGH);
      delay(25000);
      digitalWrite(D7, LOW);
      
      
    if (myUsbSerialDebugOn){
       Serial.begin(9600);             // Initalize the USB Serial port
       while(!Serial.available()) SPARK_WLAN_Loop();
       Serial.println("Hello Computer");
    }

}

void loop() {
 

   mySafetyCount += 1;
   

   if (mySafetyCount >= 1500){  // no connection for a while stop motors
        myBrake=0; 
        myLeftMotorDirection=1; 
        myRightMotorDirection=1;
        myLeftMotorSpeed = 0;
        myRightMotorSpeed = 0;
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
        if (myIncoming == 'F'){ myBrake=0; myLeftMotorDirection=1; myRightMotorDirection=2; myLeftMotorSpeed=0; myRightMotorSpeed=50; mySetMotors(); }  // F rotate right
        if (myIncoming == 'G'){ myBrake=0; myLeftMotorDirection=2; myRightMotorDirection=1; myLeftMotorSpeed=50; myRightMotorSpeed=0; mySetMotors(); }  // G rotate left

        if (myIncoming == 'H'){ myLeftMotorSpeed=0; myRightMotorSpeed=0; mySetMotors(); }  // H stop
        if (myIncoming == 'I'){ myLeftMotorSpeed=50; myRightMotorSpeed=50; mySetMotors(); }  // I slow
        if (myIncoming == 'J'){ myLeftMotorSpeed=100; myRightMotorSpeed=100; mySetMotors(); }  // J OK
        if (myIncoming == 'K'){ myLeftMotorSpeed=150; myRightMotorSpeed=150; mySetMotors(); }  // K medium   

        if (myIncoming == 'L'){ myLeftMotorSpeed=200; myRightMotorSpeed=200; mySetMotors(); }  // L fast
        if (myIncoming == 'M'){ myLeftMotorSpeed=255; myRightMotorSpeed=255; mySetMotors(); }  // M fastest

        if (myIncoming == 'Q'){ myBrake=0; myLeftMotorSpeed+=10;  mySetMotors(); }  // Q left
        if (myIncoming == 'R'){ myBrake=0; myLeftMotorSpeed+=50;  mySetMotors(); }  // R big left
        if (myIncoming == 'S'){ myBrake=0; myRightMotorSpeed+=10; mySetMotors(); }  // S right
        if (myIncoming == 'T'){ myBrake=0; myRightMotorSpeed+=50; mySetMotors(); }  // T big right

        if (myIncoming == '1'){ myBrake=0; myLeftMotorSpeed-=10;  mySetMotors(); }  // Q left
        if (myIncoming == '2'){ myBrake=0; myLeftMotorSpeed-=50;  mySetMotors(); }  // R big left
        if (myIncoming == '3'){ myBrake=0; myRightMotorSpeed-=10; mySetMotors(); }  // S right
        if (myIncoming == '4'){ myBrake=0; myRightMotorSpeed-=50; mySetMotors(); }  // T big right
        if (myIncoming == '5'){ myBrake=0; myLeftMotorSpeed -=10; myRightMotorSpeed-=10; mySetMotors(); }  // 5 slower
        if (myIncoming == '6'){ myBrake=0; myLeftMotorSpeed +=10; myRightMotorSpeed+=10; mySetMotors(); }  // 6 faster


        if (myIncoming == 'U'){ myBrake=1; mySetMotors(); }  // U coast
        if (myIncoming == 'V'){ myBrake=2; mySetMotors(); }  // V power break
        if (myIncoming == 'W'){ myBrake=0; myLeftMotorDirection=1; myRightMotorDirection=1; mySetMotors(); }  // W set forward
        if (myIncoming == 'X'){ myBrake=0; myLeftMotorDirection=2; myRightMotorDirection=2; mySetMotors(); }  // X set backward
     
        
        if (myUsbSerialDebugOn){
            Serial.print(myIncoming);
        }  
  

      
      }   
    }
  }   //mygo
}
