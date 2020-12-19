String SecToTime(long int t){
    char buf[15];
    sprintf(buf, "%02d:%02d:%02d", (t / 3600) % 99, (t / 60) % 60 ,t % 60);
    return String(buf);
}

void StoperPageDraw(TTGOClass *ttgo, cButton b[], long int ttime, int p, long int his[]){
  if(p>=2){
    ttgo->tft->fillScreen(TFT_BLACK); 
  }
  
  
  //ttgo->tft->setTextDatum(TC_DATUM);   
  ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->tft->setFreeFont(FMB18);
  //ttgo->tft->drawString("Alarm", 120, 10, GFXFF);*/

  ttgo->tft->setTextDatum(TC_DATUM);

  if(p>=0){ 
    ttgo->tft->drawString(SecToTime(ttime), 120, 10, GFXFF);
    ttgo->tft->setFreeFont(FM9);
    if(his[0] >= 0) {
      ttgo->tft->drawString(SecToTime(his[0]), 60, 90, GFXFF);
    }
    if(his[1] >= 0) {
      ttgo->tft->drawString(SecToTime(his[1]), 60, 120, GFXFF);
      ttgo->tft->drawString(SecToTime(his[0]-his[1]), 180, 90, GFXFF);
    }
    if(his[2] >= 0) {
      ttgo->tft->drawString(SecToTime(his[2]), 60, 150, GFXFF);
      ttgo->tft->drawString(SecToTime(his[1]-his[2]), 180, 120, GFXFF);
    }
    if(his[3] >= 0) {
      ttgo->tft->drawString(SecToTime(his[3]), 60, 180, GFXFF);
      ttgo->tft->drawString(SecToTime(his[2]-his[3]), 180, 150, GFXFF);
    }
    if(his[4] >= 0) {
      //ttgo->tft->drawString(SecToTime(his[4]), 60, 210, GFXFF);
      ttgo->tft->drawString(SecToTime(his[3]-his[4]), 180, 180, GFXFF);
    }
  }
  
  
  if(p>=1) for(int i = 0; i < 3; i++) b[i].Draw(ttgo, false);     
  //for(int i = 0; i < 2; i++) e[i].Draw(ttgo, 2);  
}



void StoperPage(TTGOClass *ttgo){
  Swipe s;
  uint32_t now; 
  String Stime;
  //String his[]={"", "", "", ""};
  long int his[] = { -1, -1, -1, -1, -1 };
  
  long int ttime = 0;//setne sekundy
  boolean Running = false;
  RTC_Date _t = ttgo->rtc->getDateTime();
  
  cButton _b[3] = {
    cButton(85, 205, 70, ButtonHeight, "Wstecz", ButtonOK_FG, ButtonOK_BG),
    cButton(40, 50, 70, ButtonHeight, "Start", ButtonOK_FG, ButtonOK_BG),
    cButton(130, 50, 70, ButtonHeight, "Pomiar")
  };
      
  StoperPageDraw(ttgo, _b, ttime, 2, his);
  /*ttgo->rtc->setTimer(64, 1, false);
  ttgo->rtc->disableCLK();
  ttgo->rtc->disableTimer();*/

  do{s.Run(ttgo, millis()); } while(s.Swiping);
  int last_s = -1;
  

  do{
    now = millis();
    s.Run(ttgo, now);
    for(int i = 0; i < 3; i++) _b[i].Run(ttgo, s, now); 
    //for(int i = 0; i < 2; i++) _e[i].Run(ttgo, s, now); 

    last_s = _t.second;
    _t = ttgo->rtc->getDateTime();

    if(Running && last_s != _t.second){
      //ttgo->rtc->clearTimer();
      //ttgo->rtc->setTimer(64, 1, false);
      ttime++;
      StoperPageDraw(ttgo, _b, ttime, 0, his);
    }

    if(_b[1].IsPressed()){
      Running = !Running;
      if(Running){
        //ttgo->rtc->enableCLK(1);
        //ttgo->rtc->enableTimer();
        last_s = _t.second;
        _b[1].Text = "Stop";
        _b[2].Text = "Pomiar";
      }else{
        //ttgo->rtc->disableCLK();
        //ttgo->rtc->disableTimer();
        _b[1].Text = "Start";
        _b[2].Text = "Reset";
      }
      StoperPageDraw(ttgo, _b, ttime, 1, his);
    }
    if(_b[2].IsPressed()){
      if(Running){
        his[4] = his[3];
        his[3] = his[2];
        his[2] = his[1];
        his[1] = his[0];
        his[0] = ttime; 
      }else{
        ttime = 0;
      }
      StoperPageDraw(ttgo, _b, ttime, 0, his);
    }
    
  } while(!_b[0].IsReleased());

  do{s.Run(ttgo, millis()); } while(s.Swiping);
  ttgo->tft->fillScreen(TFT_BLACK); 
}
