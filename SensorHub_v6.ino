#include <MemoryFree.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#include <RCSwitch.h>
#include <IRLib.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

#include <dht.h>
dht DHT;
#define DHT11_PIN 5


// *****************************************
// ETEK CODES
// *****************************************

const int NUMBER_OF_etekELEMENTS = 20;
typedef struct {
   char etekCodeArr[16];
   byte etekStateArr;
   byte etekIDArr;
} etekCODES;

const etekCODES etekArr [NUMBER_OF_etekELEMENTS] PROGMEM = {
  {"F0FF0FFF0101",1,0}, {"F0FF0FFF1001",1,1}, {"F0FF0FF10001",1,2}, {"F0FFFF1F0001",1,3}, {"F0FFF1FF0001",1,4},
  {"F0FFFFFF0101",1,0}, {"F0FFFFFF1001",1,1}, {"F0FFFFF10001",1,2}, {"F0FF0F1F0001",1,3}, {"F0FF01FF0001",1,4},
  {"F0FF0FFF0110",0,0}, {"F0FF0FFF1010",0,1}, {"F0FF0FF10010",0,2}, {"F0FFFF1F0010",0,3}, {"F0FFF1FF0010",0,4},
  {"F0FFFFFF0110",0,0}, {"F0FFFFFF1010",0,1}, {"F0FFFFF10010",0,2}, {"F0FFFF1F0010",0,3}, {"F0FF01FF0010",0,4},
};  


// *****************************************
// WH FUNCTION CODES
// *****************************************

const int NUMBER_OF_WHfuncELEMENTS = 20;
typedef struct {
   long int irCodeArr;
   long int whCodeArr;
} WHfuncCODES;

const WHfuncCODES WHfuncArr [NUMBER_OF_WHfuncELEMENTS] PROGMEM = {
    { 0x1CE3D02F,  0x807FB04F },  // SY_CHNDWN
    { 0x1CE350AF,  0x807F30CF },  // SY_CHNUP
    { 0x1CE3F20D,  0x807F58A7 },  // SY_DOWN
    { 0x1CE3CA35,  0x807F6897 },  // SY_EXIT
    { 0x1CE3EA15,  0x807F42BD },  // SY_ASPECT
    { 0x1CE3C837,  0x807F48B7 },  // SY_SOURCE
    { 0x1CE3F807,  0x807FD827 },  // SY_LEFT
    { 0x1CE3E817,  0x807FA857 },  // SY_MENU
    { 0x1CE318E7,  0x807F8877 },  // SY_MUTE
    { 0x1CE32AD5,  0x807FB847 },  // SY_OK
    { 0x1CE348B7,  0x20DF10EF },  // SY_POWER 0x20DF10EF
    { 0x1CE39867,  0x807FDA25 },  // SY_BACK
    { 0x1CE338C7,  0x00 },        // SY_RESET
    { 0x1CE37887,  0x807F38C7 },  // SY_RIGHT
    { 0x1CE3B04F,  0x807FE817 },  // SY_SLEEP
    { 0x1CE330CF,  0x807FC837 },  // SY_INFO
    { 0x1CE3728D,  0x807F9867 },  // SY_UP
    { 0x1CE3F00F,  0x807FF00F },  // SY_VOLDWN
    { 0x1CE3708F,  0x807F708F },  // SY_VOLUP  
};


// *****************************************
// KEYPAD CODES
// *****************************************

const int NUMBER_OF_keypadELEMENTS = 30;
typedef struct {
   long int keypadCodeArr;
   byte keypadID;
   byte keypadState;
   byte keypadVal;
} keypadCODES;

const keypadCODES keypadArr [NUMBER_OF_keypadELEMENTS] PROGMEM = {
  { 0x807F807F, 0, 1, 1 }, 
  { 0x807F40BF, 1, 1, 2 }, 
  { 0x807FC03F, 2, 1, 3 }, 
  { 0x807FDA25, 3, 1, 4 },
  { 0x807FA05F, 4, 1, 5 },
  { 0x807F609F, 0, 0, 6 },
  { 0x807FE01F, 1, 0, 7 },
  { 0x807F10EF, 2, 0, 8 },
  { 0x807F906F, 3, 0, 9 },
  { 0x807F00FF, 4, 0, 0 },
  { 0x1CE3807F, 0, 1, 1 },  
  { 0x1CE340BF, 1, 1, 2 },  
  { 0x1CE3C03F, 2, 1, 3 },  
  { 0x1CE320DF, 3, 1, 4 },  
  { 0x1CE3A05F, 4, 1, 5 },  
  { 0x1CE3609F, 0, 0, 6 },  
  { 0x1CE3E01F, 1, 0, 7 },  
  { 0x1CE310EF, 2, 0, 8 },  
  { 0x1CE3906F, 3, 0, 9 },  
  { 0x1CE300FF, 4, 0, 0 },  
  { 0x807F8877, 0, 1, 1 },  
  { 0x807F48B7, 1, 1, 2 },  
  { 0x807FC837, 2, 1, 3 },  
  { 0x807F28D7, 3, 1, 4 },  
  { 0x807FA857, 4, 1, 5 },  
  { 0x807F6897, 0, 0, 6 },  
  { 0x807FE817, 1, 0, 7 },  
  { 0x807F18E7, 2, 0, 8 },  
  { 0x807F9867, 3, 0, 9 },  
  { 0x807F58A7, 4, 0, 0 },  
};





// *****************************************
// LED RGB LIGHT BULB
// *****************************************
const int NUMBER_OF_bulbELEMENTS = 24;
typedef struct {
   long int bulbValArr;
   char bulbTxtArr[12];
} bulbCODES;

const bulbCODES bulbArr [NUMBER_OF_bulbELEMENTS] PROGMEM = {
  { 0xF720DF, "bulbRED1" }, { 0xF710EF, "bulbRED2" }, { 0xF730CF, "bulbRED3" }, { 0xF708F7, "bulbRED4" }, { 0xF728D7, "bulbRED5" },
  { 0xF7A05F, "bulbGRN1" }, { 0xF7906F, "bulbGRN2" }, { 0xF7B04F, "bulbGRN3" }, { 0xF78877, "bulbGRN4" }, { 0xF7A857, "bulbGRN5" }, 
  { 0xF7609F, "bulbBLU1" }, { 0xF750AF, "bulbBLU2" }, { 0xF7708F, "bulbBLU3" }, { 0xF748B7, "bulbBLU4" }, { 0xF76897, "bulbBLU5" }, 
  { 0xF7E01F, "bulbWHITE"  }, { 0xF7D02F, "bulbFLASH" }, { 0xF7F00F, "bulbSTROBE" }, { 0xF7C837, "bulbFADE" }, { 0xF7E817, "bulbSMOOTH" },
  { 0xF700FF, "bulbBRITER" }, { 0xF7807F, "bulbDIMMER" }, { 0xF740BF, "bulbOFF" }, { 0xF7C03F, "bulbON" },
};




// MEGA 2560 PINS 
const byte IRRX_PIN = 5;      // IMPLICITLY DEFINED IN IRLIB LIBRARY
const byte RCRX_PIN = 2; 
const byte RHRX_PIN = 2; 
const byte RCTX_PIN = 12;  
const byte RHTX_PIN = 12; 

const byte PWR1_PIN = 13;  // RFTX POWER
const byte GND1_PIN = A3; 
const byte LED1_PIN = A0;
const byte BUZZ_PIN = 4;   // BUZZER PIN

const byte ANA1_PIN = A1;  // 
const byte ANA2_PIN = A2;  // LDR
const byte DIG1_PIN = 7;   // PIR
const byte DIG2_PIN = 11;  


const byte RGB_PINS[3] = { A12, A14, A15 };
const byte RGBgnd_PIN = A13;


// MICROPHONE
const byte mANA_PIN = A4;
const byte mGND_PIN = A5;
const byte mPWR_PIN = A6;
const byte mDIG_PIN = A7;





// UNO PINS 
//const byte IRRX_PIN = 8;
//const byte RCRX_PIN = 2; 
//const byte RHRX_PIN = 2; 
//const byte RCTX_PIN = 4; 
//const byte RHTX_PIN = 4; 


const byte PIR = DIG1_PIN;
const byte LDR = ANA2_PIN;
const byte mANA = mANA_PIN;
const byte mDIG = mDIG_PIN;



// ^^^^ RC VARIABLES^^^^
RCSwitch RCsend = RCSwitch();
RCSwitch RCrecv = RCSwitch();
unsigned long Etek_code;
unsigned int Etek_length;
unsigned int pulsetime = 140;  // RCremote pulse out


// ^^^^ RH VARIABLES^^^^
RH_ASK driver(2000,RHRX_PIN,RHTX_PIN);  //(speed, RXpin, TXpin, ptt;IN, inverted)
char RHMsg[20];


// ^^^^ IR VARIABLES ^^^^
IRrecv irRecv(IRRX_PIN);
//IRsend irSend;             // UNO PWM PIN 3
IRsendNEC irSendNEC;       // irlib
IRdecodeNEC irDecoder;        // irlib
IRTYPES irType;
unsigned int irBuffer[RAWBUF];
unsigned long irValue; 
int irLen;
int irRECV_flag=0;



// ^^^^ MIC VARIABLES ^^^^
int mic_threshold = 500;
byte mic_flag = 0;
byte led_flag = 0;

// ^^^^ TIME DELAYS ^^^^
unsigned long prevMillMIC = 0;
unsigned long prevMillisPRINT = 0;
const unsigned long interval = 3000;  
const unsigned long intervalPRINT = 7000; 

// ^^^^ PIR VARIABLES ^^^^
unsigned long prevMillPIR = 0;
const unsigned long intervalPIR = 300000; 
byte pir_flag=0;
byte pir_prev=0;
byte pir_curr=0;


// #######################################################
// RTC
// #######################################################




// #######################################################
// SETUP
// #######################################################
void setup() {
  Serial.begin(9600);
    // if(!driver.init()) {Serial.println(F("init failed"));}  // screwed up ir library
  
  // RTC REAL-TIME CLOCK
  Wire.begin();
  RTC.begin();  
  if (! RTC.isrunning()) {
    Serial.println(F("RTC is NOT running!"));
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }  
  
  pinMode(ANA1_PIN, INPUT);
  pinMode(ANA2_PIN, INPUT);
  pinMode(mANA_PIN, INPUT);
  pinMode(DIG1_PIN, INPUT);
  pinMode(DIG2_PIN, INPUT); 
  pinMode(mDIG_PIN, INPUT);
  
  pinMode(RGBgnd_PIN, OUTPUT); digitalWrite(RGBgnd_PIN, LOW);
  pinMode(mGND_PIN, OUTPUT); digitalWrite(mGND_PIN, LOW);
  pinMode(GND1_PIN, OUTPUT); digitalWrite(GND1_PIN, LOW);
  pinMode(PWR1_PIN, OUTPUT); digitalWrite(PWR1_PIN, HIGH);
  pinMode(mPWR_PIN, OUTPUT); digitalWrite(mPWR_PIN, HIGH);
  pinMode(LED1_PIN, OUTPUT); digitalWrite(LED1_PIN, HIGH);
  pinMode(BUZZ_PIN, OUTPUT); digitalWrite(BUZZ_PIN, LOW);
  
  for (int thisPin=0; thisPin < 3; thisPin++) { 
    pinMode(RGB_PINS[thisPin], OUTPUT); 
    Serial.print("PIN: "); Serial.println(RGB_PINS[thisPin]); 
    analogWrite(RGB_PINS[thisPin], 255); delay(1000);
    analogWrite(RGB_PINS[thisPin], 0);   delay(1000);
  }
  
  digitalWrite(BUZZ_PIN, HIGH); delay(100); digitalWrite(BUZZ_PIN, LOW);
  
  LED_FLASH(8, 50, LED1_PIN);  

  RCsend.enableTransmit(RCTX_PIN);
  RCsend.setProtocol(1);
  RCsend.setPulseLength(pulsetime);
  RCsend.setRepeatTransmit(6); 
  RCrecv.enableReceive(0);   // ARDUINO PIN 2 - interrupt 0 
  
  irRecv.enableIRIn(); // Start the receiver
  irDecoder.UseExtnBuf(irBuffer);
  
  Serial.println ();
  Serial.print (F("Free memory = "));
  Serial.println (freeMemory());
  Serial.println(F("Setupt complete")); 
  
}



// #######################################################
// MAIN LOOP
// #######################################################
void loop() {
  
  delay(4);
  unsigned long currMill=millis();
  
  // GET DHT DATA
  DHT_READ_FUNC();

  //***************************************
  // SENSOR code 
  //***************************************
  int  LDR_VAL = analogRead(LDR);
  byte PIR_VAL = digitalRead(PIR);
  int  mANA_VAL = analogRead(mANA);
  byte mDIG_VAL = analogRead(mDIG); 

  if(mDIG_VAL>mic_threshold && led_flag==0)
  {
    mic_flag = 1;
    led_flag=1;
    prevMillMIC = currMill; 
    digitalWrite(LED1_PIN, HIGH);
    Serial.print(F("ANA MIC:"));     Serial.print(mANA_VAL);
    Serial.print(F(" | DIG MIC: ")); Serial.println(mDIG_VAL); 
  }
 
  if(currMill - prevMillMIC >= interval && mic_flag==1) 
  {
    digitalWrite(LED1_PIN, LOW);
    prevMillMIC = currMill;   
    mic_flag = 0;
    led_flag = 0;
    Serial.println(F("LIGHT OFF")); 
  }
  
  if(currMill - prevMillisPRINT >= intervalPRINT) {
    prevMillisPRINT = currMill;   
    Serial.print(F("LDR:"));     Serial.print(LDR_VAL);
    Serial.print(F(" | PIR: ")); Serial.print(PIR_VAL);
    Serial.print(F(" | MIC: ")); Serial.println(mANA_VAL);
    RTC_SHOW();
  }
  
  //***************************************
  // PIR LOGIC
  //*************************************** 
  pir_curr=PIR_VAL;
  if(pir_curr!=pir_prev && pir_curr==0)
  {
    Serial.println(F("PIR TIMER STARTED"));
    prevMillPIR=currMill;
    pir_flag=1;
  }
  else if(pir_curr==1)
  {
    prevMillPIR=currMill;
    pir_flag=0;
  }  
  
  if( (currMill - prevMillPIR >= intervalPIR) && pir_flag==1) {
    prevMillPIR = currMill;   
    pir_flag=0;
    Serial.println(F("TV OFF"));   //  Serial.print(LDR_VAL);
    delay(300);
    IR_SEND(0x807FE817);   // WH SLEEP
  }  
  pir_prev=pir_curr;
  

  //***************************************
  // RCremote RX code 
  //***************************************
  //int etekState  = -1;
  //int etekSwitch = -1;
  if (RCrecv.available()) {
    //delay(1);
    RC_RECEIVE();  
  }
  
  
  //***************************************  
  // RADIOHEAD RX CODE 
  //***************************************
  uint8_t RHbuf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t RHbuflen = sizeof(RHbuf);
  if (driver.recv(RHbuf, &RHbuflen)) // Non-blocking
  {
    RH_RECEIVE((uint8_t *)RHbuf, RHbuflen);
  } 
  

  //***************************************  
  // IR REMOTE RX code 
  //***************************************  

  if (irRecv.GetResults(&irDecoder)) {  
    IR_RECEIVE();
  } else{}
  
  if(irValue!=0)
  {
    IR_CODE_CHECK(irValue);
  }

  irValue=0;
  delay(4);

}  // # END MAIN LOOP 















// #######################################################
// FUNCTIONS
// #######################################################


//------------------------------------
// SENSOR READ FUNCTION
//------------------------------------
void IR_CODE_CHECK(long int code){
   Serial.println(F("Code check..."));
   byte ir_found_flag=0;
   byte arrCNT=0;
   while(arrCNT<NUMBER_OF_keypadELEMENTS ){
     
      keypadCODES keypadItem1;
      memcpy_P (&keypadItem1, &keypadArr[arrCNT], sizeof keypadItem1);
    
      int ID = int(keypadItem1.keypadID);   
      if(code==keypadItem1.keypadCodeArr)
      {
        Serial.print(ID);
        if(keypadItem1.keypadState==1)
        {
          Serial.println(F(":ON"));  
          ir_found_flag=1;
          RC_SEND(ID, 1);  
          break;
        } 
        else if(keypadItem1.keypadState==0)
        {
          Serial.println(F("OFF"));  
          ir_found_flag=1;
          RC_SEND(ID, 0);      
          break; 
        }
        break;
      }
      else{} 
      arrCNT++;     
   }  // END WHILE LOOP
   
   // -------------------------------------

   byte WHfuncCNT=0;
   while(WHfuncCNT<NUMBER_OF_WHfuncELEMENTS ){
      
      WHfuncCODES WHfuncItem1;
      memcpy_P (&WHfuncItem1, &WHfuncArr[WHfuncCNT], sizeof WHfuncItem1);
      
      if(code==WHfuncItem1.irCodeArr)
      {
          Serial.print(F("EXEC WH FUNCTION: "));
          Serial.println(WHfuncItem1.whCodeArr, HEX);
          ir_found_flag=1;
          IR_SEND(WHfuncItem1.whCodeArr); 
          break;
      }
      else{} 
      WHfuncCNT++;     
      
   }  // END WHILE LOOP
   

   if(ir_found_flag==0) Serial.println(F("IR UNKNOWN"));

}



//------------------------------------
// RTC FUNCTION
//------------------------------------
void RTC_SHOW(){
    DateTime now = RTC.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();  
}



//------------------------------------
// IR REMOTE SEND 
//------------------------------------
void IR_RECEIVE()
{
      if(irDecoder.decode()) {
          //GotOne=true;
          irValue = irDecoder.value;
          irLen = irDecoder.bits;
          irType = irDecoder.decode_type;
              //irDecoder.decode();
          Serial.print(F("Received "));
          Serial.print(Pnames(irType));
          Serial.print(F(" Value:0x"));
          Serial.println(irDecoder.value, HEX);
          delay(500);
      }
      irRecv.resume(); 
}




//------------------------------------
// IR REMOTE SEND 
//------------------------------------
void IR_SEND(unsigned long ircode)
{
  
  Serial.println(F("IR_SEND")); delay(10);
  Serial.flush();
  RCrecv.disableReceive();
  RCsend.disableTransmit();
  driver.setModeIdle();
  delay(25);
    
    unsigned long int code_in = ircode;
    unsigned long int red = 0xF720DF;
    unsigned long int blue = 0xF7609F;
    unsigned long int down = 0x5743CC33;
    unsigned long int sleeps = 0x807FE817;
    
    for(int h=0; h<2; h++){
      irSendNEC.send(code_in);   // WESTINGHOUSE SLEEP
      delay(355);
    }
    delay(10); 
  
  irRecv.enableIRIn();
  RCrecv.enableReceive(0);
  RCsend.enableTransmit(RCTX_PIN);
  RCsend.setProtocol(1);
  RCsend.setPulseLength(pulsetime);
  driver.setModeRx();
  delay(5);
  
}



//-------------------
// RADIOHEAD RX CODE 
//-------------------
void RH_RECEIVE(uint8_t *RHbuf, uint8_t RHbuflen)
{
    memset(RHMsg, '\0', sizeof(RHMsg));
    Serial.print(F("Got[")); Serial.print(RHbuflen); Serial.print(F("]:"));
    for (int i = 0; i < RHbuflen; i++){
      RHMsg[i] = char(RHbuf[i]); //Serial.print((char)RHbuf[i]);
    }
    Serial.print(RHMsg); Serial.println();
} 


//-------------------
// RCremote RX code 
//-------------------
void RC_RECEIVE()
{
  int value = RCrecv.getReceivedValue();
  Etek_code = RCrecv.getReceivedValue();
  Etek_length = RCrecv.getReceivedBitlength();
  char* b = dec2binWzerofill(Etek_code, Etek_length);
  
  if (value == 0) 
  {
    Serial.print(F("Unknown encoding"));
  } 
  else {
    Serial.print(F("Received rcREMOTE:"));
    Serial.println( Etek_code );
    if(Etek_code==11097496 | Etek_code==4551939 ){
      Serial.println(F("BELL RUNG"));
      //RH_BELLRUNG();}
  }
  
  delay(200);
  RCrecv.resetAvailable();
  
}}





//------------------------
// RC REMOTE SEND code 
//-----------------------
void RC_SEND(int etekID, int etekState)
{
  RCrecv.disableReceive();
  RCsend.enableTransmit(RCTX_PIN);
  RCsend.setProtocol(1);
  RCsend.setPulseLength(pulsetime);
  
  if(etekState==1){
    txRemoteOn(etekID);
  }
  else if(etekState==0){
    txRemoteOff(etekID);
  }
  
  RCsend.disableTransmit();
  RCrecv.enableReceive(0);
  
}

void txRemoteAllOn()
{
  for(int w=0; w<5; w++)
  {
    etekCODES etekItem;
    memcpy_P (&etekItem, &etekArr[w], sizeof etekItem);
    for(int k = 0; k<2; k++)
    {
      RCsend.sendTriState(etekItem.etekCodeArr); 
      delay(2);
    } 
    delay(20);
  }
}

void txRemoteAllOff()
{
  for(int w=10; w<20; w++)
  {
    etekCODES etekItem;
    memcpy_P (&etekItem, &etekArr[w], sizeof etekItem);
    for(int k = 0; k<2; k++)
    {
      RCsend.sendTriState(etekItem.etekCodeArr); 
      delay(2);
    } 
    delay(20);
  }
}


void txRemoteOn(int p)
{
  etekCODES etekItem1;
  etekCODES etekItem2;
  memcpy_P (&etekItem1, &etekArr[p], sizeof etekItem1);
  memcpy_P (&etekItem2, &etekArr[p+5], sizeof etekItem2);
  for(int k = 0; k<2; k++)
  {
    RCsend.sendTriState(etekItem1.etekCodeArr); 
    RCsend.sendTriState(etekItem2.etekCodeArr); 
    delay(4);
  }	
  //Serial.print(F("EtekON:")); Serial.println(etekItem1.etekIDArr);
  delay(1);
}


void txRemoteOff(int j)
{ 
  etekCODES etekItem1;
  etekCODES etekItem2;
  memcpy_P (&etekItem1, &etekArr[j+10], sizeof etekItem1);
  memcpy_P (&etekItem2, &etekArr[j+15], sizeof etekItem2);
 for(int k = 0; k<2; k++)
 {
    RCsend.sendTriState(etekItem1.etekCodeArr); 
    RCsend.sendTriState(etekItem2.etekCodeArr); 
    delay(4);
  }
  //Serial.print(F("EtekOFF:")); Serial.println(etekItem1.etekIDArr);
  delay(1);
}







//-------------------
// RC HELPER FUNCTION 
//-------------------
static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength)
{
  static char bin[64]; 
  unsigned int i=0;

  while (Dec > 0) {
    bin[32+i++] = (Dec & 1 > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }

  for (unsigned int j = 0; j< bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    }else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';
  
  return bin;
}


void LED_FLASH(int freq, int duration, byte PIN)
{
  for(int k=0; k<freq; k++)
  {
    digitalWrite(PIN, HIGH);
    delay(duration);
    digitalWrite(PIN, LOW);
    delay(duration);
  }
}



