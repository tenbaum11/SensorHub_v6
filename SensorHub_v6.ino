#include <MemoryFree.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#include <RCSwitch.h>
#include <IRLib.h>

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

const byte ANA1_PIN = A1;  // 
const byte ANA2_PIN = A2;  // LDR
const byte DIG1_PIN = 7;  // PIR
const byte DIG2_PIN = 11;  // 

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
IRsend irSend;             // UNO PWM PIN 3
IRsendNEC irSendNEC;       // irlib
IRdecode irDecoder;        // irlib
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
unsigned long previousMillis = 0;
unsigned long prevMillisPRINT = 0;
const unsigned long interval = 3000;  
const unsigned long intervalPRINT = 3000; 



// #######################################################
// SETUP
// #######################################################
void setup() {
  Serial.begin(9600);
    // if(!driver.init()) {Serial.println(F("init failed"));}  // screwed up ir library
  
  pinMode(ANA1_PIN, INPUT);
  pinMode(ANA2_PIN, INPUT);
  pinMode(mANA_PIN, INPUT);
  pinMode(DIG1_PIN, INPUT);
  pinMode(DIG2_PIN, INPUT); 
  pinMode(mDIG_PIN, INPUT);
  
  pinMode(mGND_PIN, OUTPUT); digitalWrite(mGND_PIN, LOW);
  pinMode(GND1_PIN, OUTPUT); digitalWrite(GND1_PIN, LOW);
  pinMode(PWR1_PIN, OUTPUT); digitalWrite(PWR1_PIN, HIGH);
  pinMode(mPWR_PIN, OUTPUT); digitalWrite(mPWR_PIN, HIGH);
  pinMode(LED1_PIN, OUTPUT); digitalWrite(LED1_PIN, HIGH);
  
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
  
  //***************************************
  // SENSOR code 
  //***************************************
  int  LDR_VAL = analogRead(LDR);
  byte PIR_VAL = digitalRead(PIR);
  int  mANA_VAL = analogRead(mANA);
  byte mDIG_VAL = analogRead(mDIG); 
  
  unsigned long currentMillis=millis();
    
  if(mDIG_VAL > mic_threshold &  led_flag==0)
  {
    mic_flag = 1;
    led_flag=1;
    currentMillis = millis();
    previousMillis = currentMillis; 
    digitalWrite(LED1_PIN, HIGH);
    Serial.print(F("ANA MIC:"));     Serial.print(mANA_VAL);
    Serial.print(F(" | DIG MIC: ")); Serial.println(mDIG_VAL); 
  }
 
  if(currentMillis - previousMillis >= interval && mic_flag==1) 
  {
    unsigned long currentMillis = millis();
    digitalWrite(LED1_PIN, LOW);
    previousMillis = currentMillis;   
    mic_flag = 0;
    led_flag = 0;
    Serial.println(F("LIGHT OFF")); 
  }
  
  unsigned long currtMillisPRINT=millis();
  if(currtMillisPRINT - prevMillisPRINT >= intervalPRINT) {
    prevMillisPRINT = currtMillisPRINT;   
    Serial.print(F("LDR:"));     Serial.print(LDR_VAL);
    Serial.print(F(" | PIR: ")); Serial.println(PIR_VAL);
    RC_SEND(0,1);
    RC_SEND(1,1);
    IR_SEND(0xF7E01F); 
    delay(3000);
    IR_SEND(0xF750AF);

  }
  
  if(PIR_VAL!=0){
    digitalWrite(LED1_PIN, HIGH);
  } 
  else{
    digitalWrite(LED1_PIN, LOW);
  }


  

  //Serial.println();
  
  //***************************************
  // RCremote RX code 
  //***************************************
  int etekState  = -1;
  int etekSwitch = -1;
  if (RCrecv.available()) {
    delay(1);
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
  else{}

  irValue=0;
  delay(40);

}  // # END MAIN LOOP 




// #######################################################
// FUNCTIONS
// #######################################################

//-------------------
// IR REMOTE SEND 
//-------------------
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
    
    for(int h=0; h<3; h++){
      irSendNEC.send(code_in);   // WESTINGHOUSE SLEEP
      delay(100);
    }
    delay(10); 
    
//    for(int h=0; h<3; h++){
//      irSendNEC.send(0xF7E01F);   // WESTINGHOUSE SLEEP
//      delay(100);
//    }
//    for(int h=0; h<3; h++){
//      irSendNEC.send(0xF750AF);   // WESTINGHOUSE SLEEP
//      delay(100);
//    }
    
  //irRecv.enableIRIn();
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
  //bb = dec2binWzerofill(Etek_code, Etek_length);
  
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



