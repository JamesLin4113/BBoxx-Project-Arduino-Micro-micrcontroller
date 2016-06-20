//global variables
float AmpHour = 1.7;          //user allowance
int countDeepDischarged = 0;  //count significant detections that indicate the battery is entering deep discharging mode
int countFullCharged = 0;     //count significant detections that indicate the battery is fully charged
int T = 10000;                //detecting period - 10 seconds (also is the length of loop function)

int BatteryVoltagePin = 3;    //analog pin which reads battery voltage
int ResistorVoltagePin = 4;   //analog pin which reads the output voltage of in-amp
int ReferenceVoltagePin = 5;  //analog pin which reads the reference voltage of in-amp

//int BuckVoltagePin = 2;     //used in testing section to read the output voltage of buck SMPS

int ledGreenPin1 = 12;//digital pin that turns on/off green LED 1
int ledGreenPin2 = 11;//digital pin that turns on/off green LED 2
int ledGreenPin3 = 10;//digital pin that turns on/off green LED 3
int ledGreenPin4 = 9; //digital pin that turns on/off green LED 4
int ledShortCircuit = 8; //digital pin that turns on/off a red LED when the circuit is in short/normal condition.
int lightMosfetGate = 6; //digital pin that turns HIGH/LOW the gate of MOSFET in light circuit
int phoneMosfetGate = 5; //digital pin that turns HIGH/LOW the gate of MOSFET in phone charging circuit

//interrupt
const byte interruptPin = 7;
volatile bool shortCircuit = false;


void setup() {
  pinMode(ledGreenPin1, OUTPUT);   // sets the pin as output
  pinMode(ledGreenPin2, OUTPUT);
  pinMode(ledGreenPin3, OUTPUT);
  pinMode(ledGreenPin4, OUTPUT);
  pinMode(ledShortCircuit, OUTPUT); //output pin for short circuit LED 
  pinMode(lightMosfetGate, OUTPUT); //output pin for MOSFET in light circuit
  pinMode(phoneMosfetGate, OUTPUT); //output pin for MOSFET in phone charging circuit

  //interrupt
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), faultInterrupt, HIGH);//calls ISR and set the interrupt pin to digital HIGH which switches off the circuit 
  
  Serial.begin(9600);
}

void loop() {
  // main code here which runs repeatedly:
  
  //******initialisation********
  float v1=0;
  float v2=0;
  float v3=0;
  float v4=0; 
  float BatteryVoltage = 0; //stores battery voltage
  float ResistorVoltage = 0;  //stores voltage across resistor
  float ReferenceVoltage = 0; //stores reference voltage read from the in-amp
  float I = 0;  //current flow in the whole circuit
 
  //**************measuring potential difference across the battery*************
  v1 = analogRead(BatteryVoltagePin);
  BatteryVoltage = v1/1024*20;
  Serial.print(BatteryVoltage, DEC);
  Serial.print(",");
  
  //**************measuring Buck SMPS's voltage output for testing purpose***
  /*
  v5 = analogRead(BuckVoltagePin);
  BuckVoltage = v5/1024*5;
  Serial.print(BuckVoltage, DEC);
  Serial.print(",");
  */
  
  //**************Current Sensing Circuit*******************************
  v2 = analogRead(ResistorVoltagePin);
  ResistorVoltage = v2/1024*5;
  v3 = analogRead(ReferenceVoltagePin);
  ReferenceVoltage = v3/1024*5;  
  // current flow in the whole circuit, where 20 is the gain of in-amp and 0.05 is the resistance of the shunt resistor
  I = (ResistorVoltage - ReferenceVoltage)/(20*0.05)/1.055; 
  //count coulomb  
  AmpHour -= I * T/3600000;
  Serial.print(I, DEC);
  Serial.print(",");
  Serial.print(AmpHour, DEC);
  Serial.print(",");
  Serial.println(); //begin with a new line
  delay(0.6*T);

//*****************************discharging********************************
//*****************************discharging********************************
//*****************************discharging********************************
  if(I>0){//when discharging
    if(AmpHour>1.275){
      //***************detect if the SOC is above 75%****************
      //turn on MOSFETs
      digitalWrite(lightMosfetGate, HIGH);
      digitalWrite(phoneMosfetGate, HIGH);
      //flash all green LEDs
      digitalWrite(ledGreenPin1, HIGH);
      digitalWrite(ledGreenPin2, HIGH);
      digitalWrite(ledGreenPin3, HIGH);
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin1, LOW);
      digitalWrite(ledGreenPin2, LOW);
      digitalWrite(ledGreenPin3, LOW);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10);
      digitalWrite(ledGreenPin1, HIGH);
      digitalWrite(ledGreenPin2, HIGH);
      digitalWrite(ledGreenPin3, HIGH);
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin1, LOW);
      digitalWrite(ledGreenPin2, LOW);
      digitalWrite(ledGreenPin3, LOW);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10); 
    }
    else if(AmpHour>0.85){
      //***************detect if the SOC is below 75% but above 50%*********************
      //turn on MOSFETs
      digitalWrite(lightMosfetGate, HIGH);
      digitalWrite(phoneMosfetGate, HIGH);
      //flash three green LEDs
      digitalWrite(ledGreenPin2, HIGH);
      digitalWrite(ledGreenPin3, HIGH);
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin2, LOW);
      digitalWrite(ledGreenPin3, LOW);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10);
      digitalWrite(ledGreenPin2, HIGH);
      digitalWrite(ledGreenPin3, HIGH);
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin2, LOW);
      digitalWrite(ledGreenPin3, LOW);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10);    
    }
    else if(AmpHour>0.425){
    //***************detect if the SOC is below 50% but above 25%*********************
      //turn on MOSFETs
      digitalWrite(lightMosfetGate, HIGH);
      digitalWrite(phoneMosfetGate, HIGH);
      //flash Two green LEDs
      digitalWrite(ledGreenPin3, HIGH);
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin3, LOW);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10);
      digitalWrite(ledGreenPin3, HIGH);
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin3, LOW);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10);       
    }
    else if(AmpHour>0){
  //***************detect if the SOC is below 25% but above 0%*******************
      //turn on MOSFETs
      digitalWrite(lightMosfetGate, HIGH);
      digitalWrite(phoneMosfetGate, HIGH);  
      //flash One green LED
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10);
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10);      
    }
  //***************detect if the SOC is (deep discharged)***********************
    else if (AmpHour<0){
      //reset coulomb counting
      AmpHour = 0;
      //cut off the circuit
      digitalWrite(lightMosfetGate, LOW);
      digitalWrite(phoneMosfetGate, LOW);
      delay(T*0.4);
    }
    if(BatteryVoltage<10.8){
      //*****voltage method for preventing deep discharging*********
      countDeepDischarged++;
      if(countFullCharged == 5){
        //when the battery is determined to be in deep discharging mode 
        AmpHour = 1.7;//reset to 1.7
        countFullCharged = 0;//reset 0
        //switch off the circuit
        digitalWrite(lightMosfetGate, LOW);
        digitalWrite(phoneMosfetGate, LOW);
      }
      delay(T*0.4);
    }else{
      //reset count to 0 if the battery is not in deep discharging mode
      countDeepDischarged = 0;
      delay(T*0.4);
    }
    
    
  }else if(I<-0.1){
    //**********************battery charging*************************
    //**********************battery charging*************************
    //**********************battery charging*************************

    //**********************less than 33% SOC*************************
    if( 0.567 > AmpHour >= 0){
      //turn on the MOSFETs
      digitalWrite(lightMosfetGate, HIGH);
      digitalWrite(phoneMosfetGate, HIGH);
      //flash one green LEDs      
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10);
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10);      
    }
    //**********************less than 66% but more than 33% SOC*************************
    else if(1.13 > AmpHour>=0.567){
      //turn on the MOSFETs
      digitalWrite(lightMosfetGate, HIGH);
      digitalWrite(phoneMosfetGate, HIGH);   
      //flash Two green LEDs
      digitalWrite(ledGreenPin3, HIGH);
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin3, LOW);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10);
      digitalWrite(ledGreenPin3, HIGH);
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin3, LOW);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10);      
    }
    //**********************less than 99% but more than 66%*************************
    else if(1.7>AmpHour>=1.13){
      //turn on the MOSFETs
      digitalWrite(lightMosfetGate, HIGH);
      digitalWrite(phoneMosfetGate, HIGH);
      //flash three green LEDs
      digitalWrite(ledGreenPin2, HIGH);
      digitalWrite(ledGreenPin3, HIGH);
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin2, LOW);
      digitalWrite(ledGreenPin3, LOW);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10);
      digitalWrite(ledGreenPin2, HIGH);
      digitalWrite(ledGreenPin3, HIGH);
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin2, LOW);
      digitalWrite(ledGreenPin3, LOW);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10);  
    }
    //**********************fully charged*************************
    else if(AmpHour>=1.7){
      AmpHour = 1.7;//reset to 1.7
      //turn on the MOSFETs
      digitalWrite(lightMosfetGate, HIGH);
      digitalWrite(phoneMosfetGate, HIGH);
      //flash all green LEDs
      digitalWrite(ledGreenPin1, HIGH);
      digitalWrite(ledGreenPin2, HIGH);
      digitalWrite(ledGreenPin3, HIGH);
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin1, LOW);
      digitalWrite(ledGreenPin2, LOW);
      digitalWrite(ledGreenPin3, LOW);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10);
      digitalWrite(ledGreenPin1, HIGH);
      digitalWrite(ledGreenPin2, HIGH);
      digitalWrite(ledGreenPin3, HIGH);
      digitalWrite(ledGreenPin4, HIGH);
      delay(T/10);
      digitalWrite(ledGreenPin1, LOW);
      digitalWrite(ledGreenPin2, LOW);
      digitalWrite(ledGreenPin3, LOW);
      digitalWrite(ledGreenPin4, LOW);
      delay(T/10); 
    }else{
      delay(T*0.4);
    }
    //************voltage method for full charge mode*****************
    if(BatteryVoltage>13.4){
      //accumulate count
      countFullCharged++;
      if(countDeepDischarged == 5){
        //when the battery is fully charged
        AmpHour = 0;//reset to 0
        countDeepDischarged = 0;//reset 0
        //MOSFETs still on
        digitalWrite(lightMosfetGate, HIGH);
        digitalWrite(phoneMosfetGate, HIGH);
      }
      delay(T*0.4);
    }else{
      //reset count if the battery is not fully charged
      countFullCharged = 0;
      delay(T*0.4);
    }
  }
  else{//neither charging or discharging 
    //do nothing but delay 0.4*T miliseconds
    delay(0.4*T);
  }

    
//***********************short circuit detection and protection*************************
//***********************short circuit detection and protection*************************
//***********************short circuit detection and protection*************************
  if(shortCircuit == true){
    //Volatile variable 'shortCircuit' becomes true when short Circuit occurs 
    digitalWrite(ledShortCircuit, HIGH);
    /*a reset button will be added in the future, so that the circuit 
    can go back to its initial state when short circuit has been fixed*/
  }else{
    digitalWrite(ledShortCircuit, LOW);
  }
  
} 

void faultInterrupt() {
  shortCircuit = true;
  //switch off the circuit
  digitalWrite(lightMosfetGate,LOW);
  digitalWrite(phoneMosfetGate,LOW);
}
