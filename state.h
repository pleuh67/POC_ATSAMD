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












 */
