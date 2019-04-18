// on 5 wire turning motor with potentiometer
// red motor B1
// black motor B2
// white 3V3
// yellow A0
// brown GND


void setup() {
    pinMode(D7, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(D4, OUTPUT);
}

void loop() {

   
    for (int x=0; x <= 200; x++){
        setDirection(2050, 100);
        delay(20);    
      
    }
    digitalWrite(D7, 1);
    delay(1000);   
    digitalWrite(D7, 0);
    
 
 
 
    
    for (int x=0; x <= 200; x++){
        setDirection(1200, 100);
        delay(20);    
      
    }
    digitalWrite(D7, 1);
    delay(1000);   
    digitalWrite(D7, 0);  
       
       
       
       
    for (int x=0; x <= 200; x++){
        setDirection(2900, 100);
        delay(20);    
      
    }
    digitalWrite(D7, 1);
    delay(6000);   
    digitalWrite(D7, 0);
   
   
   
   
   
   
}





// must loop through several times before it sets the mark
void setDirection(int myDirection , int mySpeed ){
    // assume A0 for potentiometer reading
    // assume A4 for DC turning motor speed
    // assume D4 for direction. reverse motor wires if wrong
    int myBias = 100;  // what we are OK for as straight   bias 30 shakes
    
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
        Particle.publish("Going straight", String(analogRead(A0)), 60, PRIVATE);
    }
    
    
}
