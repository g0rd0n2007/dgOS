void MenuDraw(TTGOClass *ttgo, cButton b[]){
  ttgo->tft->fillScreen(TFT_BLACK); 
  ttgo->tft->setTextDatum(TC_DATUM);   
  ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->tft->setFreeFont(FSS9);
  ttgo->tft->drawString("Menu", 120, 10, GFXFF);
  for(int i = 0; i < 5; i++) b[i].Draw(ttgo, false);  
}

void Menu(TTGOClass *ttgo){
  Swipe s;
  uint32_t now;
  cButton _b[5] = {
    cButton(85, 205, 70, ButtonHeight, "Wstecz", ButtonOK_FG, ButtonOK_BG),
    cButton(0, 50, ButtonWidth, ButtonHeight, "Alarm"),
    cButton(60, 50, ButtonWidth, ButtonHeight, "Ekran"),
    cButton(120, 50, ButtonWidth, ButtonHeight, "Stoper"),
    cButton(180, 50, ButtonWidth, ButtonHeight, "Timer")
  };
  
  MenuDraw(ttgo, _b);
  

  do{s.Run(ttgo, millis()); } while(s.Swiping);

  do{
    now = millis();
    s.Run(ttgo, now);
    for(int i = 0; i < 5; i++) _b[i].Run(ttgo, s, now);

    if(_b[1].IsReleased()){
      AlarmPage(ttgo);
      MenuDraw(ttgo, _b);
    }
    if(_b[2].IsReleased()){
      ScreenPage(ttgo);
      MenuDraw(ttgo, _b);
    }
    if(_b[3].IsReleased()){
      StoperPage(ttgo);
      MenuDraw(ttgo, _b);
    }
    if(_b[4].IsReleased()){
      TimerPage(ttgo);
      MenuDraw(ttgo, _b);
    }
  } while(!_b[0].IsReleased());

  do{s.Run(ttgo, millis()); } while(s.Swiping);
  ttgo->tft->fillScreen(TFT_BLACK); 
}
