// LED RGB ROPE
const long int ROPEon = 0xFF02FD;
const long int ROPEr1 = 0xFF9A65;
const long int ROPEg1 = 0xFF1AE5;
const long int ROPEb1 = 0xFFA25D;
const long int ROPEw  = 0xFF0022DD;
const long int ROPEbrt  = 0xFF003AC5;
const long int ROPEdim  = 0xFF00BA45;


// LED RGB BULB
const long int BULBon = 0xFF02FD;
const long int BULBr1 = 0xF720DF;
const long int BULBg1 = 0xF7A05F;
const long int BULBb1 = 0xF7609F;
const long int BULBw  = 0xF7E01F;
const long int BULBbrt  = 0xF700FF;
const long int BULBdim  = 0xF7807F;

// ROKU
const long int RKhome  =  0x5743C03F;
const long int RKok  =  0x574354AB;
const long int RKback  =  0x57436699;
const long int RKup  =  0x57439867;
const long int RKdown  =  0x5743CC33;
const long int RKleft  =  0x57437887;
const long int RKright  =  0x5743B44B;
const long int RKplay  =  0x574332CD;

// WH
const long int WHpower  =  0x20DF10EF;
const long int WHsleep  =  0x807FE817;
const long int WHmenu  =  0x807FA857;
const long int WHok  =  0x807FB847;
const long int WHexit  =  0x807F6897;
const long int WHback  =  0x807FDA25;
const long int WHsource  =  0x807F48B7;
const long int WHmute  =  0x807F8877;
const long int WHvoldwn  =  0x807FF00F;
const long int WHvolup  =  0x807F708F;
const long int WHup  =  0x807F9867;
const long int WHdown  =  0x807F58A7;
const long int WHleft  =  0x807FD827;
const long int WHright  =  0x807F38C7;

// ROOMBA 32 BITS UNKNOWN ENCODING
const long int ROOMBAclean  =  0xF1505D8A;
const long int ROOMBAdock  =  0x6FB84EC5;
const long int ROOMBAfwd  =  0xCC36FF5F;
const long int ROOMBAleft  =  0x5B2F91B5;
const long int ROOMBAright  =  0xCC36FF5E;

void IR_SEND_TEST(long int code1, byte repeat, int delayms)
{
  for(byte cnt=0; cnt<repeat; cnt++)
  {
    irSendNEC.send(code1);
    //irSendNEC.send(NEC,code1, 32); // ROKU DOWN
    delay(delayms);
  }
  
}

int delayms = 50;







