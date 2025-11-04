
/*
//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//       _____       _     _                            
//      / ____|     (_)   (_)                           
//     | (___   __ _ _ ___ _  ___  ___   ___ _ __  _ __ 
//      \___ \ / _` | / __| |/ _ \/ __| / __| '_ \| '_ \
//      ____) | (_| | \__ \ |  __/\__ \| (__| |_) | |_) |
//     |_____/ \__,_|_|___/_|\___||___(_)___| .__/| .__/
//                                          | |   | |   
//                                          |_|   |_|    
// ---------------------------------------------------------------------------*
#define __INIT_DONE
#include "define.h"

uint64_t inputHex(const char* variable, uint64_t valeurInitiale);
uint32_t inputDecimal(const char* variable, uint32_t valeurInitiale);



// ---------------------------------------------------------------------------*
// @brief Interface de saisie d'une valeur hexadécimale 64 bits
// @param variable Nom de la variable (affichage)
// @param valeurInitiale Valeur initiale à afficher
// @return uint64_t Valeur hexadécimale saisie
// ---------------------------------------------------------------------------*
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
*/
