#include "config.h"
#include <soc/rtc.h>
#include <time.h>
#include <inttypes.h>

#include "timer.h"
#include "Free_Fonts.h"
//#include "SPIFFS.h"
TTGOClass *ttgo;

Timer drawT, sleepT;
uint32_t Now;
boolean irqAPX202 = false, irqRTC = false;
String SecToTime(long int t);

#include "icons.h"
#include "displayTime.h"
#include "displayBattery.h"
#include "sleepMode.h"
#include "swipeHandle.h"
#include "Controls.h"
#include "getInput.h"

#include "alarm_page.h"
#include "screen_page.h" 
#include "stoper_page.h"
#include "timer_page.h"
#include "menu.h"


cButton cMenu = cButton(85, 205, 70, ButtonHeight, "...", TFT_WHITE, TFT_BLACK);
Swipe S;

void S_Release(){
  sleepT.SetNext(millis());
}

boolean S_Timeout(Swipe *s, uint32_t t){
  if((s->CatchTime + 1000) < t){
    if(s->CatchInRect(0, 40, 240, 80)){  
      String a;
      char buf[10];

      sprintf(buf, "%d:%02d", hh, mm);
      a = String(buf);
      
      if(getInput(ttgo, "Ustawianie godziny:", &a, 5, BPLUS_HOUR)){
        sscanf(a.c_str(), "%d:%d", &hh, &mm);
        ttgo->rtc->setDateTime(yyear, mmonth, dday, hh, mm, 0);
      }
      sleepT.SetNext(millis());
      return true;
    }
    if(s->CatchInRect(0, 120, 240, 80)){
      String a;
      char buf[15];

      sprintf(buf, "%d/%d/%04d", dday, mmonth, yyear);
      a = String(buf);

      if(getInput(ttgo, "Ustawianie daty:", &a, 10, BPLUS_DATE)){
        sscanf(a.c_str(), "%d/%d/%d", &dday, &mmonth, &yyear);
        ttgo->rtc->setDateTime(yyear, mmonth, dday, hh, mm, ss);
      }
      sleepT.SetNext(millis());
      return true;
    }
  }
  
  return false;
}

void DrawDesktop(){
  displayTime(); 
  displayBattery(); 
  displayAlarm();
  cMenu.Draw(ttgo);
}

void setup()
{    
    ttgo = TTGOClass::getWatch();
    ttgo->begin();

    ttgo->openBL(); // Turn on the backlight
    ttgo->bl->adjust(BacklightLevel);
    
    // ADC monitoring must be enabled to use the AXP202 monitoring function
    ttgo->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
    //Przycisk
    pinMode(AXP202_INT, INPUT_PULLUP);
    pinMode(RTC_INT_PIN, INPUT_PULLUP);
    
    attachInterrupt(AXP202_INT, [] {
        irqAPX202 = true;
    }, FALLING);
    attachInterrupt(RTC_INT_PIN, [] {
        irqRTC = true;
    }, FALLING);
    
    //!Clear IRQ unprocessed  first
    ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, true);
    ttgo->power->clearIRQ();

    ttgo->motor_begin();
    
    ttgo->rtc->check();
    ttgo->rtc->syncToSystem();
    ttgo->rtc->disableTimer();
    ttgo->rtc->disableCLK();
    ttgo->rtc->clearTimer();
    //RTC_Alarm alm = ttgo->rtc->getAlarm();
    //alarm_hh = alm.hour;
    //alarm_mm = alm.minute;
    ttgo->rtc->setAlarm(Alarm_hh[0], Alarm_mm[0], PCF8563_NO_ALARM, PCF8563_NO_ALARM, Alarm_Active[0]);
    ttgo->rtc->disableAlarm();
    AlarmActive = ttgo->rtc->isAlarmEnabled();
    
    
    Now = millis();
    drawT.Set(Now, 1000);
    sleepT.Set(Now, 5000);
    S.Release = S_Release;
    S.TimeOut = S_Timeout;
    
    DrawDesktop();
    //ttgo->motor->onec();
    setCpuFrequencyMhz(40);
}

void loop()
{
    Now = millis();

    if(!SleepMode){
      S.Run(ttgo, Now);
      cMenu.Run(ttgo, S, Now);

      if(cMenu.IsReleased()){
        Menu(ttgo);
        sleepT.SetNext(millis());
        drawT.TargetTime = 0;
      }
      
      if (drawT.TON(Now)) {
        drawT.SetNext(Now);
        DrawDesktop();
      }

      if(sleepT.TON(Now)){
        //Zasypiamy              
        StartLowPowerMode();   
        ttgo->power->clearIRQ(); irqAPX202 = false;     
      }  
    }
    
    
    if(irqAPX202){
      irqAPX202 = false;
      ttgo->power->readIRQ();
      if (ttgo->power->isVbusPlugInIRQ()) {
        if(SleepMode) WakeUp(Now);
        displayBattery();
        sleepT.SetNext(Now);
      }
      if (ttgo->power->isVbusRemoveIRQ()) {
        if(SleepMode) WakeUp(Now);
        displayBattery();
        sleepT.SetNext(Now);
      }
      if(ttgo->power->isChargingDoneIRQ()){
        if(SleepMode) WakeUp(Now);
        displayBattery();
        sleepT.SetNext(Now);
        ttgo->motor->onec();
      }
      if (ttgo->power->isPEKShortPressIRQ()) {
        if(SleepMode){
          WakeUp(Now);
        }else{
          ttgo->power->clearIRQ();
          StartLowPowerMode();
        }
      }
      ttgo->power->clearIRQ();      
    }
    if(ttgo->rtc->alarmActive()){
      irqRTC = false;
      ttgo->rtc->resetAlarm();
      Timer alm_t, to;
      boolean bo = true;

      alm_t.Set(0, 500);
      to.Set(millis(), 20000);
      if(SleepMode) WakeUp(millis());

      ttgo->tft->setFreeFont(FSS18);
      ttgo->tft->setTextDatum(MC_DATUM);

      do{
        Now = millis();
        S.Run(ttgo, Now); 

        if(alm_t.TON(Now, true)){
          if(bo){
            ttgo->tft->fillScreen(TFT_BLACK); 
            ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
            ttgo->motor->onec();
          }else{
            ttgo->tft->fillScreen(TFT_WHITE);
            ttgo->tft->setTextColor(TFT_BLACK, TFT_WHITE); 
          }
          ttgo->tft->drawString(CurAlmName, 120, 120, GFXFF);
          
          bo = !bo;          
        }
      } while(!S.Swiping && !to.TON(Now));

      ttgo->tft->fillScreen(TFT_BLACK); 
      sleepT.SetNext(Now);
      drawT.TargetTime = 0;
      TrySetNearestAlarm();
    }
}
