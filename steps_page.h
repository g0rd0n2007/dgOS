int StepsHistory[7] = {0, 30, 50, 30, 50, 10, 20}, StepsMax = 50;

void StepsMoveHistory(int s, bool shift){
  if(shift){
    StepsHistory[6] = StepsHistory[5];
    StepsHistory[5] = StepsHistory[4];
    StepsHistory[4] = StepsHistory[3];
    StepsHistory[3] = StepsHistory[2];
    StepsHistory[2] = StepsHistory[1];
    StepsHistory[1] = StepsHistory[0];
    StepsHistory[0] = 0;
  }else{
    StepsHistory[0] = s;
  }
  

  StepsMax=0;
  for(int i=0;i<7;i++){
    if(StepsHistory[i]>StepsMax)StepsMax=StepsHistory[i];
  }
  
}

void StepsPageDraw(TFT_eSprite *g, cButton b[]){
  int bar_w = 10, bar_dis = 30, ch_x = 20, ch_y = 40;
  RTC_Date tnow = ttgo->rtc->getDateTime();
  
  StepsMoveHistory(StepCount, false); 


  g->fillSprite(TFT_BLACK); 
  //ttgo->tft->setTextDatum(TC_DATUM);   
  g->setTextColor(TFT_WHITE, TFT_BLACK);
  g->setFreeFont(FSS9);


  g->setTextDatum(TC_DATUM);
  g->drawString("Wykres tygodniowy", 120, 10, GFXFF);

  g->setTextDatum(TL_DATUM); 
  g->drawFastHLine(ch_x, ch_y + 130, bar_dis * 7, TFT_WHITE);
  g->drawFastVLine(ch_x, ch_y + 28, 102, TFT_WHITE);
  //g->drawString("0", 65, 135, GFXFF);
  g->drawString(String(StepsMax), ch_x, ch_y, GFXFF);

  int h = 0, d;
  g->setTextDatum(TC_DATUM); 
  
  for(int i=0; i<7; i++){
    h = (100 * StepsHistory[i]) / StepsMax;
    RTC_Date d = tnow.SubDays(i);
    //75
    //95
    //115
    //135
    //155
    //175
    //195
    g->fillRect(ch_x + 3 + bar_dis * (6 - i) + bar_dis / 2 - bar_w / 2, ch_y + 128 - h, bar_w, h, (i == 0)?TFT_YELLOW:ButtonOK_BG);
    g->drawString(String(d.day), ch_x + 2 + bar_dis * (6 - i) + bar_dis / 2, ch_y + 135, GFXFF);
  }

   
  for(int i = 0; i < 1; i++) b[i].Draw(g);  

  g->pushSprite(0, 0);
}



void StepsPage(TTGOClass *ttgo){
  Swipe s;
  uint32_t now; 
    
  cButton _b[1] = {
    cButton(85, 205, 70, ButtonHeight, "<", ButtonOK_FG, ButtonOK_BG)//,
    //cButton(0, 30, ButtonWidth, ButtonHeight, "Alarm")
  };
  
    
  StepsPageDraw(g, _b);
  

  do{s.Run(ttgo, millis()); } while(s.Swiping);

  do{
    now = millis();
    s.Run(ttgo, now);
    for(int i = 0; i < 1; i++) _b[i].Run(g, s, now); 
    //for(int i = 0; i < 2; i++) _e[i].Run(ttgo, s, now);    

    
  } while(!_b[0].IsReleased());

  do{s.Run(ttgo, millis()); } while(s.Swiping);
  //ttgo->tft->fillScreen(TFT_BLACK); 
}
