void displayBattery(TFT_eSprite *g){
  char txt[20];
  static uint16_t LastPercentage;
  uint16_t Percentage;

  LastPercentage = Percentage;
  Percentage = ttgo->power->getBattPercentage();
  //if(Percentage != LastPercentage) ttgo->tft->fillRect(180, 0, 60, TopBarHeight, TFT_BLACK);

  
  if (ttgo->power->isVBUSPlug()) g->setTextColor(TFT_GREEN, TFT_GREEN);
  else if (Percentage >= 20) g->setTextColor(TFT_WHITE, TFT_WHITE);
  else if (Percentage >= 10) g->setTextColor(TFT_YELLOW, TFT_YELLOW);
  else g->setTextColor(TFT_RED, TFT_RED);

  
  sprintf(txt, "%d%%", Percentage);
  g->setTextDatum(TR_DATUM);
  g->setFreeFont(FSS9);
  g->drawString(txt, 230, 10, GFXFF);  
}



void displayAlarm(TFT_eSprite *g){
  AlarmActive = ttgo->rtc->isAlarmEnabled();


  if(AlarmActive) g->drawXBitmap(160, 10, alarmIcon, 16, 16, TFT_WHITE);
   
  /*if(AlarmActive) ttgo->tft->setTextColor(TFT_WHITE);
  else ttgo->tft->setTextColor(0x3186);
  
  ttgo->tft->setTextDatum(TC_DATUM);
  ttgo->tft->setFreeFont(FSS9);
  ttgo->tft->drawString("Alm", 150, 10, GFXFF); */ 
}

void DisplaySteps(TFT_eSprite *g){
  g->drawXBitmap(10, 10, stepIcon, 16, 16, TFT_WHITE);

  g->setTextColor(TFT_WHITE);
  g->setFreeFont(FSS9);
  g->setTextDatum(TL_DATUM); 
  g->drawString(String(StepCount) + " ", 30, 10, GFXFF);  
}
