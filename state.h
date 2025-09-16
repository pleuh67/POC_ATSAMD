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
 * 
 * affiche en taille 2 pendat envoi : 
 * SENDIND
 * PAYLOAD
 * 
15:38:15.938 -> 712346
15:38:15.938 -> 712346
15:38:15.938 -> 7$12341$56
15:38:16.269 -> 712346
15:38:16.269 -> 712346
15:38:16.269 -> 712346
...
15:38:16.920 -> 712346
15:38:16.920 -> 712346
15:38:16.920 -> 712346
15:38:16.920 -> 712346
15:38:16.920 -> 712346
15:38:16.920 -> 712346
15:38:16.920 -> 71234$£Interruption Alarme 2 activée, reprogrammée pour: 15:33:00
15:38:16.969 -> 
15:38:16.969 -> 2£1 
15:38:16.969 -> final hexpayload[i]38
15:38:16.969 -> hexPayload: 00AC266300000000000000000000000000000
15:38:17.017 -> hexPayload: 3030333033333330333333333333333033333 len : 19
15:38:17.017 -> appel LoRaBee.send
15:38:17.951 -> $
15:38:18.417 -> The device is not connected to the network in Send_LoRa_Mess(). The program will reset the RN module...$$$$$$$$=== STATUS LoRa ===
15:38:26.525 ->  => RN2483#04
15:38:26.525 -> DevEUI =  0004A30B00EEA5D5
15:38:26.525 -> AppEUI =  414245494C4C4534
15:38:26.525 -> AppKey =  5048494C495050454C4F56454C414B4F
15:38:26.525 -> =====================
15:38:26.525 -> setupLoRaOTAA(), Network connection failed!
15:38:26.949 -> $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$










  rtc.writeSqwPinMode(DS3231_OFF); bloque les interrupts


Payload: 19 Octets

// N° de rucher  
  payload[0] 
// temperature DHT22
  payload[1]  // Temperature: Least significant byte first, little endian
  payload[2]  
// humidity  DHT22
  payload[3]
  payload[4] 
// Brightness (LDR)
  payload[5] 
  payload[6]
// tension moyenne des 10 dernières lectures (VBat)   => ne calculer la moyenne que lors de l'envoie/Affichage, pas à chaque lecture.
  payload[7] 
  payload[8]
// tension moyenne des 10 dernières lectures (VSol)   => Idem
  payload[9]
  payload[10]
// Balance 1
  payload[11]
  payload[12] 
// Balance 2
  payload[13] 
  payload[14]
// Balance 3
  payload[15]
  payload[16]
// Balance 4
  payload[17] 
  payload[18]


  // Reading temperature or humidity takes about 250 milliseconds!




i2c ASSYCHRONE 


degraisser : 

16:45:37.527 -> IRQ1 1s 270 =====
16:45:38.556 -> IRQ1 1s 271 =====
16:45:39.537 -> IRQ1 1s 272 =====
16:45:40.524 -> IRQ1 1s 273 =====
16:45:41.562 -> === CONFIGURATION ALARMES RTC + INTERRUPTIONS ===
16:45:41.562 -> s
16:45:41.562 -> Interruption Alarme 2 activée, reprogrammée pour: 16:43:00
16:45:41.562 -> État pin RTC après config alarmes: HIGH
16:45:41.562 -> === FIN CONFIGURATION ALARMES + INTERRUPTIONS ===
16:45:41.562 -> IRQ2 15m 0 =====
16:45:41.562 -> PROG/Réveil payload
16:45:41.562 -> hexPayload: 52657374617274 len : 7
16:45:41.562 -> appel LoRaBee.send
16:45:49.365 -> NOError Successful transmission in Send_LoRa_Mess().
16:45:49.414 -> IRQ1 1s 274 =====
16:45:49.554 -> IRQ1 1s 275 =====
16:45:50.537 -> IRQ1 1s 276 =====
16:45:51.564 -> IRQ1 1s 277 =====

compteur sur OLED (une ligne par IRQ)










virer

08:30:35.815 -> Configuration interruption sur pin 2
08:30:35.815 -> État initial pin RTC: HIGH
08:30:35.815 -> Interruption RTC attachée
08:30:36.798 -> État pin après config: HIGH
08:30:36.798 -> Initialisation terminee

08:30:36.798 -> === SYNCHRONISATION FORCÉE ===
08:30:36.798 -> === COPIE DS3231 -> MICRO ===
08:30:36.798 -> Différence DS3231-Système: 0 secondes
08:30:36.798 -> Synchronisation nécessaire...
08:30:36.798 -> Avant - DS3231: 8:29:59 | Système: 8:29:59
08:30:36.798 -> Heure copiée du DS3231 vers micro


08:30:37.970 -> === COMPARAISON DES HORLOGES ===
08:30:37.970 -> Système: 22/8/2025 8:30:0
08:30:37.970 -> RTC:     22/8/2025 8:30:0
08:30:37.970 -> Différence: 0 secondes
08:30:37.970 -> ===============================
08:30:37.970 -> Mise à l'heure

envoi en boucle Payload en exploitation






 */
