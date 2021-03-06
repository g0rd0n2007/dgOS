String SecToTime(long int t){
    char buf[15];
    sprintf(buf, "%02d:%02d:%02d", (t / 3600) % 99, (t / 60) % 60 ,t % 60);
    return String(buf);
}

void StoperPageDraw(TFT_eSprite *g, cButton b[], long int ttime, int p, long int his[]){
  g->fillSprite(TFT_BLACK); 
    
  //ttgo->tft->setTextDatum(TC_DATUM);   
  g->setTextColor(TFT_WHITE, TFT_BLACK);
  g->setFreeFont(FSS18);
  //ttgo->tft->drawString("Alarm", 120, 10, GFXFF);*/

  g->setTextDatum(TC_DATUM);

  g->drawString(SecToTime(ttime), 120, 10, GFXFF);
  g->setFreeFont(FSS9);
  if(his[0] >= 0) {
      g->drawString(SecToTime(his[0]), 60, 90, GFXFF);
  }
  if(his[1] >= 0) {
      g->drawString(SecToTime(his[1]), 60, 120, GFXFF);
      g->drawString(SecToTime(his[0]-his[1]), 180, 90, GFXFF);
  }
  if(his[2] >= 0) {
      g->drawString(SecToTime(his[2]), 60, 150, GFXFF);
      g->drawString(SecToTime(his[1]-his[2]), 180, 120, GFXFF);
  }
  if(his[3] >= 0) {
      g->drawString(SecToTime(his[3]), 60, 180, GFXFF);
      g->drawString(SecToTime(his[2]-his[3]), 180, 150, GFXFF);
  }
  if(his[4] >= 0) {
      //ttgo->tft->drawString(SecToTime(his[4]), 60, 210, GFXFF);
      g->drawString(SecToTime(his[3]-his[4]), 180, 180, GFXFF);
  }
  
  
  
  for(int i = 0; i < 3; i++) b[i].Draw(g);     
  //for(int i = 0; i < 2; i++) e[i].Draw(ttgo, 2);  

  g->pushSprite(0, 0);
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
    cButton(85, 205, 70, ButtonHeight, "<", ButtonOK_FG, ButtonOK_BG),
    cButton(30, 50, 70, ButtonHeight, "Start", ButtonOK_FG, ButtonOK_BG),
    cButton(140, 50, 70, ButtonHeight, "Pomiar", TFT_WHITE, 0x3016)
  };
      
  StoperPageDraw(g, _b, ttime, 2, his);
  /*ttgo->rtc->setTimer(64, 1, false);
  ttgo->rtc->disableCLK();
  ttgo->rtc->disableTimer();*/

  do{s.Run(ttgo, millis()); } while(s.Swiping);
  int last_s = -1;
  

  do{
    now = millis();
    s.Run(ttgo, now);
    for(int i = 0; i < 3; i++) _b[i].Run(g, s, now); 
    //for(int i = 0; i < 2; i++) _e[i].Run(ttgo, s, now); 

    last_s = _t.second;
    _t = ttgo->rtc->getDateTime();

    if(Running && last_s != _t.second){
      //ttgo->rtc->clearTimer();
      //ttgo->rtc->setTimer(64, 1, false);
      ttime++;
      StoperPageDraw(g, _b, ttime, 0, his);
    }

    if(_b[1].IsPressed()){
      Running = !Running;
      if(Running){
        //ttgo->rtc->enableCLK(1);
        //ttgo->rtc->enableTimer();
        last_s = _t.second;
        _b[1].Text = "Stop";
        _b[2].Text = "Pomiar";
        _b[2].BG = 0x3016;
      }else{
        //ttgo->rtc->disableCLK();
        //ttgo->rtc->disableTimer();
        _b[1].Text = "Start";
        _b[2].Text = "Reset";
        _b[2].BG = ButtonNo_BG;
      }
      StoperPageDraw(g, _b, ttime, 1, his);
      ttgo->motor->onec();
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
      StoperPageDraw(g, _b, ttime, 0, his);
    }
    
  } while(!_b[0].IsReleased());

  do{s.Run(ttgo, millis()); } while(s.Swiping);
  //ttgo->tft->fillScreen(TFT_BLACK); 
}
