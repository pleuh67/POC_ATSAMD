//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//      _____  _____ _____                   
//     |_   _|/ ____|  __ \                  
//       | | | (___ | |__) |  ___ _ __  _ __ 
//       | |  \___ \|  _  /  / __| '_ \| '_ \
//      _| |_ ____) | | \ \ | (__| |_) | |_) |
//     |_____|_____/|_|  \_(_)___| .__/| .__/
//                               | |   | |   
//                               |_|   |_|   
// ---------------------------------------------------------------------------*
#define __INIT_DONE
#include "define.h"

// ---------------------------------------------------------------------------*
// @brief Fonction d'interruption RTC
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void onRTCAlarm(void) // $ ou £
{
debugSerial.println("\n€ISR€");  // €€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€


// ISR1 doit être activé, sinon vu toutes les n minutes de ISR2
  if (millis() - loopWDT >  100000) // 100s sans passage par loop() => RESET
  {

debugSerial.println(loopWDT);
   loopWDT  = millis();
debugSerial.println(loopWDT);
    
//    softReset();
// Reset immédiat du microcontrôleur, le code ne continue jamais après cette ligne
//trouver autre chose que MORT
  }
  else
    debugSerial.println(loopWDT);
  
  
// ALARME 1 : Toutes les secondes (mode programmation)
  if (rtc.alarmFired(1)) 
  {
    rtc.clearAlarm(1);
    if (!alarm1_enabled) 
      return;     // interruption execution code de IRQ1 pendant IRQ2
    wakeup1Sec = alarm1_enabled;  // True si autorisé par IRQ2
debugSerial.print("\n$1$\n");   // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  }
  
// ALARME 2 : Payload périodique    
  if (rtc.alarmFired(2)) 
  {
    alarm1_enabled = false;  // Bloquer alarme 1
    rtc.clearAlarm(1);       
    rtc.clearAlarm(2); 

    if (!alarm2_enabled) 
    {
      debugSerial.println("onRTCAlarm():alarm2_enabled = False");
      return;     // interruption execution code de IRQ1 pendant IRQ2
    }
    wakeup1Sec = false;    // annule si activée
displayNextPayload = false; //true;
    wakeupPayload = true;
debugSerial.println("OnRTCAlarm/wakeupPayload set to true");
    

    if (config.applicatif.SendingPeriod)     // si 0 pas d'envois par IT
    {
      DS3231setRTCAlarm2(); // Reprogrammer prochaine alarme
    }
debugSerial.println("\n£2£\n");  // £££££££££££££££££££££££££££££££££££££££££££££££££££££££££
  }
}
