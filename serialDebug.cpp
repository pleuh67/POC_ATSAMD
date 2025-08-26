
#define __INIT_DONE
#include "define.h"



/**
 * @brief Affiche un nombre avec 2 chiffres minimum
 * @param number Nombre à afficher
 * @return void
 */
void print2digits(int number) 
{
  if (number >= 0 && number < 10) 
  {
      debugSerial.write('0');
  }
  debugSerial.print(number);
}



/**
 * @brief Construit et affiche le message HEXA sur le port série de debug
 * @param payload Pointeur vers les données
 * @param len Longueur des données
 * @return void
 */
void debugSerialPrintLoraPayload(uint8_t *payload, uint8_t len)
{
    int i;
    for (i = 0; i < len; i++) 
    {
        sprintf(&hexPayload[i * 2], "%02X", payload[i]);  
    }
    hexPayload[hexPayloadSize - 1] = '\0';
    
    debugSerial.print("hexPayload: "); 
    debugSerial.print(hexPayload);
    debugSerial.print(" len : "); 
    debugSerial.println(len);
}

/**
 * @brief Affiche des données au format hexadécimal sur le port série
 * @param txt Pointeur vers les données
 * @param len Longueur des données
 * @return void
 */
void printOndebugSerial(char *txt, char len)
{ int i;
  char buf[256];
    
    if (COM_DebugSerial)
    {
        memset(buf, 0, 256);
        for (i = 0; i < len; i++)  
        { 
            buf[2*i] = ((txt[i]>>4) < 10) ? (txt[i]>>4) + 48 : (txt[i]>>4) + 55;
            buf[2*i+1] = ((txt[i]&0x0F) < 10) ? (txt[i]&0x0F) + 48 : (txt[i]&0x0F) + 55;
        }
        debugSerial.print(" ");
        buf[2*i] = 0;
        debugSerial.println(buf);
    }
}



void debugSerialPrintNextAlarm(DateTime nextPayload, int IRQ)  
{
  sprintf(serialbuf,"Interruption Alarme %d activée, reprogrammée pour: ",IRQ );
  debugSerial.print(serialbuf); 
  sprintf(serialbuf,"%02d:%02d:%02d",nextPayload.hour(),nextPayload.minute(),nextPayload.second());
  debugSerial.println(serialbuf); 
}


void debugSerialPrintLoRaStatus()  
{
  debugSerial.println("=== STATUS LoRa ===");
  sprintf(serialbuf," => RN2483#%02d",Ruche.Num_Carte);        // affiche N° de carte
  debugSerial.println(serialbuf);
  debugSerial.print("DevEUI = ");printOndebugSerial((char *)DevEUI,8);
  debugSerial.print("AppEUI = ");printOndebugSerial((char *)AppEUI,8);
  debugSerial.print("AppKey = ");printOndebugSerial((char *)AppKey,16); 
  debugSerial.println("=====================");
}  

/**
 * @brief Fonction de test du clavier analogique
 * @param Aucun
 * @return void
 */
void debugSerialPrintKbdKey(void) 
{ key_code_t key = readKey();
    
  switch (key) 
  {
    case KEY_NONE:
        break;
          
    case KEY_1:
        debugSerial.println("Touche 1 pressée");
        break;
            
    case KEY_2:
        debugSerial.println("Touche 2 pressée");
        break;
            
    case KEY_3:
        debugSerial.println("Touche 3 pressée");
        break;
            
    case KEY_4:
        debugSerial.println("Touche 4 pressée");
        break;
            
    case KEY_5:
        debugSerial.println("Touche 5 pressée");
        break;
           
    case KEY_INVALID:
        debugSerial.print("Erreur de lecture clavier - valeur: ");
        debugSerial.println(analogRead(KBD_ANA));
        break;
            
    default:
        debugSerial.println("Code de touche inconnu");
        break;
  }
  delay(100);
}


/**
 * @brief Affiche les informations système sur debugSerial
 * @param Aucun
 * @return void
 */
void serialDebugPrintSystemInfo(void)
{
    debugSerial.println("=== INFORMATIONS SYSTEME ===");
    debugSerial.println("Projet: " PROJECT_NAME);
    debugSerial.println("Version: " VERSION);
    debugSerial.print("Mode actuel: ");
    debugSerial.println(modeExploitation ? "EXPLOITATION" : "PROGRAMMATION");
    debugSerial.print("Configuration version: ");
    debugSerial.print(config.materiel.version/100);
    debugSerial.print(".");
    debugSerial.println(config.materiel.version%100);
    debugSerial.print("Compilation: ");
    debugSerial.print(__DATE__);
    debugSerial.print(" ");
    debugSerial.println(__TIME__);
 /*   
    // Attendre une touche pour continuer
    while (readKey() == KEY_NONE)
    {
        delay(100);
    }
    
    OLEDDisplayMessageL8("Retour menu principal", false, false);
*/   
}
