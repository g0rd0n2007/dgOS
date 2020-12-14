      
void AlarmPageDraw(TTGOClass *ttgo, cButton b[], cEdit e[], cCheckbox c[]){
  ttgo->tft->fillScreen(TFT_BLACK); 
  ttgo->tft->setTextDatum(TC_DATUM);   
  ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->tft->setFreeFont(FSS9);
  ttgo->tft->drawString("Alarm", 120, 10, GFXFF);

  ttgo->tft->setTextDatum(TL_DATUM);
  ttgo->tft->drawString("Godzina:", 10, 55, GFXFF);
  
  for(int i = 0; i < 1; i++) b[i].Draw(ttgo, false);  
  for(int i = 0; i < 1; i++) e[i].Draw(ttgo);
  for(int i = 0; i < 1; i++) c[i].Draw(ttgo); 
 
  
  
}

void AlarmPage(TTGOClass *ttgo){
  Swipe s;
  uint32_t now;
  char godzina[10];

  sprintf(godzina, "%d:%02d", alarm_hh, alarm_mm);
  String Sgodzina = String(godzina);
  
  cButton _b[1] = {
    cButton(100, 200, ButtonWidth, ButtonHeight, "Back", ButtonOK_FG, ButtonOK_BG)//,
    //cButton(0, 30, ButtonWidth, ButtonHeight, "Alarm")
  };
  cEdit _e[1] = {
    cEdit(120, 50, 100, ButtonHeight, &Sgodzina, false)
  };
  cCheckbox _c[1] ={
    cCheckbox(120, 100, ButtonWidth, ButtonHeight, ttgo->rtc->isAlarmEnabled())
  };
  
  AlarmPageDraw(ttgo, _b, _e, _c);
  

  do{s.Run(ttgo, millis()); } while(s.Swiping);

  do{
    now = millis();
    s.Run(ttgo, now);
    for(int i = 0; i < 1; i++) _b[i].Run(ttgo, s);
    for(int i = 0; i < 1; i++) _e[i].Run(ttgo, s, now);
    for(int i = 0; i < 1; i++) _c[i].Run(ttgo, s);

    if(_e[0].IsReleased()){
      if(getInput(ttgo, "Godzina alarmu:", &Sgodzina, 5, BPLUS_HOUR)){
        sscanf(Sgodzina.c_str(), "%d:%2d", &alarm_hh, &alarm_mm);        
        sprintf(godzina, "%d:%02d", alarm_hh, alarm_mm);
        Sgodzina = String(godzina);
        
        ttgo->rtc->setAlarm(alarm_hh, alarm_mm, PCF8563_NO_ALARM, PCF8563_NO_ALARM, _c[0].Checked);
        ttgo->rtc->resetAlarm();
      }
      AlarmPageDraw(ttgo, _b, _e, _c);
    }
    if(_c[0].IsReleased()){
      ttgo->rtc->setAlarm(alarm_hh, alarm_mm, PCF8563_NO_ALARM, PCF8563_NO_ALARM, _c[0].Checked);
      ttgo->rtc->resetAlarm();
    }
    
  } while(!_b[0].IsReleased());

  do{s.Run(ttgo, millis()); } while(s.Swiping);
  ttgo->tft->fillScreen(TFT_BLACK); 
}
