/*char buf[50];
      sprintf(buf, "Czy wlaczyc alarm? (%d:%02d)", alarm_hh, alarm_mm);
          
      if(MessageBox(ttgo, "Pytanie", String(buf), Dialog_OK_No) == Result_OK) {
        String a = String(alarm_hh);
        String b = String(alarm_mm);
      
        if(getInput(ttgo, "Godzina alarmu:", &a, 2) && getInput(ttgo, "Minuta alarmu:", &b, 2)){
          ttgo->rtc->disableAlarm();

          alarm_hh = a.toInt();
          alarm_mm = b.toInt();          
          ttgo->rtc->setAlarm(alarm_hh, alarm_mm, PCF8563_NO_ALARM, PCF8563_NO_ALARM);
          ttgo->rtc->enableAlarm();
          AlarmActive = true;

          sprintf(buf, "Godzina alarmu: %d:%02d", alarm_hh, alarm_mm);
          MessageBox(ttgo, "Info", String(buf), Dialog_OK);
        }
      }else{
        ttgo->rtc->disableAlarm();
        AlarmActive = false;
      }
      */
      
void AlarmPageDraw(TTGOClass *ttgo, cButton b[], cEdit e[]){
  ttgo->tft->fillScreen(TFT_BLACK); 
  ttgo->tft->setTextDatum(TC_DATUM);   
  ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->tft->setFreeFont(FSS9);
  ttgo->tft->drawString("Alarm", 120, 10, GFXFF);

  ttgo->tft->setTextDatum(TL_DATUM);
  ttgo->tft->drawString("Godzina:", 10, 55, GFXFF);
  
  for(int i = 0; i < 1; i++) b[i].Draw(ttgo, false);  
  for(int i = 0; i < 1; i++) e[i].Draw(ttgo); 
 
  
  
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
  
  AlarmPageDraw(ttgo, _b, _e);
  

  do{s.Run(ttgo, millis()); } while(s.Swiping);

  do{
    now = millis();
    s.Run(ttgo, now);
    for(int i = 0; i < 1; i++) _b[i].Run(ttgo, s);
    for(int i = 0; i < 1; i++) _e[i].Run(ttgo, s, now);

    if(_e[0].IsReleased()){
      if(getInput(ttgo, "Godzina alarmu:", &Sgodzina, 5, BPLUS_HOUR)){
          //ttgo->rtc->disableAlarm();

        sscanf(Sgodzina.c_str(), "%d:%2d", &alarm_hh, &alarm_mm);
        ttgo->rtc->disableAlarm();
        ttgo->rtc->setAlarm(alarm_hh, alarm_mm, PCF8563_NO_ALARM, PCF8563_NO_ALARM);
        sprintf(godzina, "%d:%02d", alarm_hh, alarm_mm);
        Sgodzina = String(godzina);
        if(MessageBox(ttgo, "Pytanie", "Wlaczyc alarm o " + Sgodzina, Dialog_OK_No) == Result_OK){
          ttgo->rtc->enableAlarm();
          AlarmActive = true;
        }else{
          ttgo->rtc->disableAlarm();
          ttgo->rtc->resetAlarm();
          AlarmActive = false;
        }        
      }
      AlarmPageDraw(ttgo, _b, _e);
    }
        //ttgo->rtc->disableAlarm();
        //AlarmActive = false;
      
     
    
  } while(!_b[0].IsReleased());

  do{s.Run(ttgo, millis()); } while(s.Swiping);
  ttgo->tft->fillScreen(TFT_BLACK); 
}
