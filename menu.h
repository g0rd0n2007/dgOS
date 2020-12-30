void MenuDraw(TFT_eSprite *g, cButton b[]){
  g->fillSprite(TFT_BLACK); 
  g->setTextDatum(TC_DATUM);   
  g->setTextColor(TFT_WHITE, TFT_BLACK);
  g->setFreeFont(FSS9);
  g->drawString("Menu", 120, 10, GFXFF);
  for(int i = 0; i < 6; i++) b[i].Draw(g);  
  g->pushSprite(0, 0);
}

void Menu(TTGOClass *ttgo){
  Swipe s;
  uint32_t now;
  cButton _b[6] = {
    cButton(85, 205, 70, ButtonHeight, "<", ButtonOK_FG, ButtonOK_BG),
    cButton(0, 50, 73, ButtonHeight, "Alarm"),
    cButton(83, 50, 73, ButtonHeight, "Ekran"),
    cButton(166, 50, 73, ButtonHeight, "Stoper"),
    cButton(0, 90, 73, ButtonHeight, "Timer"),
    cButton(83, 90, 73, ButtonHeight, "Kroki")
  };
  
  MenuDraw(g, _b);
  

  do{s.Run(ttgo, millis()); } while(s.Swiping);

  do{
    now = millis();
    s.Run(ttgo, now);
    for(int i = 0; i < 6; i++) _b[i].Run(g, s, now);

    if(_b[1].IsReleased()){
      AlarmPage(ttgo);
      MenuDraw(g, _b);
    }
    if(_b[2].IsReleased()){
      ScreenPage(ttgo);
      MenuDraw(g, _b);
    }
    if(_b[3].IsReleased()){
      StoperPage(ttgo);
      MenuDraw(g, _b);
    }
    if(_b[4].IsReleased()){
      TimerPage(ttgo);
      MenuDraw(g, _b);
    }
    if(_b[5].IsReleased()){
      StepsPage(ttgo);
      MenuDraw(g, _b);
    }
  } while(!_b[0].IsReleased());

  do{s.Run(ttgo, millis()); } while(s.Swiping);
  //ttgo->tft->fillScreen(TFT_BLACK); 
}
