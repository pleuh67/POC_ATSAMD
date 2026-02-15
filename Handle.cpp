#define __INIT_DONE
#include "define.h"
//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//      _    _                 _ _                       
//     | |  | |               | | |                      
//     | |__| | __ _ _ __   __| | | ___   ___ _ __  _ __ 
//     |  __  |/ _` | '_ \ / _` | |/ _ \ / __| '_ \| '_ \
//     | |  | | (_| | | | | (_| | |  __/| (__| |_) | |_) |
//     |_|  |_|\__,_|_| |_|\__,_|_|\___(_)___| .__/| .__/
//                                           | |   | |   
//                                           |_|   |_|   
// ---------------------------------------------------------------------------*
//  Fonctions handleOperationMode() et handleProgrammingMode()
//  executées dans loop()
//  
//  selon mode, 
//  
//  Traite en mode non bloquant
//  
//  La navigation dans les menus:
//    Ouvre autre menu
//    Lance une saisie
//    Execute une fonction
//    Affiche un écran
//  
//  Les saisie de Listes, numériques, Alphanumériques, chaines HEXA, date, time, email et IP



#define __SerialDebugPoc      // decommenter pour afficher messages debug

// ---------------------------------------------------------------------------*
// MODE EXPLOITATION
// ---------------------------------------------------------------------------*
// @brief Gestion du mode exploitation
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void handleOperationMode(void) // Mode exploitation : que réveil payload
{ uint8_t rc;
  if (switchToOperationMode)    // affiche qu'une fois
  { 
    SETUPswitchToOperationMode();
  }  
#ifdef __SerialDebugPoc    
//debugSerial.print("E");   // EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
#endif

// ---------------------------------------------------------------------------*
// ISR2
// ---------------------------------------------------------------------------*
  if (wakeupPayload)                          // Envoi LoRa, LED Activité LoRa
  { static int counterPayload=0;   

//GestionEnCours("ISR2a");  // Surveillance pour Debug
    wakeupPayload = false;
debugSerial.println("handleOperationMode/wakeupPayload set to false");
    
    counterPayload++;  // compte le nombre d'envois Payload
//#ifdef __SerialDebugPoc    
sprintf(serialbuf, "I2£%d ", counterPayload);   
debugSerial.println(serialbuf);       // I2£n I2£n I2£n I2£n I2£n I2£n I2£n
//#endif
  turnOnRedLED();     // PCB donne GREEN?
  MESUREStake_All_Measure();
   
  buildLoraPayload();
#ifdef __SendLoRaInOperationMode
debugSerial.println("__SendLoRaInOperationMode DEFINED => RN2483AsendLoRaPayload()");   
  rc = RN2483AsendLoRaPayload((uint8_t*)payload,19);   // hex


char localOLEDbuf[21] = "12345678901234567890";
  snprintf(localOLEDbuf,20, "LoRa : %d", rc);    // 16

  
  OLEDDebugDisplay((char *)RN2483AloRaSendErrorToString(rc));

#endif    
    turnOffRedLED();
debugSerial.println("Fin Payload, Reactive IRQ1");    
    alarm1_enabled = true;   // Réactiver alarme 1 
// GestionEnCours("ISR2b");  // Surveillance pour Debug
  }

// Entrée en veille si activée
  if (DEBUG_LOW_POWER && modeExploitation) // pas de low power en config
  {
debugSerial.println("Low");     
    debugSerial.flush();  
    sleep();
debugSerial.println("low"); 
  }
// Lecture mode de fonctionnement
}


unsigned int dtobin(unsigned char h)
{
  double n;
   unsigned int b=0;
 
   for(n=0;n<=7;n++)
   {
       b+=(pow(10,n)*(h%2));
       h/=2;
   }
 
   return b;
}

// ---------------------------------------------------------------------------*
// @brief affiche le type de traitement en cours de gestion par le handler
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void GestionEnCours(char *from)
{ unsigned char encours=0;
  static unsigned char oldencours=255;
// n'afficher qu'a chaque changement
  
  if (isListInputActive())  
  {
    encours += 1;
    sprintf(serialbuf, "GestionEnCours()\\Menu/list WDT : %d GestionEnCours ",loopWDT);
  }  
  else if (isInfoScreenActive()) 
  {
    encours += 2;
    sprintf(serialbuf, "Info WDT : %d GestionEnCours: ",loopWDT);
  }  
  else if (isNumInputActive())
  {
    encours += 4;
    sprintf(serialbuf, "Number WDT : %d GestionEnCours ",loopWDT);
  }  
  else if (isStringInputActive())
  {
    encours += 8;
    sprintf(serialbuf, "String WDT : %d GestionEnCours ",loopWDT);
  }  
  else if (isTimeInputActive())
  {
    encours += 16;
    sprintf(serialbuf, "Time WDT : %d GestionEnCours ",loopWDT);
  }  
  else if (isHexInputActive())
  {
    encours += 32;
    sprintf(serialbuf, "Hex WDT : %d GestionEnCours ",loopWDT);
  }  
  else
  {
    encours = 0;
    sprintf(serialbuf, "!!!else!!! : %d GestionEnCours ",loopWDT);
  }
  if (encours != oldencours)
  {
    LOG_DEBUG(serialbuf);
    sprintf(serialbuf, "from %s: %d", from, dtobin(encours));
    LOG_DEBUG(serialbuf);                                             // passer sur 1 ligne
    oldencours = encours;
  }  
}


// cas sortie list de selection paramètres (SF, Ruchers, ...)
// Appel des fonction xxDone pour rangement saisies dans destination
// et desactivation de la saisie
void HandleSaisieActive()
{     switch (saisieActive) // 12(pas fait ?), 21-, 22-, 23-, 24-, 32-, 40, 41
        {
          case 12:                                  // m01_2F_GetNumRucher()            pas initialement ???? > KKKKKKKKKKKKKKKKK
          {
            m01_2F_GetNumRucherDone();                            // 
LOG_DEBUG("lancement m01_2F_GetNumRucher");                       
            saisieActive=0;
            break;
          }
          case 21:                                  //  m02_1F_AppKEY()
          {
            m02_1F_AppKEYDone();
LOG_DEBUG("lancement m02_1F_AppKEYDone");                       
            saisieActive=0;
            break;
          }
          case 22:      // m02_1F_AppEUI
          {
            m02_2F_AppEUIDone();
LOG_DEBUG("lancement m02_1F_AppEUIDone");                       
            saisieActive=0;
            break;
          }
          case 23:
          {
             m02_3L_GetSFDone();
LOG_DEBUG("lancement m02_3L_GetSFDone()");                       
            saisieActive=0;
            break;
          }
          case 24:
          {
            m02_4F_GetPayloadDelayDone();
LOG_DEBUG("lancement m02_3L_GetSFDone()");                       
            saisieActive=0;
            break;
          }
          case 30:
          {
            m03_0F_CalibVBatDone();
LOG_DEBUG("lancement m03_2F_CalibVBatDone()");                       
            saisieActive=0;
            break;
          }     
          case 31:
          {
            m03_1F_CalibVSolDone();
LOG_DEBUG("lancement m03_1F_CalibVSolDone()");                       
            saisieActive=0;
            break;
          }     
          case 32:
          {
            m03_2F_CalibVLumDone();
LOG_DEBUG("lancement m03_2F_CalibVLumDone()");                       
            saisieActive=0;
            break;
          }     
          case 40:                 // m04_0F_InfoBalDone())
          {                        // désactive screenRefresh
            m04_0F_InfoBalDone();                            // 
LOG_DEBUG("lancement m04_0F_InfoBalDone()");                       
            saisieActive=0;
            break;
          }
          case 41:                                  // m04_1F_PoidsTare()
          {
            m04_1F_PoidsTareDone();                            // 
LOG_DEBUG("lancement m04_1F_PoidsTareDone()");                       
            saisieActive=0;
            break;
          }
          case 490:                                  // m04x_0F_numBalxDone()
          {
            m04x_0F_numBalxDone();                            // 
LOG_DEBUG("lancement m04x_0F_numBalxDone()");                       
            saisieActive=0;
            break;
          }



          
          default:  // ne rien faire
          {
 LOG_DEBUG("Default dans isListInputActive()/saisieActive/default, pourquoi ???????????????????");           
            break;      
          }
        }
//E24C32DumpConfigToJSON(); 
}


// ---------------------------------------------------------------------------*
// MODE PROGRAMMATION
// ---------------------------------------------------------------------------*
// @brief Gestion du mode programmation avec saisie non-bloquante
// Mode programmation : réveil payload + 1 sec + interface utilisateur
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
 // serial : PL (Liste) ou PN (Numerique) ou PA (Alphanumérique) ou PK (Pas de saisie, attente touche)
void handleProgrammingMode(void) 
{  if (switchToProgrammingMode)    // affiche qu'une fois
  {  
// supprimer ligne si pas revu.
LOG_DEBUG("Passage en mode PROGRAMMATION");

    restartGestionSaisieOLED();

// fin void Restart_Gestion_Saisie_OLED() 
    
  wakeupPayload = false; // désactive envoi généré pendant PROGRAMMATION
LOG_DEBUG("handleProgrammingMode1/wakeupPayload set to false");
  }

#ifdef __SendLoRaInProgrammationMode
  if (wakeupPayload)                          // Envoi LoRa, LED Activité LoRa
  { static int counterPayload=0;   
    uint8_t rc;
    wakeupPayload = false;
//LOG_DEBUG("handleProgrammingMode2/wakeupPayload set to false");
//LOG_DEBUG("GestionEnCours(\"ISR2a\")");   //                                            recherche KKKKK
    counterPayload++;  // compte le nombre d'envois Payload
//#ifdef __SerialDebugPoc    
//sprintf(serialbuf, "I2£%d ", counterPayload);   
//LOG_DEBUG(serialbuf);       // I2£n I2£n I2£n I2£n I2£n I2£n I2£n
//#endif


    // SAUVEGARDER l'état de l'écran avant l'envoi pour eviter KKKKKKKKKKKKKKKKKKK
    infoScreenState_t savedInfoState = infoScreenState;


    turnOnRedLED();     // PCB donne GREEN?
    buildLoraPayload();                                          // pas la cause du KKKKKKKKKKKKKKKKKKKKK
    rc = RN2483AsendLoRaPayload((uint8_t*)payload,19);   // pas la cause du KKKKKKKKKKKKKKKKKKKKK
    turnOffRedLED();
   
    // RESTAURER l'état de l'écran après l'envoi pour eviter KKKKKKKKKKKKKKKKKKK
    infoScreenState = savedInfoState;
    

    
    alarm1_enabled = true;   // Réactiver alarme 1 
#ifdef __SerialDebugPoc  

//LOG_DEBUG("7");   // 777777777777777777777777777777777777

//Apparition de KKKKK

//debugSerial.println("GestionEnCours(\"ISR2a\")");   //                                            recherche KKKKK
//GestionEnCours("FIN ISR2a");  // Surveillance pour Debug
#endif
   }
#endif    

#ifdef __SerialDebugPoc    
 //debugSerial.print("P");   // PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
#endif
// Gestion normale des menus quand pas de saisie
GestionEnCours("handleProgrammingModea");

// ---------------------------------------------------------------------------*
// Vérifier si une sélection de liste est en cours 
// ---------------------------------------------------------------------------*
  if (isListInputActive())
  {
    static uint8_t selectedModeIndex = 0; // Index du mode sélectionné
GestionEnCours("handleProgrammingModeb"); // affiche le type de traitement en cours de gestion par le handler
        
#ifdef __SerialDebugPoc    
//debugSerial.print("L");   // LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
#endif

    listInputState_t state = processListInput();
        
    switch (state)
    {           
      case LIST_INPUT_COMPLETED:
      {

 HandleSaisieActive();


 // cas sortie list de menu        
        menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
char dummy[21] = "";        
        selectedModeIndex = finalizeListInput(dummy); // Récupérer l'index sélectionné

 // Gestion navigation dans les menus       

sprintf(serialbuf, " Mode selectionne: %d - Depth: %d - Val: %s", selectedModeIndex, currentMenuDepth, currentMenu->title);
debugSerial.println(serialbuf);
sprintf(serialbuf, "azerty: %s", currentMenu->menuList[selectedModeIndex]);
debugSerial.println(serialbuf);
//        debugSerial.println(m0_Demarrage[selectedModeIndex]);

        if (currentMenuDepth > 0)
        {
//menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
 // Gestion navigation dans les menus               
debugSerial.print("Mode selectionne: ");
debugSerial.print(selectedModeIndex);
debugSerial.print(" - Depth: ");
debugSerial.print(currentMenuDepth);
debugSerial.print(" - Val: ");
debugSerial.println(currentMenu->title);
// debugSerial.println(m0_Demarrage[selectedModeIndex]);
debugSerial.println(currentMenu->menuList[selectedModeIndex]);

// Ici vous pouvez traiter la sélection
debugSerial.print("Chaine validée : ");
//sprintf(stringSaisie,(char *)m0_Demarrage[selectedModeIndex]);    // recopie saisie dans destination
//debugSerial.println(stringSaisie);
//debugSerial.println(Data_LoRa.RucherName);
// err debugSerial.println([selectedModeIndex]);
debugSerial.println(currentMenu->title);
          OLEDClear();// Effacer écran
          processMenuSelection(selectedModeIndex);
        }
        else
        {
debugSerial.println("Cas ELSE");  
          processMenuSelection(selectedModeIndex);
        }
        break;        
      }
      case LIST_INPUT_CANCELLED:
debugSerial.println("Selection LISTE annulee par timeout");

       // Si on était dans un sous-menu, revenir au menu précédent
        if (currentMenuDepth > 1)
        {
          if (saisieActive) // sortie par timeout d'une saisie de liste
          {
            backMenuFromList(); 
            saisieActive=0; // annulation saisie
          }  
          else
            popMenu();   // sortie navigation MENU
        }
        else
        {
          // Sinon réinitialiser complètement
          currentMenuDepth = 0;
        }
        break;
      default:
                // Sélection toujours en cours, ne rien faire d'autre
              return;
    }
  }
// ---------------------------------------------------------------------------*
// Vérifier si un écran d'info est actif               
// ---------------------------------------------------------------------------*
 else if (isInfoScreenActive())
  { 
    GestionEnCours("handleProgrammingModec"); // affiche le type de traitement en cours de gestion par le handler
    infoScreenState_t state = processInfoScreen();
// debugSerial.print("S");   // SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS    

    switch (state)
    {
      case INFO_SCREEN_CLOSED:
      {
        debugSerial.println("Ecran info ferme");
        infoScreenState = INFO_SCREEN_IDLE; // Reset
        OLEDDisabelAllRefresh();    // RAZ brutale de tous les indicateurs de refresh
        break;
      }  
      default:
        // Écran toujours actif, ne rien faire d'autre
        return;
    }
  }  
// ---------------------------------------------------------------------------*
// Vérifier si une saisie numérique est en cours
// ---------------------------------------------------------------------------*
  // Vérifier si une saisie numérique est en cours
  else if (isNumInputActive())
  {
    static char numBuffer[21] = "1234";

GestionEnCours("handleProgrammingModed");   // affiche le type de traitement en cours de gestion par le handler
      
#ifdef __SerialDebugPoc    
 // debugSerial.print("N");   // NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
#endif
    numInputState_t state = processNumInput();
    switch (state)
    {
      case NUM_INPUT_COMPLETED:

HandleSaisieActive();
      
  //      finalizeNumInput(numBuffer);
  //      debugSerial.print("Nouveau nombre: ");
   //     debugSerial.println(numBuffer);
        // Ici vous pouvez traiter le nombre
/*
debugSerial.println("Chaine validée : ");
sprintf(stringSaisie,(char *)numBuffer);    // recopie saisie dans destination !!!!!  attentin LISTE
debugSerial.println(stringSaisie);
debugSerial.println(Data_LoRa.RucherName);
*/

/*
ICI traiter SaisieActive des NUM





        switch (saisieActive)
        {
          case 12:
          {
            m01_2F_GetNumRucherDone(); // retour au menu        
          }
          case 23:
          {
            m02_4F_GetPayloadDelayDone(); // retour au menu        
          }


          default:
            break; 
        }

*/
       
// peut être on peut enlever break??? pour executer le valid suivi du TMT 
// en veillant a ne pas annuler laa saisie validée ;-)
// ceci peut éviter de doubler les tests 
        break;        
      case NUM_INPUT_CANCELLED:

//HandleSaisieCanceled()   // stoppe les rafraichissement données OLED et debugSerial
  InfoVLumScreenRefresh = false;           // desactive rafraichissement de Vlum

      
        debugSerial.println("Saisie numerique annulee par timeout");
        cancelNumInput();

m01_2F_GetNumRucherDone(); // retour au menu
        
        break;
        
      default:
        return;
    }
  }


  
// ---------------------------------------------------------------------------*
// Vérifier si une saisie alphanumérique est en cours
// ---------------------------------------------------------------------------*
  // Vérifier si une saisie alphanumérique est en cours
  else if (isStringInputActive())
  {
    static char stringBuffer[21] = "                    ";

GestionEnCours("handleProgrammingModee");   // affiche le type de traitement en cours de gestion par le handler
        
#ifdef __SerialDebugPoc    
//debugSerial.print("A");   // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
#endif    

    
    stringInputState_t state = processStringInput();
    
    switch (state)
    {
      case STRING_INPUT_COMPLETED:

HandleSaisieActive();
      
          m01_3F_GetNameRucherDone();           // SaisieActive??????????????????
/*        finalizeStringInput(stringBuffer);
        debugSerial.print("Nouvelle chaine: ");
        debugSerial.println(stringBuffer);
        // Ici vous pouvez traiter la chaîne
*/
        break;
        
      case STRING_INPUT_CANCELLED:
        debugSerial.println("Saisie alphanumérique annulee");
        cancelStringInput();
        backMenu(); 
        break;
        
      default:  // Saisie toujours en cours, ne rien faire d'autre
        return;
    }
  }
// ---------------------------------------------------------------------------*
// Vérifier si une saisie HEXA est en cours
// ---------------------------------------------------------------------------*
  else if (isHexInputActive())
  {
    static char hexBuffer[41] = "0123456789ABCDEF0123456789ABCDEF01234567"; // Buffer pour l'hexa
GestionEnCours("handleProgrammingModef");   // affiche le type de traitement en cours de gestion par le handler
#ifdef __SerialDebugPoc    
//debugSerial.print("H");   // HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
#endif    
    
    hexInputState_t state = processHexInput();
    switch (state)
    {
      case HEX_INPUT_COMPLETED:
      {

HandleSaisieActive();

        
    //    finalizeHexInput(hexBuffer); // Récupérer la chaîne finale
      //  debugSerial.print("Nouvelle cle hexadecimale: ");
        //debugSerial.println(hexBuffer);
        // Ici vous pouvez sauvegarder la clé ou autre


       
       break;
      }   
      case HEX_INPUT_CANCELLED:
      {
debugSerial.println("Saisie hexadecimale annulee par timeout");
        cancelHexInput();
backMenu();
        break;
      }         
      default:  // Saisie toujours en cours, ne rien faire d'autre
        return;
    }
  }
// ---------------------------------------------------------------------------*
// Vérifier si une saisie d'heure est en cours
// ---------------------------------------------------------------------------*
  else if (isTimeInputActive())
  {
//    static char timeBuffer[9] = ""; // Buffer pour l'heure

GestionEnCours("handleProgrammingModeg");   // affiche le type de traitement en cours de gestion par le handler
#ifdef __SerialDebugPoc    
//debugSerial.print("T");   // TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
#endif    
    timeInputState_t state = processTimeInput();
    switch (state)
    {
      case TIME_INPUT_COMPLETED:
        HandleSaisieActive();      
m01_1F_GetTimeDone();                                                                                     // affecter un saisie active : 11
        break;   
          
      case TIME_INPUT_CANCELLED:
      {
debugSerial.println("Saisie heure annulee par timeout");
        cancelTimeInput(); // Retour au menu dans fonction             
backMenu();
        break;   
      }   
      default: // Saisie toujours en cours, ne rien faire d'autre
        return;
    }
  }
// ---------------------------------------------------------------------------*
// Vérifier si une saisie de date est en cours
// ---------------------------------------------------------------------------*
  else if (isDateInputActive())
  {
    static char dateBuffer[11] = ""; // Buffer pour la date

GestionEnCours("handleProgrammingModeh");   // affiche le type de traitement en cours de gestion par le handler
#ifdef __SerialDebugPoc    
//debugSerial.print("D");   // DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
#endif    
    dateInputState_t state = processDateInput();
    
    switch (state)
    {
      case DATE_INPUT_COMPLETED:
      {

        HandleSaisieActive();

m01_0F_GetDateDone();                                                                                                // affecter un saisie active : 10
         break;
      }  
      case DATE_INPUT_CANCELLED:
      {
debugSerial.println("Saisie date annulee par timeout");
        cancelDateInput(); // Retour au menu dans fonction
backMenu();
        break;
      }  
      default:  // Saisie toujours en cours, ne rien faire d'autre
        return;
    }
  }
// ---------------------------------------------------------------------------*
// Vérifier si une saisie Email est en cours
// ---------------------------------------------------------------------------*
  else if (isEmailInputActive())
  {
    static char emailBuffer[41] = "user@example.com";

GestionEnCours("handleProgrammingModei");   // affiche le type de traitement en cours de gestion par le handler
#ifdef __SerialDebugPoc    
debugSerial.print("E");   // EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
#endif    
 
    emailInputState_t state = processEmailInput();
    
    switch (state)
    {
      case EMAIL_INPUT_COMPLETED:
      {

        HandleSaisieActive();
        
        finalizeEmailInput(emailBuffer);
        debugSerial.print("Nouvel email: ");
        debugSerial.println(emailBuffer);
        // Ici vous pouvez sauvegarder l'email
        break;
      }  
      case EMAIL_INPUT_CANCELLED:
      {
        debugSerial.println("Saisie email annulee par timeout");
        cancelEmailInput();
backMenu();
        break;
      }  
      default:
        return;
    }
  }
// ---------------------------------------------------------------------------*
// Vérifier si une saisie IP est en cours
// ---------------------------------------------------------------------------*
  else if (isIPInputActive())
  {
    static char ipBuffer[16] = "192.168.001.001";

GestionEnCours("handleProgrammingModej");   // affiche le type de traitement en cours de gestion par le handler
#ifdef __SerialDebugPoc    
debugSerial.print("I");   // IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
#endif    
 
    
    ipInputState_t state = processIPInput();
    
    switch (state)
    {
      case IP_INPUT_COMPLETED:
        HandleSaisieActive();

        finalizeIPInput(ipBuffer);
        debugSerial.print("Nouvelle IP: ");
        debugSerial.println(ipBuffer);
        // Ici vous pouvez sauvegarder l'IP
        break;
        
      case IP_INPUT_CANCELLED:
        debugSerial.println("Saisie IP annulee par timeout");
        cancelIPInput();
backMenu();
        break;
        
      default:
        return;
    }
  }
// ---------------------------------------------------------------------------*
// Gestion normale des menus quand pas de saisie, dans quels cas????, pas prévu hors saisies par fonctions
// ---------------------------------------------------------------------------*
  else
  {

// ne devrait pas y arriver???

GestionEnCours("handleProgrammingModek");     // affiche le type de traitement en cours de gestion par le handler

LOG_ERROR("Perte de la Gestion courante dans Handle.cpp (Saisies_NB)");

startupListActivated = false;
restartGestionSaisieOLED();    // solution de sortie d'urgence à valider
GestionEnCours("handleProgrammingModea");


    if (touche != KEY_NONE)
    {
      switch (touche)
      {
        case KEY_1: // Touche 1 - Fonction libre
        {
          debugSerial.println("Touche 1 - Fonction libre");
          break;
        }
        case KEY_2: // Touche 2 - Test sélection dans une liste
        {
//                        static uint8_t currentModeIndex = 0; // Index actuel (peut être sauvegardé)
//                        startListInput("CHOIX MODE:", menu000Demarrage, 9, currentModeIndex);
// Si on était dans un sous-menu, revenir au menu précédent
          if (currentMenuDepth > 1)
          {
            popMenu();
          }
          else
          {
          // Sinon réinitialiser complètement
            currentMenuDepth = 0;
          }
        break;
        }
        case KEY_3: // Touche 3 - Test saisie numérique
        {
  static char currentNumber[11] = "12345";
          startNumInput("Saisie NUMERIQUE",currentNumber, 6, false, false, 0, 999999);

          break;
        }            
        case KEY_4: // Touche 4 - Test saisie alphanumérique
        {
          static char currentString[21] = "MBVSB0CBJTPOT0WJUF";
// appeler avec chaine a modifier    ex: Data_LoRa.RucherName[20]
//      startStringInput("SAISIE TEXTE:", currentString, 20); 

          stringSaisie=config.applicatif.RucherName;  // pointeur sur valeur courante traiter
// attention si existe "CANCEL", proceder par variable tampon
          startStringInput("SAISIE TEXTE:", stringSaisie, 20);
          break;
        }            
        case KEY_5: // Touche 5 - Reset soft
          debugSerial.println("Touche 5 - Reset soft");
          break;
        default:
          break;
      }
      touche = KEY_NONE; // Reset de la touche
    }
  }
}


// repositionner ?????

// ---------------------------------------------------------------------------*
// @brief Traite l'écran d'informations, appelé dans loop\handleProgrammingMode
// @param void
// @return État actuel de l'écran
// ---------------------------------------------------------------------------*
infoScreenState_t processInfoScreen(void)
{
  if (infoScreenState != INFO_SCREEN_ACTIVE)
  {
    return infoScreenState;
  }
  
  // Traitement de la touche VALIDE pour revenir
  if (touche == VALIDE)
  {
debugSerial.println("processInfoScreen()\VALIDE");
    touche = KEY_NONE;
    infoScreenState = INFO_SCREEN_CLOSED;
    // Revenir au menu d'appel de l'écran d'info
    if (currentMenuDepth > 0)
    {
      menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
      startListInput(currentMenu->title, currentMenu->menuList, currentMenu->menuSize, currentMenu->selectedIndex, 0);
      debugSerial.println("Retour du menu infos");
    }
    else
    {
      debugSerial.println("ERREUR: currentMenuDepth = 0 dans processInfoScreen");
    }
  }
  return infoScreenState;
}

// ---------------------------------------------------------------------------*
// @brief Vérifie si un écran d'info est actif
// @param void
// @return true si écran actif
// ---------------------------------------------------------------------------*
bool isInfoScreenActive(void)
{
  return (infoScreenState == INFO_SCREEN_ACTIVE);
}

// ---------------------------------------------------------------------------*
// @brief Entrée dans mode Programmation
// @param void
// @return void
// ---------------------------------------------------------------------------*
void restartGestionSaisieOLED(void)
{
// préciser le statut des menus, retour au PRINCIPAL
  switchToOperationMode = true;   // 
  switchToProgrammingMode = false;
  OLEDClear();
// Réinit structure à Default
// listInputContext_t listInputCtx = {LIST_INPUT_IDLE, 0, 0, 10, 0, 0, false, false, 0, false, 0, 0, 0, "", NULL};
  listInputCtx.state = LIST_INPUT_IDLE;
  listInputCtx.selectedIndex = 0;
  listInputCtx.scrollOffset = 0;
  listInputCtx.maxItems = 5;
  listInputCtx.lastScrollOffset = 0;
  listInputCtx.lastSelectedIndex = 0;
  listInputCtx.lastCursorBlink = false;                    
  listInputCtx.displayRefresh = false;
  listInputCtx.lastUpdate = 0;
  listInputCtx.cursorBlink = false;
  listInputCtx.lastActivity = 0;
  listInputCtx.lastActivity = 0;
  listInputCtx.timeoutDuration = 0;                   
  strncpy(listInputCtx.title,"",21);
  listInputCtx.itemList = NULL;                    

  OLEDDisabelAllRefresh();    // Reset de tous les rafraichissement OLED

// remise Niveau menus à 0
  currentMenuDepth = 0;  // départ menu PRINCIPAL
 // Activer la liste au démarrage si pas encore fait
  if (!startupListActivated)
  {
debugSerial.println("Lancement Menu principal");
    initStartupList();  // Initialise l'affichage de la liste au démarrage
  }
}
