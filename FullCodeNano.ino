int spdt1 = 3;
int spdt2 = 2;

//Resistance Mode Initialisation
int analogPin_R = A1;
int V_measured= 0;
int Vin = 5;
float Vout = 0;
float R2 = 0;
int R1K = 9;                //1k resistor
int R10K = 10;              //10k resistor
int R100K = 11;             //100k resistor
int R1M = 12;               //1M resistor

//Capacitance Mode Initialisation
int analogPin_C = A0;
int dischargePin = 6;       //220 resistor
int chargePinMicro = 7;     //10k resistor
int chargePinNano = 8;      //4.7M resistor
unsigned long starttime;
unsigned long elapsedTime;
double uF, nF;

//Inductance Mode Initialisation
double pulse, frequency, capacitance, inductance;
int HchargePin = 4;
int Hread = 5;    

String mode = " "; 
String value = " ";
    
void setup() { 
  Serial.begin(9600);
  pinMode(spdt1, OUTPUT);
  pinMode(spdt2, OUTPUT);

//Resistance mode
  pinMode(analogPin_R,INPUT);
  pinMode(R1K,INPUT);
  pinMode(R10K,INPUT);
  pinMode(R100K,INPUT);
  pinMode(R1M,INPUT);

//Capacitance mode
  pinMode(chargePinNano, INPUT);
     
//Inductance mode
  pinMode(Hread, INPUT);
  pinMode(HchargePin, OUTPUT);

}

void loop() {
 while(Serial.available())  
  {
   uint8_t byteFromSerial = Serial.read();
   uint8_t buff[100] = {byteFromSerial};
   String str = (char*)buff;
   mode = str;
  
//Resistance mode
if(mode=="R"){
digitalWrite(spdt1,HIGH);
digitalWrite(spdt2,LOW);
pinMode(R1K,OUTPUT);
pinMode(R10K,INPUT);
pinMode(R100K,INPUT);
pinMode(R1M,INPUT);
digitalWrite(R1K,HIGH);
delay(100);

float R1= 0.994;                            //Set this values to the value of the used resistor in K ohms
V_measured= analogRead(analogPin_R);        //in 8bits
Vout = (V_measured * Vin)/1024.0;           //in volts
R2 = (Vout/(Vin-Vout)) * R1;
R2 = R2*1000;                               //*1000 because we express it in ohms 

if (R2 < 1000)
  {
  Serial.print(R2,1);
  Serial.println(" Ω");
  }
  
  ///////////////////-10k-/////////////////////
if (R2 > 1000)
  {
  pinMode(R1K,INPUT);
  pinMode(R10K,OUTPUT);
  pinMode(R100K,INPUT);
  pinMode(R1M,INPUT);
  digitalWrite(R10K,HIGH);
  delay(100);
  
  float R1= 9.96;                         //Set this values to the value of the used resistor in K ohms
  V_measured= analogRead(analogPin_R);    //in 8bits
  Vout = (V_measured * Vin)/1024.0;       //in volts
  R2 = (Vout/(Vin-Vout)) * R1;

      if (R2 < 10)
      {
      Serial.print(R2,1);
      Serial.println(" kΩ");  
      }
      
      ///////////////////-100k-/////////////////////
      if (R2 > 10)
      {
        pinMode(R1K,INPUT);
        pinMode(R10K,INPUT);
        pinMode(R100K,OUTPUT);
        pinMode(R1M,INPUT);
        digitalWrite(R100K,HIGH);
        delay(100);
        
        float R1= 99.5;                         // Set this values to the value of the used resistor in K ohms
        V_measured= analogRead(analogPin_R);    //in 8bits
        Vout = (V_measured * Vin)/1024.0;       //in volts
        R2 = (Vout/(Vin-Vout)) * R1;

        if (R2 < 100)
        {
        Serial.print(R2,1);
        Serial.print(" kΩ");
        }
        
        ///////////////////-1M-/////////////////////
        if (R2 > 100)
        {
        pinMode(R1K,INPUT);
        pinMode(R10K,INPUT);
        pinMode(R100K,INPUT);
        pinMode(R1M,OUTPUT);
        digitalWrite(R1M,HIGH);
        delay(100);
  
        float R1= 0.97;                         //Set this values to the value of the used resistor in M ohms
        V_measured= analogRead(analogPin_R);    //in 8bits
        Vout = (V_measured * Vin)/1024.0;       //in volts
        R2 = (Vout/(Vin-Vout)) * R1;

          if (R2 < 1)
          {
          Serial.print(R2,1);
          Serial.print(" MΩ");
          }
  
          if (R2 > 1)
          {
          Serial.println("Overload");
          }
      }
    }
  }
}

// Capacitance mode
if(mode=="C"){
digitalWrite(spdt1,LOW);
digitalWrite(spdt2,HIGH);
pinMode(chargePinMicro, OUTPUT);
digitalWrite(chargePinMicro, HIGH);  //Start charging the capacitor
    
starttime = millis ();               //Starts the timer
  
    while (analogRead (analogPin_C) < 648)
       {       
        //Wait for capacitor to charge until 63.2% of total voltage
       } 
    
    elapsedTime = millis () - starttime;  //Determine charging time
    
    uF = ((float) elapsedTime / 10000) * 1000;
    
    digitalWrite(chargePinMicro, LOW);
    pinMode(chargePinMicro, INPUT);
    
    pinMode(dischargePin, OUTPUT); 
    digitalWrite(dischargePin, LOW);      //Discharge the capacitor   
    
    while (analogRead(analogPin_C) > 0)
       {
        //Wait until capacitor fully discharged      
       } 
        
    pinMode(dischargePin, INPUT);        //Stop discharging

    if (elapsedTime > 10)          
       {     
        Serial.print(uF,1);
        Serial.println(" μF");      
       }
    else
       {       
        pinMode(chargePinNano, OUTPUT); 
        digitalWrite(chargePinNano, HIGH);    //Start charging the capacitor
        
        starttime = millis ();  // Starts the timer
  
        while (analogRead(analogPin_C) < 648)
           {       
            //Wait for capacitor to charge until 63.2% of total voltage
           } 
    
        elapsedTime = millis () - starttime;  //Determine charging time

        nF = ((float) elapsedTime / 47) * 10;

        digitalWrite(chargePinNano, LOW);
        pinMode(chargePinNano, INPUT);
        pinMode(dischargePin, OUTPUT); 
        digitalWrite(dischargePin, LOW);      //Discharge the capacitor    
    
        while (analogRead(analogPin_C) > 0)
           {
            //Wait until capacitor fully discharged  
           } 
        
        pinMode(dischargePin, INPUT);        //Stop discharging
        
        if (elapsedTime > 10)          
           {           
            Serial.print(nF,1);       
            Serial.println(" nF");       
           }
         
       }
}

//Inducatance mode
if(mode=="L"){
digitalWrite(spdt1,LOW);
digitalWrite(spdt2,LOW);
digitalWrite(HchargePin, HIGH);
delay(5);                               //charge inductor.
digitalWrite(HchargePin,LOW);
delayMicroseconds(100);                 //make sure resonance is measured
pulse = pulseIn(Hread,HIGH,5000);       //returns 0 if timeout
  if(pulse > 0.1){                        //reading is taken

  capacitance = 2.E-6;                    //capacitance value
  frequency = 1.E6/(2*pulse);
  inductance = 1./(4*sq(3.14159)*sq(frequency)*capacitance);
  inductance *= 1E6;              
  
  if(inductance >= 1000000){
  Serial.print(inductance/1000,1); 
  Serial.print(" mH"); 
  }
  else{
  Serial.print(inductance,1); 
  Serial.print(" μH");            
  }
  }
}
}
}
