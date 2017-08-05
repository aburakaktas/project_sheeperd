
//Hardware Config

//Signal Light
int ledPin = 9;

//Button
int buttonPin = 2;

//Light Sensors
int lightPin1 = 1;
int lightPin2 = 3;
int lightPin3 = 5;

//Vibration Motor
int vibPin = 8;

//Universal Timer
int sayac = 0;

//Points that will count down for the game, put higher values for longer game.
int puan = 100;

//Game Difficulty. Points will drop when the light is below this threshold. In percentage, between the calibrated range.

int minThreshold = 50;

//Vibration Lenght x2 (by default it's 15)
const int vibLen = 6;

//Vibration Timer, no need to manipulate
int vibTimer = 0;
int vibPattern[vibLen];

//Misc
String oyunModu = "";
int ledBreathe = 0;
String ledBreatheYon = "yukari";
int longPressCount = 0;
int isikMiktari = 0;
int newHigh = 0;
int newLow = 0;
int frekans = 0;
const int numReadings = 10;

int readings1[numReadings];  
int readings2[numReadings]; 
int readings3[numReadings]; 
int index1 = 0;                 
int total1 = 0;                  
int average1 = 0;
int index2 = 0;                 
int total2 = 0;                  
int average2 = 0;
int index3 = 0;                 
int total3 = 0;                  
int average3 = 0;
int allReadings[3];

void setup(){
  Serial.begin(9600);
  //Startup Text
  Serial.println("Sheeperd Headwear Firmware");
  Serial.println("Version 1.1");
  Serial.println("-------------------------------------");
  Serial.println(" ");
  Serial.println("Standing by...");
  Serial.println("Stand outside the light and long press button to begin the game");
  
  //Put game in standby in initialization
  oyunModu="standby";
  
  //Hardware Config
  pinMode(buttonPin,INPUT);
  
  //Reset Smoothing
   for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings1[thisReading] = 0; 
    
   for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings2[thisReading] = 0; 
    
   for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings3[thisReading] = 0; 
    
   for (int i=0; i<vibLen; i++){
     if (i<=vibLen/2){
       vibPattern[i]=1;
     }else{
       vibPattern[i]=0;
     }
   }
     
}
  

void loop(){
  
  //STANDBY
  if (oyunModu=="standby"){
    //Serial.println(ledBreathe);
    analogWrite(ledPin,ledBreathe);
    if (ledBreatheYon == "yukari"){
      ledBreathe++;
    }
    if (ledBreatheYon == "asagi"){
      ledBreathe--;
    }
    if (ledBreathe<2) {ledBreatheYon="yukari";}
    if (ledBreathe>170) {ledBreatheYon="asagi";}
    if (digitalRead(buttonPin)==1){
      longPressCount++;
      if (longPressCount>300) {
        //Serial.println(longPressCount);
        for (int i = 0; i<5; i=i+1){
          digitalWrite(ledPin,HIGH);
          delay(200);
          digitalWrite(ledPin,LOW);
          delay(200);
          Serial.print("*");
        }
        oyunModu = "kalibrasyon";
        longPressCount=0;
      }
    } else {longPressCount = 0;}
    
  }
  
  //CALIBRATION
  if (oyunModu=="kalibrasyon"){
    Serial.println("Starting calibration...");
    Serial.println("Move inside the light in 15 seconds");
    frekans=1;
    isikMiktari=analogRead(lightPin1);
    newLow=isikMiktari;
    newHigh=isikMiktari;

    for (int i=0; i<300; i++){
     if (ledBreatheYon == "yukari"){
      ledBreathe=ledBreathe+5;
     }
     if (ledBreatheYon == "asagi"){
      ledBreathe=ledBreathe-5;
     }
     if (ledBreathe<frekans) {ledBreatheYon="yukari";}
     if (ledBreathe>255) {ledBreatheYon="asagi";}
     isikMiktari=analogRead(lightPin1);
     if (isikMiktari<newLow) {newLow=isikMiktari; }
     if (isikMiktari>newHigh) {newHigh=isikMiktari;}
     isikMiktari=analogRead(lightPin2);
     if (isikMiktari<newLow) {newLow=isikMiktari; }
     if (isikMiktari>newHigh) {newHigh=isikMiktari;}
     isikMiktari=analogRead(lightPin3);
     if (isikMiktari<newLow) {newLow=isikMiktari; }
     if (isikMiktari>newHigh) {newHigh=isikMiktari;}
     Serial.print("Round: ");
     Serial.print(i);
     Serial.print("/300    ");
     Serial.print("New Min Light Limit: ");
     Serial.print(newLow);
     Serial.print(" New Max Light Limit: ");
     Serial.println(newHigh);
     analogWrite(ledPin,ledBreathe);
     frekans=frekans+1;
     
    }
    newHigh=newHigh-15;
    Serial.println("Calibration complete.");
    Serial.print("Min Light Limit: ");
    Serial.print(newLow);
    Serial.print(" Max Light Limit: ");
    Serial.println(newHigh);
    Serial.println(" ");
    Serial.println("Starting the game...");
    oyunModu = "oyun";
  }
  
  //GAMEPLAY
  if (oyunModu == "oyun"){
    sayac++;
    
    //SMOOTH SENSOR 1
    total1= total1 - readings1[index1];
    readings1[index1] = analogRead(lightPin1); 
    total1= total1 + readings1[index1]; 
    index1 = index1 + 1;  
    if (index1 >= numReadings)               
    index1 = 0; 
    average1 = total1 / numReadings; 
    
    //SMOOTH SENSOR 2
    total2= total2 - readings2[index2];
    readings2[index2] = analogRead(lightPin2); 
    total2= total2 + readings2[index2]; 
    index2 = index2 + 1;  
    if (index2 >= numReadings)               
    index2 = 0; 
    average2 = total2 / numReadings; 
    
    //SMOOTH SENSOR 3
    total3= total3 - readings3[index3];
    readings3[index3] = analogRead(lightPin3); 
    total3= total3 + readings3[index3]; 
    index3 = index3 + 1;  
    if (index3 >= numReadings)               
    index3 = 0; 
    average3 = total3 / numReadings; 
    
    //SAVE SENSOR READINGS IN ARRAY
    allReadings[0]=average1;
    allReadings[1]=average2;
    allReadings[2]=average3;
    
    //SORT SENSOR READINGS FROM LOW TO HIGH
    for(int i=0; i<2; i++) {
        for(int o=0; o<(3-(i+1)); o++) {
                if(allReadings[o] > allReadings[o+1]) {
                    int t = allReadings[o];
                    allReadings[o] = allReadings[o+1];
                    allReadings[o+1] = t;
                }
        }
    }
    
    //USE HIGHEST READING AS DEFAULT AND MAP IT AS PERCENT
    isikMiktari=map(allReadings[2],newLow,newHigh,0,100);
    
    Serial.print(isikMiktari);
    Serial.print("% ");
    Serial.print("Lowest Light Detected,   Points: ");
    Serial.print(puan);
    Serial.print("  Sensor 0: ");
    Serial.print(allReadings[0]);
    Serial.print("  Sensor 1: ");
    Serial.print(allReadings[1]);
    Serial.print("  Sensor 2: ");
    Serial.print(allReadings[2]);
    Serial.print("    vibTimer: ");
    Serial.print(vibTimer);
    Serial.print("    Pattern Yeri: ");
    Serial.print(vibPattern[vibTimer%vibLen]);
    Serial.print("    Pattern: ");
    for (int i=0; i<vibLen; i++) Serial.print(vibPattern[i]);
    Serial.println(" ");
    vibTimer++;
    
    
    if (isikMiktari<minThreshold) {
      
   
      if (sayac%5 == 0 ){
         puan--;
       }
       
      if (vibPattern[vibTimer%vibLen] == 1) {digitalWrite(vibPin,HIGH);}
       if (vibPattern[vibTimer%vibLen] == 0) {digitalWrite(vibPin,LOW);}
      
      
        
      
    } else {  digitalWrite(vibPin,LOW);}
   if (sayac%(puan/2) == 0) {
      digitalWrite(ledPin,HIGH);
      delay(50);
      digitalWrite(ledPin,LOW);
   }
   if (puan<1) {
     oyunModu="kayip";
   }
  }
  
  //GAME LOST
   if (oyunModu == "kayip"){
     digitalWrite(ledPin,HIGH);
     digitalWrite(vibPin,HIGH);
     sayac++;
     
     if (digitalRead(buttonPin)==1){
      longPressCount++;
      if (longPressCount>300) {
        //Serial.println(longPressCount);
        for (int i = 0; i<5; i=i+1){
          digitalWrite(ledPin,HIGH);
          delay(200);
          digitalWrite(ledPin,LOW);
          delay(200);
          Serial.print("*");
        }
        oyunModu = "standby";
        longPressCount=0;
        digitalWrite(ledPin,LOW);
        digitalWrite(vibPin,LOW);
        Serial.println("Sheeperd Headwear Firmware");
        Serial.println("Version 1.1");
        Serial.println("-------------------------------------");
        Serial.println(" ");
        Serial.println("Standing by...");
        Serial.println("Stand outside the light and long press button to begin the game");
      }
    } else {longPressCount = 0;}
   }
}
