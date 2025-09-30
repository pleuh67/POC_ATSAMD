#define __INIT_DONE
#include "define.h"


#define __SerialDebugPoc      // decommenter pour afficher messages debug

// ---------------------------------------------------------------------------
// MODE EXPLOITATION
// ---------------------------------------------------------------------------
/**
 * @brief Gestion du mode exploitation
 * @param Aucun
 * @return void
 */
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



// ---------------------------------------------------------------------------
// MODE PROGRAMMATION
// ---------------------------------------------------------------------------
/**
 * @brief Gestion du mode programmation avec saisie non-bloquante
 * @param Aucun
 * @return void
 */
 // serial : PL (Liste) ou PN (Numerique) ou PA (Alphanumérique) ou PK (Pas de saisie, attente touche)
void handleProgrammingMode(void) 
{    // Mode programmation : réveil payload + 1 sec + interface utilisateur
  if (switchToProgrammingMode)    // affiche qu'une fois
  {  
    switchToOperationMode = true;
    OLEDClear();
 //   OLEDDrawScreenTime(0,0);
//    OLEDDrawText(1, 7, 0, "MODE PROGRAMMATION");
    switchToProgrammingMode = false;
// Activer la liste au démarrage si pas encore fait
    if (!startupListActivated)
    {
// afficher structure gestion menu avant 1er menu
debugSerial.println("Lancement Menu principal");
      initStartupList();
    }
  }

// Traite l'écran d'informations (à appeler dans loop)
infoScreenState_t processInfoScreen();


   
#ifdef __SerialDebugPoc    
//debugSerial.print("P");   // PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
#endif

// ------------------------------------------------
// Vérifier si une sélection de liste est en cours 
// ------------------------------------------------
  if (isListInputActive())
  {
    static uint8_t selectedModeIndex = 0; // Index du mode sélectionné
        
#ifdef __SerialDebugPoc    
//debugSerial.print("L");   // LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
#endif

    listInputState_t state = processListInput();
        
    switch (state)
    {               debugSerial.println(menu000Demarrage[selectedModeIndex]); 
      case LIST_INPUT_COMPLETED:
                selectedModeIndex = finalizeListInput(); // Récupérer l'index sélectionné

 // Gestion navigation dans les menus               
                debugSerial.print("Mode selectionne: ");
                debugSerial.print(selectedModeIndex);
                 debugSerial.print(" - Depth: ");
                debugSerial.print(currentMenuDepth);
                debugSerial.print(" - Val: ");
                debugSerial.println(menu000Demarrage[selectedModeIndex]);

          if (currentMenuDepth > 0)
        {
          menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
          debugSerial.println(currentMenu->menuList[selectedModeIndex]);
              
                
                // Ici vous pouvez traiter la sélection

debugSerial.print("Chaine validée : ");
//sprintf(stringSaisie,(char *)menu000Demarrage[selectedModeIndex]);    // recopie saisie dans destination
//debugSerial.println(stringSaisie);
//debugSerial.println(Data_LoRa.RucherName);
debugSerial.println(menu000Demarrage[selectedModeIndex]);

                OLEDClear();// Effacer écran
//                OLEDDrawScreenTime(0, 0); // Affiche Time/Date au complet    
           processMenuSelection(selectedModeIndex);
        }
        else
        {
          debugSerial.println("Cas ELSE");  
         processMenuSelection(selectedModeIndex);
        }
                
                break;
                
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
// ------------------------------------------------    
// Vérifier si un écran d'info est actif                >> doit retourner dans LISTE menu
// ------------------------------------------------    
  else if (isInfoScreenActive())
  {
    infoScreenState_t state = processInfoScreen();
    
    switch (state)
    {
      case INFO_SCREEN_CLOSED:
        debugSerial.println("Ecran info ferme");
        infoScreenState = INFO_SCREEN_IDLE; // Reset

listInputCtx.state = LIST_INPUT_ACTIVE;

        
        break;
        
      default:
        // Écran toujours actif, ne rien faire d'autre
        return;
    }
  }  
// ------------------------------------------------    
// Vérifier si une saisie numérique est en cours
// ------------------------------------------------
  else if (isNumberInputActive())   // numberInputCtx.state == NUMBER_INPUT_ACTIVE
  {
    static char numberBuffer[11] = ""; // Buffer pour le nombre
      
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
#ifdef __SerialDebugPoc    
debugSerial.print("N");   // NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
#endif
  }
// ------------------------------------------------
// Vérifier si une saisie alphanumérique est en cours
// ------------------------------------------------
  else if (isStringInputActive())
  {
    static char stringBuffer[21] = ""; // Buffer pour la chaîne
        
#ifdef __SerialDebugPoc    
debugSerial.print("A");   // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
#endif    

    stringInputState_t state = processStringInput();   // traite la touche lors de la saisie
        
    switch (state)
    {
      case STRING_INPUT_COMPLETED:
                finalizeStringInput(stringBuffer); // Récupérer la chaîne finale
                debugSerial.print("Nouvelle chaine: ");
                debugSerial.println(stringBuffer);
                // Ici vous pouvez traiter la chaîne 

//stringBuffer contient la chaine à copier dans la destination
//      sprintf(Data_LoRa.RucherName,stringBuffer);    

                sprintf(stringSaisie,stringBuffer);    // recopie saisie dans destination
debugSerial.print("Chaine validée : "); debugSerial.print(stringSaisie);debugSerial.println(Data_LoRa.RucherName);
                OLEDClear();// Effacer écran
 //               OLEDDrawScreenTime(0, 0); // Affiche Time/Date au complet     
                break;
                
      case STRING_INPUT_CANCELLED:
                debugSerial.println("Saisie alphanumérique annulee");
                break;
                
      default:
                // Saisie toujours en cours, ne rien faire d'autre
                return;
    }
/*    
    if (stringInputCtx.state != STRING_INPUT_IDLE)  // Fin de saisie String
    {
      sprintf(Data_LoRa.RucherName,stringBuffer);    
      debugSerial.println("copie dans destination");
    }
*/
  }
// ------------------------------------------------
// Gestion normale des menus quand pas de saisie
// ------------------------------------------------    
  else
  {
#ifdef __SerialDebugPoc    
debugSerial.print("K");   // KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
#endif    
    if (touche != KEY_NONE)
    {
      switch (touche)
      {
        case KEY_1: // Touche 1 - Fonction libre
                    debugSerial.println("Touche 1 - Fonction libre");
                    break;
                    
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
                    }
                    break;
                    
        case KEY_3: // Touche 3 - Test saisie numérique
  static char currentNumber[11] = "12345";
  startNumberInput("SAISIE NOMBRE:", currentNumber, 8, false);
                    break;
                    
        case KEY_4: // Touche 4 - Test saisie alphanumérique
 
                        static char currentString[21] = "MBVSB0CBJTPOT0WJUF";
// appeler avec chaine a modifier    ex: Data_LoRa.RucherName[20]
//                        startStringInput("SAISIE TEXTE:", currentString, 20); 

                        stringSaisie=Data_LoRa.RucherName;  // pointeur sur valeur courante traiter
// attention si existe "CANCEL", proceder par variable tampon

                        
                        startStringInput("SAISIE TEXTE:", stringSaisie, 20);
                    break;
                    
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

/**
 * @brief Traite l'écran d'informations (à appeler dans loop)
 * @param void
 * @return État actuel de l'écran
 */
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

/**
 * @brief Vérifie si un écran d'info est actif
 * @param void
 * @return true si écran actif
 */
bool isInfoScreenActive(void)
{
  return (infoScreenState == INFO_SCREEN_ACTIVE);
}
