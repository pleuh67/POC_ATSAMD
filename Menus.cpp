#define __INIT_DONE
#include "define.h"

// PROGRAMMATION MODE
// *************************************************************************************
//  MENUS
// *************************************************************************************
// ===== FONCTIONS DE GESTION DES MENUS =====
/**
 * @brief Empile un nouveau menu dans la pile
 * @param title Titre du menu
 * @param menuList Pointeur vers la liste du menu
 * @param menuSize Nombre d'éléments
 * @param initialIndex Index initial sélectionné
 * @return void
 */


//         pushMenu("Menu Niv 3:", menu043Reserve040, 5, 0);

 
void pushMenu(const char* title, const char** menuList, uint8_t menuSize, uint8_t initialIndex)
{
sprintf(serialbuf, "pushMenu() %s",title); 
debugSerial.println(serialbuf);
  if (currentMenuDepth < MAX_MENU_DEPTH)
  {
    menuStack[currentMenuDepth].menuList = menuList;
    menuStack[currentMenuDepth].menuSize = menuSize;
    menuStack[currentMenuDepth].selectedIndex = initialIndex;
    strncpy(menuStack[currentMenuDepth].title, title, 20);
    menuStack[currentMenuDepth].title[20] = '\0';
    
    currentMenuDepth++;
    
    // Démarrer l'affichage du nouveau menu
    startListInputWithTimeout(title, menuList, menuSize, initialIndex, 0);
    
    debugSerial.print("Menu empile: ");
    debugSerial.print(title);
    debugSerial.print(" Profondeur: ");
    debugSerial.println(currentMenuDepth);
  }
  else
    OLEDDisplayMessageL8(">5 SOUS MENUS !!!", false, false);  
}

/**
 * @brief Dépile le menu actuel et revient au précédent
 * @param void
 * @return void
 */
void popMenu(void)
{
sprintf(serialbuf, "popMenu()"); 
debugSerial.println(serialbuf);  
  if (currentMenuDepth > 1)
  {
    currentMenuDepth--;
    
    // Récupérer le menu précédent
    menuLevel_t* prevMenu = &menuStack[currentMenuDepth - 1];
    
    // Redémarrer l'affichage du menu précédent
    startListInputWithTimeout(prevMenu->title, prevMenu->menuList, prevMenu->menuSize, prevMenu->selectedIndex, 0);
    
    debugSerial.print("Retour menu precedent: ");
    debugSerial.print(prevMenu->title);
    debugSerial.print(" Profondeur: ");
    debugSerial.println(currentMenuDepth);
  }

// Apres 
  debugSerial.print(" menu apres popMenu(): ");
  debugSerialPrintMenuStruct(&menuStack[currentMenuDepth - 1]);  
}









// isListInputActive()FALSE
// n'est plus vrai dans handle donc teste plus comme si reponse à liste
// Trouver ou il se devalide (en passant par Fonction())
//
//    case VALIDE:
//        listInputCtx.state = LIST_INPUT_COMPLETED;

// voir aussi apres uint8_t finalizeListInput(void)
    // Reset du contexte
//    listInputCtx.state = LIST_INPUT_IDLE;
//    listInputCtx.selectedIndex = 0;
//    listInputCtx.scrollOffset = 0;
//    listInputCtx.maxItems = 0;
//    listInputCtx.itemList = NULL;



/**
 * @brief Traite la sélection d'un élément de menu
 * @param selectedIndex Index de l'élément sélectionné
 * @return void
 */
void processMenuSelection(uint8_t selectedIndex)
{
  if (currentMenuDepth == 0)
  {
    debugSerial.println("Erreur: Aucun menu actif");
    return;
  }
  
  menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
debugSerialPrintMenuStruct(currentMenu);
  
  // Sauvegarder la sélection dans le menu actuel
  currentMenu->selectedIndex = selectedIndex;

debugSerial.print("processMenuSelection ");
debugSerial.println(selectedIndex);  
  // Traitement selon le menu actuel
// ---------------------------------------------------------------------------------
// menu000Demarrage (MENU de démarrage)
// ---------------------------------------------------------------------------------

  if (currentMenu->menuList == menu000Demarrage)
  {
    // Menu principal
    switch (selectedIndex)
    {
    // ----------------------------------
  // APPEL D'UN AFFICHAGE D'ECRAN  
    // ----------------------------------   
      case 1: // "INFOS"
        debugSerial.println("Appel d'un ecran");
        debugSerial.println("CONFIG. SYSTEME - Ecran INFOS demandé");     
        OLEDdisplayInfoScreen();
        // popMenu(); // Retour au menu principal
        break;
    // ----------------------------------
  // APPEL D'UNE FONCTION
    // ----------------------------------        
      case 2: // "CONFIG. SYSTEME"
        debugSerial.println("Appel d'une Fonction");
        debugSerial.println("CONFIG. SYSTEME - Fonction a implementer");
        break;
        
      case 3: // "CONNEX. RESEAU"
        debugSerial.println("Appel d'une Fonction");      
        debugSerial.println("CONNEX. RESEAU - Fonction a implementer");
        break;
    // ----------------------------------
  // APPEL D'UN SOUS MENU    
    // ----------------------------------
    case 4: // "CALIB. TENSIONS"
        debugSerial.println("Appel d'un sous menu");    
        pushMenu("CALIB. TENSIONS:", menu040CalibTensions, 5, 0);
        break;
        
      case 5: // "CALIB. BALANCES"
        debugSerial.println("Appel d'une Fonction");
        debugSerial.println("Appel weightCalibration(1)");
        // weightCalibration(1); // Fonction existante
        break;
    // ----------------------------------
  // APPEL D'UNE SAISIE, ici DATE
    // ----------------------------------        
     case 6: // "SAISIE DATE"
    {
      debugSerial.println("Appel d'une saisie date");      
      debugSerial.println("Lancement saisie date a importer");
      static char currentDate[11] = /*__DATE__; //*/"01/01/2024"; // Format DD/MM/YYYY

debugSerial.print(__DATE__);
debugSerial.println(" => CONFIG. SYSTEME - Demande saisie DATE");

// importer 
//      startDateInput(currentDate);

      // Mise à la date DS32331
    }
    break;
    // ----------------------------------
  // APPEL D'UNE SAISIE, ici HEURE
    // ----------------------------------        
     case 7: // "SAISIE HEURE"
    {
      debugSerial.println("Lancement saisie heure a importer");
      static char currentTime[11] = __TIME__; //  "08:12:50"; // Format HH:MM:SS
debugSerial.println("CONFIG. SYSTEME - Demande saisie TIME");

// importer 
            //startTimeInput(currentTime);
            startTimeInputWithTimeout(const char* title, const char* initialNumber, unsigned long timeoutMs);

  DateTime systemTime = getSystemTime();
  sprintf(OLEDbuf, "%02d:%02d:%02d    %02d/%02d/%02d", 
          systemTime.hour(), systemTime.minute(), systemTime.second(),
          systemTime.day(), systemTime.month(), systemTime.year()-2000);
debugSerial.println(OLEDBuf);              
 
      // Mise à l'heure DS32331

// Activer la liste au démarrage si pas encore fait
 //   if (!startupListActivated)
    {
// afficher structure gestion menu avant 1er menu
debugSerial.println("Relance Menu principal");
      initStartupList();
    }

      
    }
    break;
        
      case 8: // "LISTE_MENU9"
        debugSerial.println("LISTE_MENU9 - Fonction a implementer");
        break;
        
      default:
        break;
    }
  }
// ---------------------------------------------------------------------------------
// menu040CaliTensions
// ---------------------------------------------------------------------------------
  else if (currentMenu->menuList == menu040CalibTensions)
  {
    switch (selectedIndex)
    {
      case 0: // "Calib. VBAT"
        debugSerial.println("Calib. VBAT - Fonction a implementer");
//        popMenu(); // Retour au menu principal
        break;
        
      case 1: // "Calib. VSOL"
        debugSerial.println("Calib. VSOL - Saisie numerique a implementer");
// startNumericInput("CALIB VSOL:", &variable_vsol, min, max);
//        popMenu(); // Retour au menu principal
        break;
        
      case 2: // "Calib. LUM"
        debugSerial.println("Calib. LUM - Fonction a implementer");
//        popMenu(); // Retour au menu principal
        break;
        
      case 3: // "Reserve"
        debugSerial.println("Reserve - Fonction libre");
        pushMenu("Menu Niv 3:", menu043Reserve040, 5, 0);
//        popMenu(); // Retour au menu principal
        break;
        
      case 4: // "RETOUR"
        popMenu(); // Retour au menu principal
        break;
        
      default:
        break;
    }
  }  
// ---------------------------------------------------------------------------------
// menu043Reserve040[]
// ---------------------------------------------------------------------------------
  else if (currentMenu->menuList == menu043Reserve040)
  {
    switch (selectedIndex)
    {
      case 0: // menu043-0
        debugSerial.println("0");
//        popMenu(); // Retour au menu principal
        break;
        
      case 1: // menu043-1
        debugSerial.println("menu043-1");
//        popMenu(); // Retour au menu principal
        break;
        
      case 2: // menu043-2
        debugSerial.println("menu043-2");
//        popMenu(); // Retour au menu principal
        break;
        
      case 3: // menu043-3
        debugSerial.println("menu043-3");
//        popMenu(); // Retour au menu principal
        break;
        
      case 4: // RET   popMenu(M040)
        popMenu(); // Retour au menu: menu040CaliTensions
        break;
      default:
        break;
    }
  }
/*
// A decliner selon les menus
   else if (currentMenu->menuList == autreMenu)
  {
  }
*/
}
