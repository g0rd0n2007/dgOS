void TimerPageDraw(TFT_eSprite *g, cButton b[], long int ttime, int p){
  g->fillSprite(TFT_BLACK); 
  
  
  //ttgo->tft->setTextDatum(TC_DATUM);   
  g->setTextColor(TFT_WHITE, TFT_BLACK);
  g->setFreeFont(FSS18);
  //ttgo->tft->drawString("Alarm", 120, 10, GFXFF);*/

  g->setTextDatum(TC_DATUM);
  g->drawString(SecToTime(ttime), 120, 10, GFXFF);
  
  for(int i = 0; i < 3; i++) b[i].Draw(g);     
  //for(int i = 0; i < 2; i++) e[i].Draw(ttgo, 2);  

  g->pushSprite(0, 0);
}



void TimerPage(TTGOClass *ttgo){
  Swipe s;
  uint32_t now; 
  String Stime;
  //String his[]={"", "", "", ""};
  //long int his[] = { -1, -1, -1, -1, -1 };
  
  long int ttime = 5 * 60, setpoint = 5 *60;//sekundy
  boolean Running = false;
  RTC_Date _t = ttgo->rtc->getDateTime();
  
  cButton _b[3] = {
    cButton(85, 205, 70, ButtonHeight, "<", ButtonOK_FG, ButtonOK_BG),
    cButton(30, 50, 70, ButtonHeight, "Start", ButtonOK_FG, ButtonOK_BG),
    cButton(140, 50, 70, ButtonHeight, "Ustaw", TFT_WHITE, 0x3016)
  };
      
  TimerPageDraw(g, _b, ttime, 2);
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
      if(ttime > 0){
        ttime--;
        TimerPageDraw(g, _b, ttime, 0);
      }else{
        Timer alm_t, to;
        boolean bo = true;

        alm_t.Set(0, 500);
        to.Set(millis(), 20000);
        
        do{
          now = millis();
          s.Run(ttgo, now); 

          if(alm_t.TON(now, true)){
            if(bo){
              ttgo->motor->onec();
            }            
            bo = !bo;          
          }
        } while(!s.Swiping && !to.TON(Now));
        Running = false;        
        _b[1].Text = "Start";   
        _b[2].Text = "Ustaw"; 
        TimerPageDraw(g, _b, ttime, 1);
      }      
    }

    if(_b[1].IsPressed()){
      Running = !Running;
      if(Running){
        //ttgo->rtc->enableCLK(1);
        //ttgo->rtc->enableTimer();
        last_s = _t.second;
        _b[1].Text = "Stop"; 
        _b[2].Text = "Reset";       
      }else{
        //ttgo->rtc->disableCLK();
        //ttgo->rtc->disableTimer();
        _b[1].Text = "Start";   
        _b[2].Text = "Ustaw";    
      }
      TimerPageDraw(g, _b, ttime, 1);
      ttgo->motor->onec();
    }
    if(_b[2].IsPressed()){
      if(Running){
        ttime = setpoint;
      }else{
        String txt;

        txt=SecToTime(setpoint);
        if(getInput(g, "Ustawianie czasu", &txt, 8, BPLUS_HOUR) == Result_OK){
          long int a, b, c;
          sscanf(txt.c_str(), "%ld:%ld:%ld", &a, &b, &c);
          ttime=setpoint=a*3600+b*60+c;
        }      
      }
      TimerPageDraw(g, _b, ttime, 1);
    }
  } while(!_b[0].IsReleased());

  do{s.Run(ttgo, millis()); } while(s.Swiping);
  //ttgo->tft->fillScreen(TFT_BLACK); 
}
