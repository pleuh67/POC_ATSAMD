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
{ 
  if (switchToOperationMode)    // affiche qu'une fois
  { 
    switchToProgrammingMode = true;
    OLEDClear(); 
    OLEDDrawText(1, 7, 0, "MODE EXPLOITATION");
    switchToOperationMode = false;
  }  
#ifdef __SerialDebugPoc    
debugSerial.print("E");   // EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
#endif
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
// puis printf("%d",dtobin(ton_nombre_allant_de_0_à_255));


// ---------------------------------------------------------------------------*
// @brief affiche le type de traitement en cours de gestion par le handler
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void GestionEnCours(char *from)
{ unsigned char encours=0;
  static unsigned char oldencours=255;
 // return;
// n'afficher qu'a chaque changement





  
  if (isListInputActive())  
  {
    encours += 1;
    sprintf(serialbuf, "Menu/list WDT : %d GestionEnCours ",loopWDT);
  }  
  else if (isInfoScreenActive()) 
  {
    sprintf(serialbuf, "Info WDT : %d GestionEnCours: ",loopWDT);
    encours += 2;
  }  
  else if (isNumInputActive())
  {
    sprintf(serialbuf, "Number WDT : %d GestionEnCours ",encours,loopWDT);
    encours += 4;
  }  
  else if (isStringInputActive())
  {
    sprintf(serialbuf, "String WDT : %d GestionEnCours ",encours,loopWDT);
    encours += 8;
  }  
  else if (isTimeInputActive())
  {
    sprintf(serialbuf, "Time WDT : %d GestionEnCours ",encours,loopWDT);
    encours += 16;
  }  
  else if (isHexInputActive())
  {
  sprintf(serialbuf, "Hex WDT : %d GestionEnCours ",encours,loopWDT);
    encours += 32;
  }  
  else
    encours = 0;

if (encours != oldencours)
{
  debugSerial.print(serialbuf);
  sprintf(serialbuf, "from %s: %d", from, dtobin(encours));
  debugSerial.println(serialbuf);
  oldencours = encours;
}  
//  debugSerial.println(encours);
//  
//  sprintf(serialbuf, "List %d / Infos %d / Number %d / String %d / Time %d / Date, Mail, IP,...",
//  isListInputActive(), isInfoScreenActive(), isNumberInputActive(), isStringInputActive(), isTimeInputActive()); 
//  debugSerial.println(serialbuf);




}


// ---------------------------------------------------------------------------*
// MODE PROGRAMMATION
// ---------------------------------------------------------------------------*
// @brief Gestion du mode programmation avec saisie non-bloquante
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
 // serial : PL (Liste) ou PN (Numerique) ou PA (Alphanumérique) ou PK (Pas de saisie, attente touche)
void handleProgrammingMode(void) 
{    // Mode programmation : réveil payload + 1 sec + interface utilisateur
  if (switchToProgrammingMode)    // affiche qu'une fois
  {  
    switchToOperationMode = true;
    OLEDClear();
    switchToProgrammingMode = false;
// Activer la liste au démarrage si pas encore fait
    if (!startupListActivated)
    {
// afficher structure gestion menu avant 1er menu
debugSerial.println("Lancement Menu principal");
      initStartupList();
    }
  }
/*  
// Traite l'écran d'informations (à appeler dans loop)
infoScreenState_t processInfoScreen();
*/
   
#ifdef __SerialDebugPoc    
// debugSerial.print("P");   // PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
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
        menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
        
        selectedModeIndex = finalizeListInput(); // Récupérer l'index sélectionné

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
// err debugSerial.println(m0_Demarrage[selectedModeIndex]);
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
                debugSerial.println("Selection mode annulee par timeout");
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
                
      default:
                // Sélection toujours en cours, ne rien faire d'autre
              return;
    }
  }
// ---------------------------------------------------------------------------*
// Vérifier si un écran d'info est actif               
// ---------------------------------------------------------------------------*
 #define REFRESHSCREEN 1000
 else if (isInfoScreenActive())
  { static unsigned long refreshScreen=0;
    if (millis() - refreshScreen > REFRESHSCREEN) // Refresh heure et datas toutes les 1s
    {
      refreshScreen = millis();
// selectionner l'écran cible
if (PageInfosLoRaRefresh)             // rien à rafraichir
{
 
}
else if (PageInfosSystRefresh)        // rafraichir Heure
{
  OLEDDrawScreenRefreshTime(1, 0);    // refresh Time/Date every second 
}
/*
else if (PageInfosBalRefresh)   // rafraichir les poids temp, hum
{

}
*/
    } 

GestionEnCours("handleProgrammingModec"); // affiche le type de traitement en cours de gestion par le handler
    infoScreenState_t state = processInfoScreen();
// debugSerial.print("S");   // SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS    

    
    
    
    
    switch (state)
    {
      case INFO_SCREEN_CLOSED:
      {
        debugSerial.println("Ecran info ferme");
        infoScreenState = INFO_SCREEN_IDLE; // Reset

////listInputCtx.state = LIST_INPUT_ACTIVE;
////infoScreenRefreshTime = false;
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
//  debugSerial.print("N");   // NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
#endif
    numInputState_t state = processNumInput();
    switch (state)
    {
      case NUM_INPUT_COMPLETED:
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
        m01_2F_GetNumRucherDone(); // retour au menu
        break;        
      case NUM_INPUT_CANCELLED:
        debugSerial.println("Saisie numerique annulee par timeout");
        cancelNumInput();

m01_2F_GetNumRucherDone(); // retour au menu
        
        break;
        
      default:
        return;
    }
  }

/*  
// ci dessous caduque  
  else if (isNumberInputActive())   // numberInputCtx.state == NUMBER_INPUT_ACTIVE
  {
    static char numberBuffer[11] = ""; // Buffer pour le nombre

GestionEnCours("handleProgrammingModed");   // affiche le type de traitement en cours de gestion par le handler
      
#ifdef __SerialDebugPoc    
  debugSerial.print("N");   // NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
#endif

    numberInputState_t state = processNumberInput();
      
    switch (state)
    {
      case NUMBER_INPUT_COMPLETED:
          finalizeNumberInput(numberBuffer); // Récupérer le nombre final
          debugSerial.print("Nouveau nombre: ");
          debugSerial.println(numberBuffer);
          // Ici vous pouvez traiter le nombre

debugSerial.println("Chaine validée : ");
sprintf(stringSaisie,(char *)numberBuffer);    // recopie saisie dans destination !!!!!  attentin LISTE
debugSerial.println(stringSaisie);
debugSerial.println(Data_LoRa.RucherName);

                OLEDClear();// Effacer écran
//                OLEDDrawScreenTime(0, 0); // Affiche Time/Date au complet    

          break;
          
      case NUMBER_INPUT_CANCELLED:
          debugSerial.println("Saisie numerique annulee");
          break;
          
      default:
          // Saisie toujours en cours, ne rien faire d'autre
          return;
    }
  }
*/
  
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
          m01_3F_GetNameRucherDone();
/*        finalizeStringInput(stringBuffer);
        debugSerial.print("Nouvelle chaine: ");
        debugSerial.println(stringBuffer);
        // Ici vous pouvez traiter la chaîne
*/
        break;
        
      case STRING_INPUT_CANCELLED:
        debugSerial.println("Saisie alphanumérique annulee");
        backMenu(); 
        cancelStringInput();
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
debugSerial.print("H");   // HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
#endif    
    
    hexInputState_t state = processHexInput();
    switch (state)
    {
      case HEX_INPUT_COMPLETED:
      {
        m02_1F_GetHexDone();
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
        if (currentMenuDepth > 0)           // Revenir au menu
        {
          menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
          startListInputWithTimeout(currentMenu->title, currentMenu->menuList, currentMenu->menuSize, currentMenu->selectedIndex, 0);
        }
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
      {
        m01_1F_GetTimeDone(); // Retour au menu dans fonction
        break;   
      }    
      case TIME_INPUT_CANCELLED:
      {
debugSerial.println("Saisie heure annulee par timeout");
        cancelTimeInput(); // Retour au menu dans fonction             
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
         m01_0F_GetDateDone(); // Retour au menu dans fonction
         break;
      }  
      case DATE_INPUT_CANCELLED:
      {
debugSerial.println("Saisie date annulee par timeout");
        cancelDateInput(); // Retour au menu dans fonction
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
        finalizeIPInput(ipBuffer);
        debugSerial.print("Nouvelle IP: ");
        debugSerial.println(ipBuffer);
        // Ici vous pouvez sauvegarder l'IP
        break;
        
      case IP_INPUT_CANCELLED:
        debugSerial.println("Saisie IP annulee par timeout");
        cancelIPInput();
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
GestionEnCours("handleProgrammingModek");     // affiche le type de traitement en cours de gestion par le handler
#ifdef __SerialDebugPoc    
debugSerial.print("K");   // KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
#endif    
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

          stringSaisie=Data_LoRa.RucherName;  // pointeur sur valeur courante traiter
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
// @brief Traite l'écran d'informations (à appeler dans loop)
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
    touche = KEY_NONE;
    infoScreenState = INFO_SCREEN_CLOSED;
    
    // Revenir au menu principal
    if (currentMenuDepth > 0)
    {
      menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
      startListInputWithTimeout(currentMenu->title, currentMenu->menuList, currentMenu->menuSize, currentMenu->selectedIndex, 0);
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
