#define __INIT_DONE
#include "define.h"

/**
 * @brief Fonction d'interruption RTC
 * @param Aucun
 * @return void
 */
void onRTCAlarm(void) // $ ou £
{
//debugSerial.println("\n€");  // €€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€
  
// ALARME 1 : Toutes les secondes (mode programmation)
  if (rtc.alarmFired(1)) 
  {
    rtc.clearAlarm(1);
    if (!alarm1_enabled) 
      return;     // interruption execution code de IRQ1 pendant IRQ2
    wakeup1Sec = alarm1_enabled;  // True si autorisé par IRQ2
debugSerial.print("\n$irq\n");   // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  }
  
// ALARME 2 : Payload périodique    
  if (rtc.alarmFired(2)) 
  {
    wakeup1Sec = false;    // annule si activée
displayNextPayload = true;
    alarm1_enabled = false;  // Bloquer alarme 1
    wakeupPayload = true;
    rtc.clearAlarm(1);       
    rtc.clearAlarm(2);      
    DS3231setRTCAlarm2(); // Reprogrammer prochaine alarme
debugSerial.println("\n£");  // £££££££££££££££££££££££££££££££££££££££££££££££££££££££££
  }
}
