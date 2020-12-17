#include <dummy.h>

boolean SleepMode = false;
Timer sleepT;


void WakeUp(uint32_t now){
  setCpuFrequencyMhz(40);
  SleepMode = false;
  sleepT.SetNext(now);
  ttgo->displayWakeup();
  ttgo->rtc->syncToSystem();  
  ttgo->openBL();  
}

void StartSleep(){
  SleepMode = true;
  
  // Set  touchscreen sleep
  ttgo->displaySleep();
  setCpuFrequencyMhz(5);
  ttgo->closeBL();
}

void ShutDown(){
  SleepMode = true;

  // Set  touchscreen sleep
  ttgo->displaySleep();
  setCpuFrequencyMhz(5);
  ttgo->closeBL();
  
#ifdef LILYGO_WATCH_2020_V1
    ttgo->powerOff();
    // LDO2 is used to power the display, and LDO2 can be turned off if needed
    // power->setPowerOutPut(AXP202_LDO2, false);
#else
    ttgo->power->setPowerOutPut(AXP202_LDO3, false);
    ttgo->power->setPowerOutPut(AXP202_LDO4, false);
    ttgo->power->setPowerOutPut(AXP202_LDO2, false);
    // The following power channels are not used
    ttgo->power->setPowerOutPut(AXP202_EXTEN, false);
    ttgo->power->setPowerOutPut(AXP202_DCDC2, false);
#endif

    // Use ext0 for external wakeup
    //esp_sleep_enable_ext0_wakeup((gpio_num_t)AXP202_INT , LOW);
    //esp_sleep_enable_ext0_wakeup((gpio_num_t)RTC_INT_PIN, LOW);

    // Use ext1 for external wakeup
    //esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
    
    //esp_sleep_enable_ext1_wakeup((1<<AXP202_INT) | (1<<RTC_INT_PIN), ESP_EXT1_WAKEUP_ANY_HIGH);
    //esp_sleep_enable_ext1_wakeup((gpio_num_t)RTC_INT_PIN, ESP_EXT1_WAKEUP_ANY_LOW);


    //esp_sleep_enable_timer_wakeup(10000000);
    
    
    //esp_deep_sleep_start();


    //Light Sleep

    gpio_wakeup_enable((gpio_num_t)AXP202_INT , GPIO_INTR_LOW_LEVEL);
    gpio_wakeup_enable((gpio_num_t)RTC_INT_PIN , GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();
    esp_light_sleep_start();


    WakeUp(millis());
}


void StartLowPowerMode(){
  ShutDown();
}
