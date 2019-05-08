// on 5 wire turning motor with potentiometer
// red motor B1
// black motor B2
// white 3V3
// yellow A0
// brown GND


int myRandom = 0;

unsigned long myDuration = 0;
unsigned long myMilliNew = 0;
unsigned long myMilliOld = 0;



void setup() {
    pinMode(D7, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(D4, OUTPUT);
    
    
    
    
    digitalWrite(D7, 1);
    // Particle.publish("myRandom2 = ", String(myRandom), 60, PRIVATE); 
   //  Particle.publish(String(myDuration), String(myTimeNow), 60, PRIVATE); 
    delay(2000);   
    digitalWrite(D7, 0);
    delay(1000);
    
    
    
}

void loop() {

   
    myMilliNew = millis();
    
    myDuration =  myMilliNew - myMilliOld;   //= myTimeNow  - myTimeOld;

    if (myDuration >= 4800){
        myMilliOld = myMilliNew;   // reset timer
        
        myRandom = (rand() / 1000000) + 1000;    // number about between 1000 and 3000 
        
        Particle.publish(String(myDuration), String(myRandom), 60, PRIVATE); 
        delay(1000);    
        
        

        
    }
    setDirection(myRandom, 100);
    delay(1);

   
  //  for (int x=0; x <= 200; x++){
  //      setDirection(2050, 100);
  //      delay(20);    
      
  //  }
  
  
  

 
 
 

   
   
   
}





// must loop through several times before it sets the mark
void setDirection(int myDirection , int mySpeed ){
    // assume A0 for potentiometer reading
    // assume A4 for DC turning motor speed
    // assume D4 for direction. reverse motor wires if wrong
    int myBias = 70;  // what we are OK for as straight   bias 30 shakes
    
    
    int myA0 = analogRead(A0);
    
    if (mySpeed < 50){ mySpeed = 50; }     // minimum turning speed;
    if (mySpeed > 255){ mySpeed = 255; }   // maximum turning speed;
    
    if (myDirection < 0){ myDirection = 0; }         // minimum turning amount
    if (myDirection > 4095){ myDirection = 4095; }   // maximum turning amount

    
    if (myDirection >= myA0 + myBias){
        digitalWrite(D4, 1);
        analogWrite(A4, mySpeed);  // slowish speed for turning motor
    } else 
    if (myDirection <= myA0 - myBias){
        digitalWrite(D4, 0);
        analogWrite(A4, mySpeed);  // slowish speed for turning motor
    }    else {
        analogWrite(A4, 0);  // stop the turning motor  
        Particle.publish("Direction:"+ String(myDirection), "Set to:"+String(myA0), 60, PRIVATE);
        delay(1000);
    }
    
    
}
