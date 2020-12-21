void displayBattery(){
  char txt[20];
  static uint16_t LastPercentage;
  uint16_t Percentage;

  LastPercentage = Percentage;
  Percentage = ttgo->power->getBattPercentage();
  if(Percentage != LastPercentage) ttgo->tft->fillRect(180, 0, 60, TopBarHeight, TFT_BLACK);

  
  if (ttgo->power->isVBUSPlug()) ttgo->tft->setTextColor(TFT_GREEN, TFT_GREEN);
  else if (Percentage >= 20) ttgo->tft->setTextColor(TFT_WHITE, TFT_WHITE);
  else if (Percentage >= 10) ttgo->tft->setTextColor(TFT_YELLOW, TFT_YELLOW);
  else ttgo->tft->setTextColor(TFT_RED, TFT_RED);

  
  sprintf(txt, "%d%%", Percentage);
  ttgo->tft->setTextDatum(TR_DATUM);
  ttgo->tft->setFreeFont(FSS9);
  ttgo->tft->drawString(txt, 230, 10, GFXFF);  
}



void displayAlarm(){
  AlarmActive = ttgo->rtc->isAlarmEnabled();


  ttgo->tft->drawXBitmap(160, 10, alarmIcon, 16, 16, (AlarmActive)? TFT_WHITE : 0x3186, TFT_BLACK);
   
  /*if(AlarmActive) ttgo->tft->setTextColor(TFT_WHITE);
  else ttgo->tft->setTextColor(0x3186);
  
  ttgo->tft->setTextDatum(TC_DATUM);
  ttgo->tft->setFreeFont(FSS9);
  ttgo->tft->drawString("Alm", 150, 10, GFXFF); */ 
}

void DisplaySteps(){
  ttgo->tft->drawXBitmap(10, 10, stepIcon, 16, 16, TFT_WHITE);

  ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->tft->setFreeFont(FSS9);
  ttgo->tft->setTextDatum(TL_DATUM); 
  ttgo->tft->drawString(String(StepCount) + " ", 30, 10, GFXFF);  
}
