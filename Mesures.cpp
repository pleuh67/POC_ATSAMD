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


//
//  M   M  EEEEE   SSS   U   U  RRRR   EEEEE  SSS    
//  MM MM  E      S      U   U  R   R  E     S      
//  M M M  EEE     SSS   U   U  RRRR   EEE    SSS   
//  M   M  E          S  U   U  R  R   E         S   
//  M   M  EEEEE   SSS    UUU   R   R  EEEEE  SSS  
//
// Projet Ruche PhLe
//
//
// ---------------------------------------------------------------------------*

#define __INIT_DONE
#include "define.h"


// DHT bloquant en erreur.

// ILS avec anti rebond : 47k en Pull Up et 100nf // ILS

/*
   typedef struct
  {
  uint8_t rucher_ID;        // ID Rucher           xx  uint8_t
  float   DHT_Temp;         // Temp DHT en °C    xx,x  Float
  float   DHT_Hum;          // Hum DHT en %      xx,x  Float
  float   Brightness;       // %Lum en LUX      xxxxx  uint16_t
  float   Bat_Voltage;      // Tension BAT en V     xx,xx  Float (uint16_t)
  float   Solar_Voltage;    // Tension BAT en V     xx,xx  Float (uint16_t)
  float   HX711Weight_1;    // masse Ruche 1 en kg xxx,xx (précision affich. 10g)
  float   temp_1;           // température peson 1 en °C xx,xx  Float
  float   HX711Weight_2;    // masse Ruche 2 en kg xx,xx
  float   temp_2;           // masse Ruche 1 en kg xx,xx  Float
  float   HX711Weight_3;    // masse Ruche 3 en kg xx,xx
  float   temp_3;           // masse Ruche 1 en kg xx,xx  Float
  float   HX711Weight_4;    // masse Ruche 4 en kg xx,xx
  float   temp_4;           // masse Ruche 1 en kg xx,xx  Float
//  float   VSol[11];
//  float   VBat[11];
  float   Lux;              // temp µC, ne sera pas conservé
  float   ProcessorTemp;    // temp µC, ne sera pas conservé
  } LoRa_Var;


*/

// ---------------------------------------------------------------------------*
//       indices des macros de 0 à 3 ou de 1 à 4????? manque de cohérence!           
// ---------------------------------------------------------------------------*
void take_All_Measure()  // durée de la proc?
{

/*       
  if  (!read_DHT(dht))  // 
  {
//  sprintf(serialbuf,"Humidité : %.2f %%\nTemperature : %.2f °C",Data_LoRa.DHT_Hum,Data_LoRa.DHT_Temp);
//  debugSerial.println(serialbuf); 
    OLEDDebugDisplay("DHT Done");
  }
  else
    OLEDDebugDisplay("DHT: Error");
 // ---------------------------------------------------------------------------* 
  
  read_DHT(dht); // initialise : Data_LoRa.DHT_Temp et Data_LoRa.DHT_Hum
  
  
  readingT=getTemperature();
  Data_LoRa.Brightness = getLuminance();
//  Data_LoRa.Lux = ???();
//  Data_LoRa.ProcessorTemp = getTemperature(); // lecture Temp en String
  Data_LoRa.Solar_Voltage=getVSolMoy();
  Data_LoRa.Bat_Voltage=getVBatMoy();

//#ifdef WEIGHT_YES

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// poids peson doit être de 0 à 3 et GetPoids de 1..4 confirmé 06/06/2025
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// rappel : #define Poids_Peson(num)      Data_LoRa.HX711Weight[num] 
// rappel : #define Temp_Peson(num)       Data_LoRa.Temp_Peson[num]
// rappel : #define Tare_Peson(num)       Jauge[Peson[Ruche.Num_Carte][num]][0]
// rappel : #define Echelle_Peson(num)    Jauge[Peson[Ruche.Num_Carte][num]][1]
//#define BalPoids(num)  (Contrainte_List[num]-Jauge[Peson[Ruche.Num_Carte][num]][0])/Jauge[Peson[Ruche.Num_Carte][num]][1]/1000) //retourne float
// rappel : #define BalPoids(num) (Contrainte_List[num]-Tare_Peson(num))/Echelle_Peson(num)/1000 //retourne float
// ?? attention step(num) entre TareTemp et la correespondance
//         1..4                    0..3
// rappel : #define TareTemp(num)   Jauge[Peson[Ruche.Num_Carte][num]][2]  // Ruche de type HW_equipement (ligne 21)
// rappel : #define CompTemp(num)   Jauge[Peson[Ruche.Num_Carte][num]][3]

  Poids_Peson(0) = GetPoids(1,1); 
  Poids_Peson(1) = GetPoids(2,1); // à ranger dans Structure
  Poids_Peson(2) = GetPoids(3,1); // à ranger dans Structure
  Poids_Peson(3) = GetPoids(4,1); // à ranger dans Structure
/* 
// compensation de 2 vers 4 pour validation règle de correction
static float oldpoids2=0;

Poids_Peson(2) = Poids_Peson(2)-0.5 - (Temp_Peson(2)-TareTemp(2))*CompTemp(2); // decalé de 500g
if (oldpoids2 ==0)
  oldpoids2 = Poids_Peson(2);
  float delta = abs(Poids_Peson(2)-oldpoids2);
  sprintf(serialbuf, "%6.3f",Poids_Peson(2) );
debugSerial.println(serialbuf); 
  sprintf(serialbuf, "%6.3f",oldpoids2);
debugSerial.println(serialbuf); 
  sprintf(serialbuf, "%6.3f",delta);
debugSerial.println(serialbuf); 
  if ( delta < 0.1) // moyennage si variation inf à 100g
    Poids_Peson(2)=(oldpoids2 + Poids_Peson(2)) / 2;  // moyenné sur moyenne(2points) + nlle mesure.
  sprintf(serialbuf, "%6.3f",Poids_Peson(2) );
debugSerial.println(serialbuf); 
  oldpoids2 = Poids_Peson(2);  // memorise poids
  Poids_Peson(3) = Poids_Peson(1) - (Temp_Peson(1)-TareTemp(1))*CompTemp(1); 
//#endif // WEIGHT_YES
*/
//debugSerial.println("Take_All_Measure");
debugSerial.println(" Temp  Hum   Vs   Vb    P1     P2     P3     P4   tare1 comp1  lum");
sprintf(serialbuf, "%04.2f %04.2f %04.2f %04.2f %06.2f %06.2f %06.2f %06.2f %04.2f %04.2f   %04.2f",
                 Data_LoRa.DHT_Temp,Data_LoRa.DHT_Hum,Data_LoRa.Solar_Voltage,Data_LoRa.Bat_Voltage,
                 Poids_Peson(0),Poids_Peson(1),Poids_Peson(2),Poids_Peson(3),TareTemp(1),CompTemp(1),Data_LoRa.Brightness);
debugSerial.println(serialbuf); 
}


// jauges de contrainte de J01 à J15
// 1: float valini = 180116;
// valini = scale.read_average(20);  // sans ruche sur balance
// 2: float Echelle = 108.5;   // V
// Echelle = Val20kg - Valini / 20 (en g ou 20000 en KG)
// debugSerial.print("poids calculé : ");
// debugSerial.println((scale.read_average(20)- valini )/Echelle
 
// ---------------------------------------------------------------------------*
// Calcule le coefficient de mise à l'echelle val peson => kg
// valeurs non sauvegardées, à noter en table des jauges.   
// in : num (de la balance 1..4), poids_en_grammes (sur la balance)
// out :  coefficient de mise à l'echelle                          
// ---------------------------------------------------------------------------*
float Set_Scale_Bal(char num, float poids_en_grammes)    // N° de jauges des balances 1 à 4
{ float pesee, valAVid, Echelle, valEnCharge;

 // num--;
  debugSerial.print("Bal. num : "); debugSerial.print((int)num);
  debugSerial.print(" Peson : J"); debugSerial.println(Peson[ConfigMateriel.Num_Carte][num]);

  // marche plus nouvelle version!
  scale.begin(balance[num][0], balance[num][1]); // DOUT, SCK
  valAVid = abs(scale.read_average(20));                   // sans charge
  debugSerial.print("Val a vide : "); debugSerial.println(valAVid);
  //valeur à saisir ou en //metre !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  debugSerial.println("poser poids xxxxxg (15s)");
  delay(15000);

  //  pesee = scale.get_units(HX711_NbLect);
  valEnCharge = abs(scale.read_average(20));              // en charge
  debugSerial.print("val en chargeue : "); debugSerial.println(valEnCharge);
  Echelle = (valEnCharge - valAVid) / poids_en_grammes;  //
  debugSerial.println("Mise à echelle faite");
  debugSerial.print("Echelle : "); debugSerial.println(Echelle);
  debugSerial.print("poids calculé : ");
  Contrainte_List [num] = (abs(scale.read_average(20)));
  debugSerial.println((Contrainte_List [num] - valAVid ) / Echelle);

  scale.power_down();              // put the ADC in sleep mode

  Jauge[Peson[ConfigMateriel.Num_Carte][num]][0] = valAVid; // tera
  Jauge[Peson[ConfigMateriel.Num_Carte][num]][1] = Echelle; // scale 
  return ((Contrainte_List [num] - valAVid ) / Echelle);
}

// lancer 10 x au setup.
// ---------------------------------------------------------------------------*
// Retourne val non compensée                             
// in  : Numéro de la jauge de contrainte (1..3)
// out : 0 = Pas de jauge enregistrée; valeur jauge, -1 = jauge enr/err HX711
//
// Formule de compensation
//  Wcomp=Wraw(1−c(Tcal−Tsc)) where :
//   Wraw : raw weight values from scale
//      c : compensation coefficient (I use 0.0002 for the same scale/ADC setup)
//   Tcal : Temperature at calibration time (slighly below 20°C)
//    Tsc : Temperature of scale (put the sensor close to the scale)
//
//
// #define debugSerialGetPoids  // decommenter pour les messages debugSerial
// ---------------------------------------------------------------------------*
float GetPoids(int num,int sample)    // N° de jauges des balances 1 à 4
{ int i;
  static long readSample[4][11] = {{0,0,0,0,0,0,0,0,0,0,0},
                                   {0,0,0,0,0,0,0,0,0,0,0}, 
                                   {0,0,0,0,0,0,0,0,0,0,0}, 
                                   {0,0,0,0,0,0,0,0,0,0,0}}; // 10 lect. [0..9]; moyenne dans [10]
  static int numSample[4] = {0,0,0,0};

  num--;
  if (!Peson[ConfigMateriel.Num_Carte][num])             // pas de balance connectée
  {
#ifdef debugSerialGetPoids    
  debugSerial.println(" Peson : N/A");
#endif
    return (0); // pas de jauge déclarée        // sortie code retour 0
  }
// Initialisation du HX711
  scale.begin(balance[num][0], balance[num][1]); // DOUT, SCK

  if (scale.wait_ready_timeout(1000))           // lecture HX711 OK
  {
    for (i=0; i<sample; i++)
    { 
      readSample[num][numSample[num]] = scale.read();
      numSample[num]++;
      if (numSample[num] == 10)
        numSample[num] = 0;
delay (10);
    }
  }
/*
#ifdef debugSerialGetPoids
    sprintf(serialbuf," Bal. num: %d,  Peson num : %d reading: %d , sample %d",
            num+1,Peson[Ruche.Num_Carte][num],readSample[num][numSample[num]],numSample[num]); 
    debugSerial.println(serialbuf);
//    sprintf(serialbuf," Bal. num: %d,  Peson num : %d",num+1,Peson[Ruche.Num_Carte][num]); 
//    debugSerial.println(serialbuf);
  sprintf(serialbuf," HX711 reading: %d , sample %d", readSample[num][i], i);
  debugSerial.println(serialbuf);
#endif 
*/
 #ifdef debugSerialGetPoids
  else 
  {
    debugSerial.println("HX711 not found.");
  }
#endif 
 
 scale.power_down();  

// recalcul de la moyenne
  readSample[num][10] = 0;
  for (i=0; i<10; i++)
  {
    readSample[num][10] += readSample[num][i];
#ifdef debugSerialGetPoids
  sprintf(serialbuf," %d ", readSample[num][i]);
  debugSerial.print(serialbuf);
#endif     
  }  
  readSample[num][10] = readSample[num][10]/10;  
#ifdef debugSerialGetPoids
  sprintf(serialbuf," M %d ", readSample[num][10]);
  debugSerial.println(serialbuf);
#endif     


// Data_LoRa.HX711Weight[num]=(float)readSample[num][10];  utiliser le return de la fonction.

#ifdef debugSerialGetPoids
//  sprintf(serialbuf," HX711 average: %d  %4.1f", readSample[num][10],Data_LoRa.HX711Weight[num]);
//  debugSerial.println(serialbuf);
#endif 

#ifdef debugSerialGetPoids
 // debugSerial.print(" poids : "); debugSerial.println(pesee);
#endif  

// Il est preferable de memoriser en structure la donnée la moins traitées possible.
return(readSample[num][10]);   // valeur du peson brute moyennée sur les 10 dernières mesures
}


// ---------------------------------------------------------------------------*
//
//
//                 TEMP AUTRES
//
// ---------------------------------------------------------------------------*

// température interne SAMD21G18 + MCP9700AT + SF + RN2483

// String getTemperature(void);

// ---------------------------------------------------------------------------*
//  Lecture temp µC                             
//
//#define debugSerialgetTemperature  // decommenter pour les messages debugSerial
// ---------------------------------------------------------------------------*
float getTemperature()
{
#ifdef EXPLORER
  //10mV per C, 0C is 500mV
  float mVolts = (float)analogRead(TEMP_SENSOR) * 3300.0 / 1023.0;
float temp = (mVolts - 500.0) / 10.0;
#ifdef debugSerialgetTemperature
  sprintf(serialbuf, "%4.1f °C", temp);
  debugSerial.print(" TEMP_SENSOR : "); debugSerial.println(serialbuf);
#endif  
  return(temp);
#endif
  return(TEMP_ERR);
}

// /////////////////////////////////////////////////////////////////////////////
//   L       DDD     RRRR
//   L       D   D   R   R
//   L       D   D   RRRR
//   L       D   D   R  R
//   LLLLL   DDDD    R   R
// /////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------*
// Le capteur fonctionne selon le principe : 10mV par °C, où 0°C correspond à 500mV         
// ---------------------------------------------------------------------------*
float getLuminance(void) //
{
  //10mV per C, 0C is 500mV
 // CS_File1("LDR.start" , "" , "" , "");
  float mVolts = (float)analogRead(LUM_SENSOR) / 10.23; // echelle 0 à 100
  float lum = (mVolts * VLumScale_List[ConfigMateriel.Num_Carte]);
  sprintf(serialbuf, "%4.1f %%", lum);
//  debugSerial.print(" LDR : "); debugSerial.println(serialbuf);
 // CS_File1("LDR.end" , "" , "" , "");
  return (lum);
}


// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
void fonctiondemerdetoggle(int port)  // ??????????????????????????????????????????????????????????????????????????????????????????????
{ static bool state = 0;

  state = ! state;
  digitalWrite(port, state);
}

// /////////////////////////////////////////////////////////////////////////////
//   TTTTT  EEEEE  N   N   SSSS  III   OOO   N    N   SSSS
//     T    E      NN  N  S       I   O   O  NN   N  S
//     T    EEE    N N N   SSS    I   O   O  N  N N   SSS
//     T    E      N  NN      S   I   O   O  N   NN      S
//     T    EEEEE  N   N  SSSS   III   OOO   N    N  SSSS
// /////////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
float getVBatMoy(void)
{ static char v = 0;  
  char i;
  
  digitalWrite(RD_VBAT, HIGH);       // Blocage FET
  VBat[v] = (float)analogRead(VBAT_MEASURE);   // on fait une mesure
  digitalWrite(RD_VBAT, LOW);       // Blocage FET
  VBat[v] *= 43 / 33; //43k/10k+33k aux tolerances des R près... et VGS
  VBat[v] *= VBatScale_List[ConfigMateriel.Num_Carte];  // on applique la résolution
 

  VBat[10]=0;
  for (i = 0; i < 10; i++)    // on recalcule la moyenne des 10 mesures
    VBat[10] += VBat[i];
  VBat[10] /= 10;
  if (++v > 9) 
    v = 0;
//          Affichage de controle
/*  
  sprintf(serialbuf, " VBat : %4.3f V", VBat[v - 1]);
  debugSerial.print(serialbuf);
  sprintf(serialbuf, " VBatMoy : %4.3f V", VBat[10]);
  debugSerial.println(serialbuf);
*/
 return (VBat[10]);
}

// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
float getVSolMoy(void)
{ char i;
  static char v = 0;

  VSol[v] = analogRead(VSOL_MEASURE) * VSolScale_List[ConfigMateriel.Num_Carte]; // on fait une mesure

  VSol[10] = 0;
  for (i = 0; i < 10; i++)    // on recalcule la moyenne des 10 mesures
    VSol[10] += VSol[i]; 
  VSol[10] /= 10;
  if (++v > 9) 
    v = 0;

 //          Affichage de controle
/*
  sprintf(serialbuf, " VSol : %4.2f V %d", VSol[v], v); // valeur lue
  debugSerial.print(serialbuf);
  sprintf(serialbuf, " VSolMoy : %4.2f V", VSol[10]); // moyenne courante
  debugSerial.println(serialbuf);
*/
  return (VSol[10]);   // moyenne courante
}


// ci dessous test non exploité.

// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
static void affiche_nombre(struct nombre n)  // ????????????????????????????????????????????????????????????????????
{
  if (n.entier)
    printf("%d\n", n.u.e);
  else if (n.flottant)
    printf("%f\n", n.u.f);
}


// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
int uniontest(void)  // ????????????????????????????????????????????????????????????????????????????????????????????
{
  struct nombre a = { 0 };
  struct nombre b = { 0 };

  a.entier = 1;
  a.u.e = 10;
  b.flottant = 1;
  b.u.f = 10.56;

  affiche_nombre(a);
  affiche_nombre(b);
  return 0;
}
