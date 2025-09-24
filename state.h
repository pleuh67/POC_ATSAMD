/*


1 false; 2 true; LP false
IRQ2 sans build et send pendant saisie = PB
loop nok, IRQ continue
suppression wakeupPayload = true  => idem remis
suppression:
//debugSerialPrintNextAlarm(nextPayload,2);
//OLEDDrawScreenNextPayload(7, 0, nextPayload );  // Status message

Géré dans main():
debugSerialPrintNextAlarm(nextPayload,2);


pas de config IRQ1
void DS3231setRTCAlarm1(void) 
{
  return; 






significations dans serialDebug:

// handle : 
E : mode Exploitation
P : Mode Programmation
PL (Liste) ou PN (Numerique) ou PA (Alphanumérique) ou PK (Pas de saisie, attente touche)






11/09/2025: pas de build et send lora payload à tester



 
void alarm1ISR() {
    if (!alarm1_enabled) return;
    
    // Traitement alarme 1
    rtc.alarmFired(1);  // Efface le flag
}

void alarm2ISR() {
    alarm1_enabled = false;  // Bloquer alarme 1
    
    // Traitement alarme 2 (prioritaire)
    rtc.alarmFired(2);
    
    // Votre code spécifique IRQ2...
    
    alarm1_enabled = true;   // Réactiver alarme 1
}

  rtc.writeSqwPinMode(DS3231_OFF); bloque les interrupts

 */
