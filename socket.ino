TCPClient client;


char server[] = "node-ws-test-jerteach.c9.io";


bool myUsbSerialDebugOn = true;      // set to false when not hooked up to USB


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
     digitalWrite(D7, LOW);
     if (myUsbSerialDebugOn){
         Serial.println("failed to connect");
      }
     return -1; // failed to connect
  }

}

void setup() {

      pinMode(D7, OUTPUT);

      Spark.function("connect", connectToMyServer);
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
  digitalWrite(D7, LOW);
  if (client.connected()) {
    if (client.available()) {
       
        char myIncoming = client.read();
        if (myIncoming == 'A'){ digitalWrite(D7, HIGH);}
      
      if (myUsbSerialDebugOn){  
          Serial.print(myIncoming);
        // delay(2);
      }
      
      
      } else {
        //digitalWrite(D7, LOW);
      }
    }
}
