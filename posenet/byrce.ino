SYSTEM_MODE(SEMI_AUTOMATIC);
 
 
 
TCPClient client;
 
Servo MyServo1;
 
 
//char server[] = "bryce-quickie-photon-brycemac.c9users.io"; // Node server needs to allow users to have read access to your url
char server[] = "byrce01-rocksetta.c9users.io"; // Node server needs to allow users to have read access to your url




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
       }
      return 1;
  } else {
     if (myUsbSerialDebugOn){
         Serial.println("failed to connect");
      }
     return -1;
  }
}
int stopMyServer(String myNothing) {
    digitalWrite(D7, HIGH);
    while(client.read() >= 0);    
    client.stop();               
    digitalWrite(D7, LOW);
    if (myUsbSerialDebugOn){
       Serial.println("successfully disconnected");
    }
    return 1;
}
 
 
 
 
void setup() {
    
    Particle.connect();
    
    
    pinMode(D0, OUTPUT);//motor                   // motor connections -- pin 1- D5 pin, 3-D1, 5-D5, 6-3v3, 7-gnd.
    pinMode(D2, OUTPUT);//Free
    pinMode(D5, OUTPUT);//motor 
    pinMode(D6, OUTPUT);//motor
    pinMode(D7, OUTPUT);//LED
    pinMode(A3, OUTPUT);//Free
    pinMode(A5, OUTPUT);//Free
  
    
    MyServo1.attach(D1);//Servo
    
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
 int myMaxSpeed = 120;
 
void loop() {
 
  
  
  if (client.connected()) {
    if (client.available()) {
        
        
        if (mySpeed < 0 ){mySpeed = 0;}
        if (mySpeed > myMaxSpeed ){mySpeed = myMaxSpeed;}
        
        
       
        char myIncoming = client.read();
 
        
        
        if (myIncoming == 'A'){ digitalWrite(D7, HIGH);}
        if (myIncoming == 'B'){ digitalWrite(D7, LOW);}
        if (myIncoming == 'C'){ RGB.brightness(5); }
        if (myIncoming == 'D'){ RGB.brightness(100); }
        if (myIncoming == 'E'){ RGB.brightness(250); }
        
        // we need to write these
        
        
        if (myIncoming == 'F'){ digitalWrite(D5, 0); digitalWrite(D6,1); Particle.publish("D5-0,D6-1","forward",60,PRIVATE); }
        if (myIncoming == 'G'){ digitalWrite(D5, 1); digitalWrite(D6,0); Particle.publish("D5-1,D6-0","backward",60,PRIVATE); }
        if (myIncoming == 'H'){ mySpeed += 5; analogWrite(A5, mySpeed);  Particle.publish("Faster motor A5 to:",String(mySpeed),60,PRIVATE); }
        if (myIncoming == 'I'){ mySpeed -= 5; analogWrite(A5, mySpeed);  Particle.publish("Slower motor A5 to:",String(mySpeed),60,PRIVATE); }



        if (myIncoming == 'J'){ digitalWrite(D2, 0); digitalWrite(D3,1); Particle.publish("D5-0,D6-1","left",60,PRIVATE); }
        if (myIncoming == 'K'){ digitalWrite(D2, 1); digitalWrite(D3,0); Particle.publish("D5-1,D6-0","right",60,PRIVATE); }
        if (myIncoming == 'L'){ mySpeed += 5; analogWrite(A2, mySpeed);  Particle.publish("faster turn A2 to:",String(mySpeed),60,PRIVATE); }
        if (myIncoming == 'M'){ mySpeed -= 5; analogWrite(A2, mySpeed);  Particle.publish("Slower turn A2 to:",String(mySpeed),60,PRIVATE); }

        if (myUsbSerialDebugOn){
            Serial.print(myIncoming);
        }  
  
 
      
      }   
    }   else { 
        analogWrite(A4, 0); // stop the motor if no wifi
        analogWrite(A2, 0); // stop the turn if no wifi
       }
}
