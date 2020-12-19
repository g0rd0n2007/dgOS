

      
void AlarmPageDraw(TTGOClass *ttgo, cButton b[], cEdit e[], cCheckbox c[]){
  ttgo->tft->fillScreen(TFT_BLACK); 
  /*ttgo->tft->setTextDatum(TC_DATUM);   
  ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->tft->setFreeFont(FSS9);
  ttgo->tft->drawString("Alarm", 120, 10, GFXFF);*/

  //ttgo->tft->setTextDatum(TL_DATUM);
  //ttgo->tft->drawString("Godzina:", 10, 55, GFXFF);
  
  for(int i = 0; i < 1; i++) b[i].Draw(ttgo, false);  
  for(int i = 0; i < (2 * NO_OF_ALARMS); i++) e[i].Draw(ttgo, 2);
  for(int i = 0; i < NO_OF_ALARMS; i++) c[i].Draw(ttgo); 
 
  
  
}

int HourToMin(int h, int m){
  return h * 60 + m;
}

void TrySetNearestAlarm(){
  int found = -1;
  int now = HourToMin(hh, mm), x[NO_OF_ALARMS];
  
  for(int i=0; i<NO_OF_ALARMS; i++){
    x[i] = HourToMin(Alarm_hh[i], Alarm_mm[i]);    
  }
  for(int i=0; i<NO_OF_ALARMS; i++){
    if(now < x[i] && found == -1 && Alarm_Active[i]) found = i;
    else if(now < x[i] && found >= 0 && Alarm_Active[i] && x[i] < x[found]) found = i;
  }
  if(found == -1){
    for(int i=0; i<NO_OF_ALARMS; i++){
      if(found == -1 && Alarm_Active[i]) found = i;
      else if(found >= 0 && Alarm_Active[i] && x[i] < x[found]) found = i;
    }
  }

  if(found == -1) {
    ttgo->rtc->setAlarm(Alarm_hh[0], Alarm_mm[0], PCF8563_NO_ALARM, PCF8563_NO_ALARM, false);
    ttgo->rtc->disableAlarm();
    ttgo->rtc->resetAlarm();
        //if(_c[0].Checked) ttgo->rtc->enableAlarm();
        //else ttgo->rtc->disableAlarm();
        //ttgo->rtc->resetAlarm();
  }else{
    ttgo->rtc->setAlarm(Alarm_hh[found], Alarm_mm[found], PCF8563_NO_ALARM, PCF8563_NO_ALARM, true);
    ttgo->rtc->enableAlarm();
    ttgo->rtc->resetAlarm();       
    CurAlmName = Alarm_Names[found];
  }
}









void AlarmPage(TTGOClass *ttgo){
  Swipe s;
  uint32_t now;
  char godzina0[10], godzina1[10], godzina2[10];
  char buf[15];

  sprintf(godzina0, "%d:%02d", Alarm_hh[0], Alarm_mm[0]);
  sprintf(godzina1, "%d:%02d", Alarm_hh[1], Alarm_mm[1]);
  sprintf(godzina2, "%d:%02d", Alarm_hh[2], Alarm_mm[2]);
  String Sgodzina0 = String(godzina0);
  String Sgodzina1 = String(godzina1);
  String Sgodzina2 = String(godzina2);
  
  cButton _b[1] = {
    cButton(85, 205, 70, ButtonHeight, "Wstecz", ButtonOK_FG, ButtonOK_BG)//,
    //cButton(0, 30, ButtonWidth, ButtonHeight, "Alarm")
  };
  cEdit _e[2 * NO_OF_ALARMS] = {
    cEdit(5,   5, 100, ButtonHeight, &Alarm_Names[0], false),
    cEdit(5,  75, 100, ButtonHeight, &Alarm_Names[1], false),
    cEdit(5, 145, 100, ButtonHeight, &Alarm_Names[2], false),
    
    cEdit(110,   5, 65, ButtonHeight, &Sgodzina0, false),
    cEdit(110,  75, 65, ButtonHeight, &Sgodzina1, false),
    cEdit(110, 145, 65, ButtonHeight, &Sgodzina2, false)
  };
  cCheckbox _c[NO_OF_ALARMS] ={
    cCheckbox(180,   5, ButtonWidth, ButtonHeight, Alarm_Active[0]),
    cCheckbox(180,  75, ButtonWidth, ButtonHeight, Alarm_Active[1]),
    cCheckbox(180, 145, ButtonWidth, ButtonHeight, Alarm_Active[2])
  };
  
  AlarmPageDraw(ttgo, _b, _e, _c);
  

  do{s.Run(ttgo, millis()); } while(s.Swiping);

  do{
    now = millis();
    s.Run(ttgo, now);
    for(int i = 0; i < 1; i++) _b[i].Run(ttgo, s, now);
    for(int i = 0; i < (2 * NO_OF_ALARMS); i++) _e[i].Run(ttgo, s, now);
    for(int i = 0; i < NO_OF_ALARMS; i++) _c[i].Run(ttgo, s);

    if(_e[0].IsReleased()){
      getInput(ttgo, "Nazwa dla " + Alarm_Names[0], &Alarm_Names[0], 9, BPLUS_ALFA);
      AlarmPageDraw(ttgo, _b, _e, _c);
    }
    if(_e[1].IsReleased()){
      getInput(ttgo, "Nazwa dla " + Alarm_Names[1], &Alarm_Names[1], 9, BPLUS_ALFA);
      AlarmPageDraw(ttgo, _b, _e, _c);
    }
    if(_e[2].IsReleased()){
      getInput(ttgo, "Nazwa dla " + Alarm_Names[2], &Alarm_Names[2], 9, BPLUS_ALFA);
      AlarmPageDraw(ttgo, _b, _e, _c);
    }
    
    if(_e[3].IsReleased()){
      if(getInput(ttgo, "Czas dla " + Alarm_Names[0], &Sgodzina0, 5, BPLUS_HOUR)){
        sscanf(Sgodzina0.c_str(), "%d:%d", &Alarm_hh[0], &Alarm_mm[0]);      
        sprintf(godzina0, "%d:%02d", Alarm_hh[0], Alarm_mm[0]);
        Sgodzina0 = String(godzina0);        
        Alarm_Active[0] = _c[0].Checked;
        TrySetNearestAlarm();
      }
      AlarmPageDraw(ttgo, _b, _e, _c);
    }
    if(_e[4].IsReleased()){
      if(getInput(ttgo, "Czas dla " + Alarm_Names[1], &Sgodzina1, 5, BPLUS_HOUR)){
        sscanf(Sgodzina1.c_str(), "%d:%d", &Alarm_hh[1], &Alarm_mm[1]);        
        sprintf(godzina1, "%d:%02d", Alarm_hh[1], Alarm_mm[1]);
        Sgodzina1 = String(godzina1);
        Alarm_Active[1] = _c[1].Checked;
        TrySetNearestAlarm();
      }
      AlarmPageDraw(ttgo, _b, _e, _c);
    }
    if(_e[5].IsReleased()){
      if(getInput(ttgo, "Czas dla " + Alarm_Names[2], &Sgodzina2, 5, BPLUS_HOUR)){
        sscanf(Sgodzina2.c_str(), "%d:%d", &Alarm_hh[2], &Alarm_mm[2]);        
        sprintf(godzina2, "%d:%02d", Alarm_hh[2], Alarm_mm[2]);
        Sgodzina2 = String(godzina2);
        Alarm_Active[2] = _c[2].Checked;
        TrySetNearestAlarm();
      }
      AlarmPageDraw(ttgo, _b, _e, _c);
    }



    
    if(_c[0].IsReleased()) { Alarm_Active[0] = _c[0].Checked; TrySetNearestAlarm(); }
    if(_c[1].IsReleased()) { Alarm_Active[1] = _c[1].Checked; TrySetNearestAlarm(); }
    if(_c[2].IsReleased()) { Alarm_Active[2] = _c[2].Checked; TrySetNearestAlarm(); }
  } while(!_b[0].IsReleased());

  do{s.Run(ttgo, millis()); } while(s.Swiping);
  ttgo->tft->fillScreen(TFT_BLACK); 
}
