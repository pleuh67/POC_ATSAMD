
#define __INIT_DONE
#include "define.h"

void inputDate(char *d);
void modifyDateDigit(char *d, uint8_t pos, int delta);
bool isDateValid(const char *d);



void inputTime(char *h);
void modifyTimeDigit(char *h, uint8_t pos, int delta);
bool isTimeValid(const char *h);


void inputListValue(const char *label, const int *liste, uint8_t nbValeurs, int *valeurSelectionnee);
void inputListValueLibelle(const char *label, const int *valeurs, const char **libelles, uint8_t nbValeurs, int *valeurSelectionnee, bool *valide);
uint64_t inputHex(const char* variable, uint64_t valeurInitiale);
uint32_t inputDecimal(const char* variable, uint32_t valeurInitiale);


/**
 * @brief Vérifie si une date est valide
 * @param d Chaîne de caractères représentant la date (format jj/mm/aaaa)
 * @return bool True si la date est valide
 */
bool isDateValid(const char *d) 
{
    int jj = (d[0]-'0')*10 + (d[1]-'0');
    int mm = (d[3]-'0')*10 + (d[4]-'0');
    int yyyy = (d[6]-'0')*1000 + (d[7]-'0')*100 + (d[8]-'0')*10 + (d[9]-'0');
    
    if (mm < 1 || mm > 12 || yyyy < 1900 || yyyy > 2099) 
        return false;
    
    int joursMax[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    
    if ((yyyy % 4 == 0 && yyyy % 100 != 0) || (yyyy % 400 == 0)) 
        joursMax[1] = 29;
    
    return jj >= 1 && jj <= joursMax[mm - 1];
}

/**
 * @brief Modifie un chiffre d'une date
 * @param d Chaîne de caractères de la date
 * @param pos Position du chiffre à modifier
 * @param delta Valeur à ajouter (+1 ou -1)
 * @return void
 */
void modifyDateDigit(char *d, uint8_t pos, int delta) 
{
    if (d[pos] >= '0' && d[pos] <= '9') 
    {
        int val = d[pos] - '0' + delta;
        if (val >= 0 && val <= 9) 
        {
            d[pos] = val + '0';
            OLEDDisplayMessageL8(isDateValid(d) ? "Date valide" : "Date invalide !", false, false);
        }
    }
}



/**
 * @brief Interface de saisie d'une date
 * @param d Chaîne de caractères pour stocker la date saisie
 * @return void
 */
void inputDate(char *d) 
{
    uint8_t pos = 0;
    bool done = false;

  sprintf(serialbuf,"%s",d);
  debugSerial.println(serialbuf); 

    while (!done) 
    {
        OLEDDisplayDate(d, pos);
        key_code_t t = readKeyOnce();

  sprintf(serialbuf,"Touche : %s / %d", keyToString(t), pos);
  debugSerial.println(serialbuf); 

        switch (t) 
        {
            case LEFT:  
                if (pos > 0) pos--; 
                break;
            case RIGHT: 
                if (pos < 9) pos++; 
                break;
            case UP:    
                modifyDateDigit(d, pos, +1); 
                break;
            case DOWN:  
                modifyDateDigit(d, pos, -1); 
                break;
            case VALIDE: 
                if (isDateValid(d)) done = true; 
                break;
        }
        delay(20);
    }
}

/**
 * @brief Vérifie si une heure est valide
 * @param h Chaîne de caractères représentant l'heure (format hh:mm:ss)
 * @return bool True si l'heure est valide
 */
bool isTimeValid(const char *h) 
{
    int hh = (h[0]-'0')*10 + (h[1]-'0');
    int mm = (h[3]-'0')*10 + (h[4]-'0');
    int ss = (h[6]-'0')*10 + (h[7]-'0');
    return hh <= 23 && mm <= 59 && ss <= 59;
}

/**
 * @brief Modifie un chiffre d'une heure
 * @param h Chaîne de caractères de l'heure
 * @param pos Position du chiffre à modifier
 * @param delta Valeur à ajouter (+1 ou -1)
 * @return void
 */
void modifyTimeDigit(char *h, uint8_t pos, int delta) 
{
    if (h[pos] >= '0' && h[pos] <= '9') 
    {
        int val = h[pos] - '0' + delta;
        if (val >= 0 && val <= 9) 
        {
            h[pos] = val + '0';
            OLEDDisplayMessageL8(isTimeValid(h) ? "Heure valide" : "Heure invalide !", false, false);
        }
    }
}

/**
 * @brief Interface de saisie d'une heure
 * @param h Chaîne de caractères pour stocker l'heure saisie
 * @return void
 */
void inputTime(char *h) 
{
    uint8_t pos = 0;
    bool done = false;
    
    while (!done) 
    {
        OLEDDisplayTime(h, pos);
        key_code_t t = readKey();
        
        switch (t) 
        {
            case LEFT:  
                if (pos > 0) pos--; 
                break;
            case RIGHT: 
                if (pos < 7) pos++; 
                break;
            case UP:    
                modifyTimeDigit(h, pos, +1); 
                break;
            case DOWN:  
                modifyTimeDigit(h, pos, -1); 
                break;
            case VALIDE: 
                if (isTimeValid(h)) done = true; 
                break;
        }
        delay(20);
    }
}

/**
 * @brief Permet de sélectionner une valeur parmi une liste fixe
 * @param label Texte à afficher (ligne 0)
 * @param liste Tableau de valeurs possibles
 * @param nbValeurs Nombre d'éléments dans la liste
 * @param valeurSelectionnee Pointeur vers la variable qui recevra la valeur choisie
 * @return void
 */
void inputListValue(const char *label, const int *liste, uint8_t nbValeurs, int *valeurSelectionnee) 
{
    uint8_t index = 0;
    bool valide = false;
    
    while (!valide) 
    {
        OLEDClear();
        OLEDDrawText(1,0, 0, label);
        
        sprintf(OLEDbuf, "Valeur : %d", liste[index]);
        OLEDDrawText(1,1, 0, OLEDbuf);
        OLEDDisplayMessageL8("Valider: VALIDE/UP/DOWN", false, false);
        
        key_code_t touche = readKey();
        switch (touche) 
        {
            case UP:
                if (index > 0) index--;
                break;
            case DOWN:
                if (index < nbValeurs - 1) index++;


    debugSerial.println("Down de inputListValue");  
                
                break;
            case VALIDE:
                *valeurSelectionnee = liste[index];
                OLEDDisplayMessageL8("Valeur confirmée !", false, false);
                valide = true;
                break;
        }
        delay(150);
    }
}

/**
 * @brief Permet de sélectionner une valeur avec libellé parmi une liste
 * @param label Texte à afficher
 * @param valeurs Tableau des valeurs
 * @param libelles Tableau des libellés correspondants
 * @param nbValeurs Nombre d'éléments
 * @param valeurSelectionnee Pointeur vers la valeur sélectionnée
 * @param valide Pointeur vers le flag de validation
 * @return void
 */
void inputListValueLibelle(const char *label, const int *valeurs, const char **libelles,
                               uint8_t nbValeurs, int *valeurSelectionnee, bool *valide) 
{
    uint8_t index = 0;
    *valide = false;
    bool enCours = true;
    
    while (enCours) 
    {
        OLEDClear();
        OLEDDrawText(1,0, 0, label);
        
        char ligne1[21];
        sprintf(ligne1, "%s (%d)", libelles[index], valeurs[index]);
        OLEDDrawText(1,1, 0, ligne1);
        OLEDDrawText(1,3, 0, "T3: ↑  T4: ↓");
        OLEDDrawText(1,4, 0, "T5: Valider  T2: Annuler");
        
        key_code_t touche = readKey();
        switch (touche) 
        {
            case UP:
                if (index > 0) index--;
                break;
            case DOWN:
                if (index < nbValeurs - 1) index++;
                break;
            case VALIDE:
                *valeurSelectionnee = valeurs[index];
                *valide = true;
                OLEDDisplayMessageL8("Valeur validée !", false, false);
                enCours = false;
                break;
            case ANNULER_4:
                OLEDDisplayMessageL8("Saisie annulée", false, false);
                enCours = false;
                break;
        }
        delay(150);
    }
}

/**
 * @brief Interface de saisie d'une valeur hexadécimale 64 bits
 * @param variable Nom de la variable (affichage)
 * @param valeurInitiale Valeur initiale à afficher
 * @return uint64_t Valeur hexadécimale saisie
 */
uint64_t inputHex(const char* variable, uint64_t valeurInitiale) 
{
    char digits[17];
    uint8_t pos = 0;
    
    debugSerial.print("inputHex()  ");
    
    for (int i = 15; i >= 0; i--) 
    {
        digits[i] = "0123456789ABCDEF"[valeurInitiale & 0xF];
        valeurInitiale >>= 4;
    }
    digits[16] = '\0';
    
    debugSerial.print(digits);    

    bool valide = false;
    while (!valide) 
    {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.print("Saisie HEXA");
        display.setCursor(0, 8);
        display.print(variable);
        display.setCursor(0, 28);
        
        for (int i = 0; i < 16; i++) 
        {
            if (i == pos) 
            {
                display.setTextColor(BLACK, WHITE);
                display.print(digits[i]);
                display.setTextColor(WHITE);
            } 
            else 
            {
                display.print(digits[i]);
            }
        }
        display.display();
        
        key_code_t touche = readKey();
        if (touche == MOINS) 
        {
            if (digits[pos] == '0') digits[pos] = 'F';
            else if (digits[pos] == 'A') digits[pos] = '9';
            else digits[pos]--;
        }
        else if (touche == PLUS) 
        {
            if (digits[pos] == '9') digits[pos] = 'A';
            else if (digits[pos] == 'F') digits[pos] = '0';
            else digits[pos]++;
        }
        else if (touche == LEFT && pos > 0) 
        {
            pos--;
        }
        else if (touche == RIGHT && pos < 15) 
        {
            pos++;
        }
        else if (touche == VALIDE) 
        {
            valide = true;
        }
        delay(200);
    }
    
    uint64_t resultat = 0;
    for (int i = 0; i < 16; i++) 
    {
        char c = digits[i];
        resultat <<= 4;
        if (c >= '0' && c <= '9') resultat += (c - '0');
        else if (c >= 'A' && c <= 'F') resultat += (c - 'A' + 10);
    }
    
    debugSerial.print(" => ");debugSerial.println(digits);
    return resultat;
}

/**
 * @brief Interface de saisie d'une valeur décimale 32 bits
 * @param variable Nom de la variable (affichage)
 * @param valeurInitiale Valeur initiale à afficher
 * @return uint32_t Valeur décimale saisie
 */
uint32_t inputDecimal(const char* variable, uint32_t valeurInitiale) 
{
    char digits[7];
    uint8_t pos = 0;
    
    debugSerial.print("inputDecimal()  ");
    debugSerial.print(valeurInitiale);
    
    for (int i = 5; i >= 0; i--)
    {
        digits[i] = '0' + (valeurInitiale % 10);
        valeurInitiale /= 10;
    }
    digits[6] = '\0';
    
    bool valide = false;
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Saisie DEC.");
    display.setCursor(0, 8);
    display.print(variable);
    
    while (!valide) 
    {
        display.setCursor((SCREEN_WIDTH - 6 * 6) / 2, 28);
        for (int i = 0; i < 6; i++)
        {
            if (i == pos) 
            {
                display.setTextColor(BLACK, WHITE);
                display.print(digits[i]);
                display.setTextColor(WHITE);
            } 
            else 
            {
                display.print(digits[i]);
            }
        }
        display.display();
        
        key_code_t touche = readKey();
        switch(touche)
        { 
            case MOINS:
                digits[pos] = (digits[pos] == '0') ? '9' : digits[pos] - 1;
                break;
            case PLUS:
                digits[pos] = (digits[pos] == '9') ? '0' : digits[pos] + 1;      
                break;
            case LEFT:
                if (pos > 0) 
                {
                    pos--;
                } 
                break;
            case RIGHT:
                if (pos < 5) 
                {
                    pos++;
                } 
                break;
            case VALIDE: 
                valide = true;
                break;
        }
        delay(200);
    }
    
    uint32_t resultat = 0;
    for (int i = 0; i < 6; i++) 
    {
        resultat = resultat * 10 + (digits[i] - '0');
    }
    
    debugSerial.print(" => ");debugSerial.println(resultat);
    return resultat;
}

// ---------------------------------------------------------------------------*
//  Converts String to char*                             
// ---------------------------------------------------------------------------*
char* strToChar(String s) 
{
  unsigned int bufSize = s.length() + 1; //String length + null terminator
  char* ret = new char[bufSize];
  s.toCharArray(ret, bufSize);
  return ret;
}
