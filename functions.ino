// MIC FUNCTION

void MIC_FUNC(int mDIG_VAL, int mANA_VAL)
{
  unsigned long currMill = millis();
  const int mic_threshold = 500;
  if(mDIG_VAL>mic_threshold && mic_flag==0)
  {
    mic_flag = 1;
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
    Serial.println(F("LIGHT OFF")); 
  }

 
}
