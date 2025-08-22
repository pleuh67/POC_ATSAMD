/*

plante pendant envoi payload, seul IRQ2 actif.



08:37:06.624 -> PROG/Réveil payload
08:37:07.792 -> Hardreset the RN module LoRa      => Reset_LoRa(void)
08:37:08.867 -> hexPayload: 52657374617274 len : 7
08:37:08.867 -> appel LoRaBee.send                => blocage

suppression de Reset_LoRa(); dans Send_LoRa_Mess(...);

suppression de processContinuousKeyboard();

idem, 

ajout :   initLoRa();
  Send_LoRa_Mess((uint8_t*)testPayload,7);
ajout :
    init2483A();


suppression lora_sleep oublié....    


ajout #include <Sodaq_wdt.h> et
    sodaq_wdt_safe_delay(5); // without this, it doesn’t sleep.. don’t know why de sleep_lora()




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








 */
