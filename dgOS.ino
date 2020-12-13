#include "config.h"
#include <soc/rtc.h>
#include <time.h>
#include <inttypes.h>

#include "timer.h"
#include "Free_Fonts.h"
TTGOClass *ttgo;

Timer drawT;
uint32_t Now;
boolean irqAPX202 = false, irqRTC = false;

#include "displayTime.h"
#include "displayBattery.h"
#include "sleepMode.h"
#include "swipeHandle.h"
#include "Controls.h"
#include "getInput.h"

#include "alarm_page.h"
#include "menu.h"


cButton cMenu = cButton(5, 205, ButtonWidth, ButtonHeight, "Menu");
Swipe S;

void S_Release(){
  sleepT.SetNext(millis());
}

boolean S_Timeout(Swipe *s, uint32_t t){
  if((s->CatchTime + 1000) < t){
    if(s->CatchInRect(0, 40, 240, 80)){  
      String a = String(hh), b = String(mm);

      if(getInput(ttgo, "Ustawianie godziny:", &a, 2, BPLUS_NONE) && getInput(ttgo, "Ustawianie minuty:", &b, 2, BPLUS_NONE)){
        hh = a.toInt();
        mm = b.toInt();
        ttgo->rtc->setDateTime(yyear, mmonth, dday, hh, mm, 0);
      }
      sleepT.SetNext(millis());
      return true;
    }
    if(s->CatchInRect(0, 120, 240, 80)){
      String a = String(dday);
      String b = String(mmonth);
      String c = String(yyear);

      if(getInput(ttgo, "Ustawianie dnia:", &a, 2, BPLUS_NONE) && getInput(ttgo, "Ustawianie miesiaca:", &b, 2, BPLUS_NONE) && getInput(ttgo, "Ustawianie roku:", &c, 4, BPLUS_NONE)){
        dday = a.toInt();
        mmonth = b.toInt();
        yyear = c.toInt();
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


    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    switch(wakeup_reason)
    {
      case 4:
        if(ttgo->rtc->alarmActive()) break;
        else StartLowPowerMode();
        
        break;
      default: break;
    }

    
    ttgo->openBL(); // Turn on the backlight
    
    // ADC monitoring must be enabled to use the AXP202 monitoring function
    ttgo->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
    //Przycisk
    pinMode(AXP202_INT, INPUT_PULLUP);
    //pinMode(RTC_INT_PIN, INPUT_PULLUP);
    
    attachInterrupt(AXP202_INT, [] {
        irqAPX202 = true;
    }, FALLING);
    /*attachInterrupt(RTC_INT_PIN, [] {
        irqRTC = true;
    }, FALLING);*/
    
    //!Clear IRQ unprocessed  first
    ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, true);
    ttgo->power->clearIRQ();

    ttgo->motor_begin();

    
    ttgo->rtc->check();
    ttgo->rtc->syncToSystem();
    RTC_Alarm alm = ttgo->rtc->getAlarm();
    alarm_hh = alm.hour;
    alarm_mm = alm.minute;
    AlarmActive = ttgo->rtc->isAlarmEnabled();
    
    Now = millis();
    drawT.Set(Now, 1000);
    sleepT.Set(Now, 10000);
    S.Release = S_Release;
    S.TimeOut = S_Timeout;
    
    DrawDesktop();
    ttgo->motor->onec();
    setCpuFrequencyMhz(40);
}

void loop()
{
    Now = millis();

    if(!SleepMode){
      S.Run(ttgo, Now);
      cMenu.Run(ttgo, S);

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
          ttgo->tft->drawString("Alarm", 120, 120, GFXFF);
          
          bo = !bo;          
        }
      } while(!S.Swiping && !to.TON(Now));

      ttgo->tft->fillScreen(TFT_BLACK); 
      sleepT.SetNext(Now);
      drawT.TargetTime = 0;
    }
}