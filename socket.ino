TCPClient client;


char server[] = "node-ws-test-jerteach.c9.io";


bool myUsbSerialDebugOn = false;      // set to false when not hooked up to USB


int connectToMyServer(String myNothing) {

  digitalWrite(D7, HIGH);
  if (client.connect(server, 80)) {
      client.write("GET / HTTP/1.1\r\n");
      client.write("Host: node-ws-test-jerteach.c9.io\r\n");
      client.write("Upgrade: websocket\r\n");
      client.write("Connection: Upgrade\r\n");
      client.write("Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n");
      client.write("Sec-WebSocket-Version: 13\r\n");
      client.write("\r\n");

      digitalWrite(D7, LOW);
      if (myUsbSerialDebugOn){
         Serial.println("successfully connected");
       }
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

    while(client.read() >= 0);    // ignore the rest of the http request
    client.stop();                // shut down the client for next connection

    digitalWrite(D7, LOW);
    if (myUsbSerialDebugOn){
       Serial.println("successfully disconnected");
    }
    return 1;

}


void setup() {

      pinMode(D7, OUTPUT);

      Spark.function("connect", connectToMyServer);
      Spark.function("stop", stopMyServer);
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

  
  if (client.connected()) {
    if (client.available()) {
       
        char myIncoming = client.read();

        
        
        if (myIncoming == 'A'){ digitalWrite(D7, HIGH);}
        if (myIncoming == 'B'){ digitalWrite(D7, LOW);}
        if (myIncoming == 'C'){ RGB.brightness(0); }
        if (myIncoming == 'D'){ RGB.brightness(100); }
        if (myIncoming == 'E'){ RGB.brightness(250); }
        
        if (myUsbSerialDebugOn){
            Serial.print(myIncoming);
        }  
  

      
      }   
    }
}
