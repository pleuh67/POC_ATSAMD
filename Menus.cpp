//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//      __  __                                       
//     |  \/  |                                      
//     | \  / | ___ _ __  _   _ ___   ___ _ __  _ __ 
//     | |\/| |/ _ \ '_ \| | | / __| / __| '_ \| '_ \
//     | |  | |  __/ | | | |_| \__ \| (__| |_) | |_) |
//     |_|  |_|\___|_| |_|\__,_|___(_)___| .__/| .__/
//                                       | |   | |   
//                                       |_|   |_|    
// ---------------------------------------------------------------------------*
#define __INIT_DONE
#include "define.h"


// Navigation entre le 5 niveaux de menus : pushMenu() et popMenu()
// C'est ici qu'on navigue vers:
//  - les sous menus
//  - Les fonctions (tarage, balances, calibrations tension, ...)
//  - Les saisies typées (DevEUI (HEX), SF, Var. Texte, date, heure, etc



// PROGRAMMATION MODE
// ---------------------------------------------------------------------------*
//  MENUS
// ---------------------------------------------------------------------------*
// ===== FONCTIONS DE GESTION DES MENUS =====
// ---------------------------------------------------------------------------*
// @brief Empile un nouveau menu dans la pile
// @param title Titre du menu
// @param menuList Pointeur vers la liste du menu
// @param menuSize Nombre d'éléments
// @param initialIndex Index initial sélectionné
// @return void
// ---------------------------------------------------------------------------*
// exemple appel: pushMenu("Menu Niv 3:", menu033Reserve040, M3_ITEM, 0);
void pushMenu(const char* title, const char** menuList, uint8_t menuSize, uint8_t initialIndex)
{
sprintf(serialbuf, "pushMenu() title: %s",title); 
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
    
sprintf(serialbuf,"Menu empile: %s  currentMenuDepth: %d" ,title, currentMenuDepth); 
debugSerial.println(serialbuf);
  }
  else
    OLEDDisplayMessageL8(">5 SOUS MENUS !!!", false, false);  
}

// ---------------------------------------------------------------------------*
// @brief Dépile le menu actuel et revient au précédent
// @param void
// @return void
// ---------------------------------------------------------------------------*
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


// ---------------------------------------------------------------------------*
// @brief Réaffiche le menu actuel après execution Fonction.
// @param void
// @return void
// ---------------------------------------------------------------------------*
void backMenu(void)
{
  if (currentMenuDepth > 0)
  {
    menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
    startListInputWithTimeout(currentMenu->title, currentMenu->menuList, currentMenu->menuSize, currentMenu->selectedIndex, 0);
  } 
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



// ---------------------------------------------------------------------------*
// @brief Traite la sélection d'un élément de menu
// @param selectedIndex Index de l'élément sélectionné
// @return void
// ---------------------------------------------------------------------------*
void processMenuSelection(uint8_t selectedIndex)
{
  if (currentMenuDepth == 0)
  {
    debugSerial.println("Erreur: Aucun menu actif");
    return;
  }

#ifdef __SerialDebugPoc    
 debugSerial.print("M");   // MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
#endif

  
  menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
debugSerialPrintMenuStruct(currentMenu);
// => debugSerialPrintMenuStruct(): currentMenuDepth 1 / title  -- MENU PRINCIPAL -- / menuSize 5 /selectedIndex 0

// Sauvegarder la sélection dans le menu actuel
  currentMenu->selectedIndex = selectedIndex;

debugSerial.print("processMenuSelection ");
debugSerial.println(selectedIndex);  

// Traitement selon le menu actuel
// ------------------------------------------------------------m0_Demarrage---*
  if (currentMenu->menuList == m0_Demarrage)    // Menu principal
  {
    switch (selectedIndex)
    {
      case 0: // "INFOS"
      {
debugSerial.print("Appel d'un ecran: ");        
        m0_0E_PageInfosSyst();   // APPEL D'UN AFFICHAGE D'ECRAN 
debugSerial.println("CONFIG. SYSTEME - Ecran INFOS demandé");   
        break;
      }
      case 1: // "CONFIG. SYSTEME"
      {
        m0_1M_ConfigSystem();       // APPEL D'UN MENU
        break;
      }
      case 2: // "CONNEX. RESEAU"
      {  
        m0_2M_ConfigLoRa();
        break;
      }  
      case 3: // "CALIB. TENSIONS"
      {
        m0_3M_CalibTensions();
        break;
      }
      case 4: // "CALIB. BALANCES"
      {  
        m0_4M_CalibBal();    
        break;
      }
      default:
      {
        break;
      }  
    }
  }
// -------------------------------------------------------m01_ConfigSystem---*
  else if (currentMenu->menuList == m01_ConfigSystem)
  {
    switch (selectedIndex)
    {
      case 0: // "SAISIE DATE"
      {
        m01_0F_GetDate();   // APPEL D'UNE SAISIE, ici DATE
        break;
      }
      case 1: // "SAISIE HEURE"
      {
        m01_1F_GetTime();   // APPEL D'UNE SAISIE, ici HEURE
        break;
      }
     case 2:  // Numéro de Rucher
      {
debugSerial.println("m01_2F_GetNumRucher demandé");   
        m01_2F_GetNumRucher();
        break;
      }
      case 3: // Nom de Rucher (Liste avec autre  => saisie TXT)
      {
debugSerial.println("m01_3F_GetNameRucher demandé");   
        m01_3F_GetNameRucher();
        break;
      }
      case 4: // ReadEEPROM
      {  
debugSerial.println("m01_5F_readConfig demandé");   
        m01_4F_readConfig();
        break;
      }
      case 5: // WriteEEPROM
      {  
debugSerial.println("m01_6F_writeConfig demandé");   
        m01_5F_writeConfig();
        break;
      }
      
      case 6: // Retour m0_Demarrage
      {
        m01_6M_PopMenu();    // Retour au menu principal
        break;
      }  
      default:
      {
        break;
      }  
    }
  }
// ----------------------------------------------------------m02_ConfigLoRa---*
  else if (currentMenu->menuList == m02_ConfigLoRa)
  {
    switch (selectedIndex)
    {
      case 0: // "INFOS"
      {
debugSerial.print("Appel d'un ecran: ");        
        m02_0E_PageInfosLoRa();   // APPEL D'UN AFFICHAGE D'ECRAN 
debugSerial.println("CONFIG. SYSTEME - Ecran INFOS demandé");   
        break;
      }
      case 1: // DevEUI => HEX
      {
debugSerial.println("m02_1F_GetHex demandé");   
        m02_1F_GetHex();
        break;
      }
      case 2: // AppEUI => HEX
      {
debugSerial.println("m02_2F_GetHex demandé");   
        m02_2F_GetHex();
        break;
      }
      case 3: // SpreadFactor => ListeSF
      {
debugSerial.println("m02_3L_GetSF demandé");   
        m02_3L_GetSF();
        break;
      }
      case 4: // delayPayload => TXT converti en uint8
      {
debugSerial.println("m02_4F_GetPayloadDelay demandé");   
        m02_4F_GetPayloadDelay();
        break;
      }
      case 5: // Demande de connexion réseau LoRa
      {
debugSerial.println("m02_5F_Join demandé");   
        m02_5F_Join(); // Connexion LoRa
        break;
      }
      case 6: // SendPayload 
      {
debugSerial.println("m02_6F_SendPayload() demandé");   
        m02_6F_SendPayLoad(); // Envoyer Payload  
        break;
      }
      case 7: // Retour m0_Demarrage
      {
        popMenu(); // Retour au menu principal
        break;
      }  
      default:
      {
        break;
      }  
    }
  }
// -------------------------------------------------------m03_CalibTensions---*
  else if (currentMenu->menuList == m03_CalibTensions)
  {
    switch (selectedIndex)
    {
      case 0: // "Calib. VBAT"
      {
        debugSerial.println("m03_0F_CalibVBat - Fonction a implementer");
        m03_0F_CalibVBat();
//        popMenu(); // Retour au menu principal
        break;
      }  
      case 1: // "Calib. VSOL"
      {
        debugSerial.println("m03_1F_CalibVSol - Saisie numerique a implementer");
        m03_1F_CalibVSol();
// startNumericInput("CALIB VSOL:", &variable_vsol, min, max);
        break;
      }  
      case 2: // "Calib. LUM"
      {
        debugSerial.println("m03_2F_CalibVLum - Fonction a implementer");
        m03_2F_CalibVLum();
        break;
      } 
      case 3: // "Reserve"
      {
        debugSerial.println("m03_3F_Reserve - Fonction libre");
        pushMenu("Menu Niv 3:", m033_Reserve, M033_ITEM , 0);
        break;
      } 
      case 4:  // Retour m0_Demarrage
      {
        popMenu(); // Retour au menu principal
        break;
      }  
      default:
        break;
    }
  }  
// -------------------------------------------------------m033_Reserve--------*
  else if (currentMenu->menuList == m033_Reserve)
  {
    switch (selectedIndex)
    {
      case 0: //
      {
        debugSerial.println("m033_0F_xxx - Fonction a implementer");
      }  
      case 1: //
      {
        debugSerial.println("m033_1F_xxx - Fonction a implementer");
        break;
      }  
      case 2: //
      {
        debugSerial.println("m033_2F_xxx - Fonction a implementer");
        break;
      } 
      case 3: 
      {
        debugSerial.println("m033_3F_xxx - Fonction a implementer");
        break;
      } 
      case 4:  // Retour m0_Demarrage
      {
        popMenu(); // Retour au menu principal
        break;
      }  
      default:
        break;
    }
  }  
  
// -------------------------------------------------------m04_CalibBalances---*
  else if (currentMenu->menuList == m04_CalibBalances)
  {
    switch (selectedIndex)
    {
      case 0:  
      {
bal = 1;   
sprintf(serialbuf,"m04_0 demandé avec balance %d : m04_CalibBal[]", bal); 
debugSerial.println(serialbuf); 
m04_0F_CalibBal_1();      
        break;
      }
      case 1: 
      {
bal = 2;    
sprintf(serialbuf,"m04_1 demandé avec balance %d : m04_CalibBal[]", bal); 
debugSerial.println(serialbuf); 
m04_nM_CalibBal_bal();
        break;
      }
      case 2:
      {
bal = 3;    
sprintf(serialbuf,"m04_2 demandé avec balance %d : m04_CalibBal[]", bal); 
debugSerial.println(serialbuf); 
m04_nM_CalibBal_bal();
        break;
      }
      case 3: 
      {
bal = 4;    
sprintf(serialbuf,"m04_3 demandé avec balance %d : m04_CalibBal[]", bal); 
debugSerial.println(serialbuf); 
m04_nM_CalibBal_bal();
        break;
      }
      case 4:  // Retour m0_Demarrage
      {
        popMenu(); // Retour au menu principal
        break;
      }  
      default:
      {
        break;
      }  
    }
  }  
// -----------------------------------------------------------m04x_CalibBal---*
  else if (currentMenu->menuList == m04x_CalibBal)
  {
    switch (selectedIndex)
    {
      case 0: // m04x-0
        sprintf(serialbuf,"menu04x-0  demandé avec Bal %d pour Tare Balance", bal);
// tester cas pour tare Balance
        m02_1F_GetHex();    // "SAISIE HEXA "

        
        
        
        break;
      case 1: // m04x-1
        sprintf(serialbuf,"menu04x-1  demandé avec Bal %d pour Echelle Balance", bal);
        break;
      case 2: // m04x-2
        sprintf(serialbuf,"menu04x-2  demandé avec Bal %d pour Comp. T° Balance", bal);
        break;
      case 3: // RET   popMenu(M04)
        popMenu(); // Retour au menu: m04_CalibBalances
        break;
      default:
        break;
    }
  }
// ---------------------------------------------------------------------------*
/*
// A decliner selon les menus
// ---------------------------------------------------------------------------*
  else if (currentMenu->menuList == AutreMenu)
  {
    switch (selectedIndex)
    {
      case 1:  
              {
 
                break;
              }
      case 2: 
              {
 
                break;
              }
      case 3:
              {
 
                break;
              }
      case 4:
              {

                break;
              }
      case 5:
              {  

                break;
              }
     case 6:
              {
 
                break;
              }
      default:
              {
                break;
              }  
    }
  }  
*/
}
