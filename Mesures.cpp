//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
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

void MESURESgetWeight()
{
  scaleA.power_up();
  delay(400);              // stabilisation obligatoire !
    
    if (Peson[config.materiel.Num_Carte][0])//  Poids_Peson(3) = GetPoids(4, 1);
    {
      Contrainte_List[0]= MESURESHX711GetStrainGauge(0,scaleA,AVR_10);  // renvoi la moyenne des 10 dernières mesures 
      HiveSensor_Data.HX711Weight[0] =  calculePoids(0); // kg     
    }
    if (Peson[config.materiel.Num_Carte][1]) //Poids_Peson(0) = GetPoids(1, 1);
    {
      Contrainte_List[1]= MESURESHX711GetStrainGauge(1,scaleB,AVR_10);  // renvoi la moyenne des 10 dernières mesures 
      HiveSensor_Data.HX711Weight[1] =  calculePoids(1); // kg     
    }
    if (Peson[config.materiel.Num_Carte][2]) //Poids_Peson(1) = GetPoids(2, 1);
    {
      Contrainte_List[2]= MESURESHX711GetStrainGauge(2,scaleC,10);  // renvoi la moyenne des 10 dernières mesures 
      HiveSensor_Data.HX711Weight[2] =  calculePoids(2); // kg     
    }
    if (Peson[config.materiel.Num_Carte][3]) //Poids_Peson(2) = GetPoids(3, 1);
    {
      Contrainte_List[3]= MESURESHX711GetStrainGauge(3,scaleD,10);  // renvoi la moyenne des 10 dernières mesures 
      HiveSensor_Data.HX711Weight[3] =  calculePoids(3); // kg     
    }
scaleA.power_down();
}



// ---------------------------------------------------------------------------*
//       indices des macros de 0 à 3 ou de 1 à 4????? manque de cohérence!           
// ---------------------------------------------------------------------------*
void MESUREStake_All_Measure()  // durée de la proc?
{

// ici coder les mesures pour envoi payload



  MESURESgetWeight();

/*
typedef struct   
{
  float   DHT_Temp;         // Temp DHT en °C    xx,x  Float
  float   DHT_Hum;          // Hum DHT en %      xx,x  Float
  float   Brightness;       // %Lum en LUX      xxxxx  uint16_t
  float   Bat_Voltage;      // Tension BAT en V     xx,xx  Float (uint16_t)
  float   Solar_Voltage;    // Tension BAT en V     xx,xx  Float (uint16_t)
  float   HX711Weight[4];    // masse 4 Ruche  en kg xxx,xx (précision affich. 10g)
  float   ProcessorTemp;    // temp µC, conservé en backup DHT22 
} HiveSensor_Data_t;   // LoRa_Data devient HiveSensor_Data 
*/


// DHT
char rc = read_DHT(dht); 

// µC
 // if (rc)   // si DHT pas lu, utilisation de température µC
    HiveSensor_Data.DHT_Temp = getTemperature();

// µC ANA
  HiveSensor_Data.Bat_Voltage = getVBatMoy();
  HiveSensor_Data.Solar_Voltage = getVSolMoy();

// LDR ANA
  HiveSensor_Data.Brightness = getLuminance();


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

// compensation de 2 vers 4 pour validation règle de correction
static float oldpoids2=0;

poidsBal_g(2) = poidsBal_g(2)-0.5 - (Temp_Peson(2)-TareTemp(2))*CompTemp(2); // decalé de 500g
if (oldpoids2 ==0)
  oldpoids2 = poidsBal_g(2);
  float delta = abs(poidsBal_g(2)-oldpoids2);
  sprintf(serialbuf, "%6.3f",poidsBal_g(2) );
debugSerial.println(serialbuf); 
  sprintf(serialbuf, "%6.3f",oldpoids2);
debugSerial.println(serialbuf); 
  sprintf(serialbuf, "%6.3f",delta);
debugSerial.println(serialbuf); 
  if ( delta < 0.1) // moyennage si variation inf à 100g
    poidsBal_g(2)=(oldpoids2 + poidsBal_g(2)) / 2;  // moyenné sur moyenne(2points) + nlle mesure.
  sprintf(serialbuf, "%6.3f",poidsBal_g(2) );
debugSerial.println(serialbuf); 
  oldpoids2 = poidsBal_g(2);  // memorise poids
  poidsBal_g(3) = poidsBal_g(1) - (Temp_Peson(1)-TareTemp(1))*CompTemp(1); 
//#endif // WEIGHT_YES
*/
LOG_DEBUG("Take_All_Measure est vide et ne mesure rien!!!!!!!!!!!!!!!!!!!!!");
LOG_DEBUG(" Temp  Hum   Vs   Vb    P1     P2     P3     P4   tare1 comp1  lum");
sprintf(serialbuf, "%04.2f %04.2f %04.2f %04.2f %06.2f %06.2f %06.2f %06.2f %04.2f %04.2f   %04.2f",
                 HiveSensor_Data.DHT_Temp,HiveSensor_Data.DHT_Hum,HiveSensor_Data.Solar_Voltage,HiveSensor_Data.Bat_Voltage,
                 poidsBal_g(0),poidsBal_g(1),poidsBal_g(2),poidsBal_g(3),TareTemp(1),CompTemp(1),HiveSensor_Data.Brightness);
LOG_DEBUG(serialbuf); 
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
float TODOHX711Set_Scale_Bal(char num, float poids_en_grammes)    // N° de jauges des balances 0 à 3
{ float pesee, valAVide, Echelle, valEnCharge;



// /!\ attention tout est à refaire pas ecran dédié

// 1 faire tare à vide (VALIDE action ou ABANDON 30s) => m04x_1F_tareBalx(); enregistre la tare et temperature
// 2 entrer valeur de tare => m04_1F_PoidsTare()  (chiffre entre 0 et 100000)
// 3 Faire mise à l'echelle (VALIDE action ou ABANDON 30s) => m04x_2F_echBalx(); calcule la mise à l'echelle


/*
  debugSerial.print("Bal. num : "); debugSerial.print((int)num);
  debugSerial.print(" Peson : J"); debugSerial.println(Peson[config.materiel.Num_Carte][num]);

  // marche plus nouvelle version!

switch (num)      // est sencé être fait
{
  case 0 :  scaleA.power_up();

    //scale.begin(config.materiel.HX711Dta_0, config.materiel.HX711Clk_0); // DOUT, SCK
           break;
  case 1 :  scaleB.power_up();
    //scale.begin(config.materiel.HX711Dta_1, config.materiel.HX711Clk_1); // DOUT, SCK
           break;
  case 2 :  scaleC.power_up();
    //scale.begin(config.materiel.HX711Dta_2, config.materiel.HX711Clk_2); // DOUT, SCK
           break;
  case 3 :  scaleD.power_up();
    //scale.begin(config.materiel.HX711Dta_3, config.materiel.HX711Clk_3); // DOUT, SCK
           break;
  default : // WTF
           break;         
}

// factoriser  
if (!scale.wait_ready_timeout(2000))  // 2 secondes de timeout
{
  debugSerial.print("ERREUR: HX711 #");
  debugSerial.print(num);
  debugSerial.println(" non détecté");
  scale.power_down();
  return 0.0;  // Sortir de la fonction
}
  valAVide = abs(scale.read_average(20));                   // sans charge
  debugSerial.print("Val a vide : "); debugSerial.println(valAVide);
  //valeur à saisir ou en //metre !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  debugSerial.println("poser poids xxxxxg (15s)");
  delay(15000);

  //  pesee = scale.get_units(HX711_NbLect);
  valEnCharge = abs(scale.read_average(20));              // en charge
  debugSerial.print("val en charge : "); debugSerial.println(valEnCharge);
  Echelle = (valEnCharge - valAVide) / poids_en_grammes;  //
  debugSerial.println("Mise à echelle faite");
  debugSerial.print("Echelle : "); debugSerial.println(Echelle);
  debugSerial.print("poids calculé : ");
  Contrainte_List [num] = (abs(scale.read_average(20)));
  debugSerial.println((Contrainte_List [num] - valAVide) / Echelle);

  scale.power_down();              // put the ADC in sleep mode

//  pesonTare(num) = valAVide; // tare
//  pesonScale(num) = Echelle; // scale 

  return ((Contrainte_List [num] - valAVide) / Echelle);
//  #define poidsBal_kg(num)  (Contrainte_List[num]-pesonTare(num))/pesonScale(num)/1000 // kg
*/
}


// ---------------------------------------------------------------------------*
// Retourne le poids en kg                            
// in  : Numéro de la jauge de contrainte (0..3)
// out : valeur poids en kg
//
// ---------------------------------------------------------------------------*
float calculePoids(int numJauge)
{
  return(poidsBal_kg(numJauge));
}

// lancer 10 x au setup.
// ---------------------------------------------------------------------------*
// Retourne val non compensée                             
// in  : Numéro de la jauge de contrainte (0..3)
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
// ---------------------------------------------------------------------------*
float MESURESHX711GetStrainGauge(int numJauge,HX711 scale,int sample)    // N° de jauges des balances 0 à 3
{ long readSample; 

  if (!Peson[config.materiel.Num_Carte][numJauge])             // pas de balance connectée
    return (0); // pas de jauge déclarée        // sortie code retour 0
//  scale.power_up();
  readSample = (long)scale.read_average(sample);
//  scale.power_down();  
 //  debugSerial.print(numJauge); debugSerial.println((char)(65+numJauge));  // affiche A/B/C/D

  if (logPeson)
  { char msg[200];
    snprintf(msg,80,"Jauge: %c Peson: %d average: %d Tare: %.0f Scale: %.0f poids: %f",
              numJauge+65,Peson[config.materiel.Num_Carte][numJauge],
              readSample,pesonTare(numJauge), pesonScale(numJauge), calculePoids(numJauge)); 
    LOG_DEBUG(msg); 
   }
   return(readSample);   // valeur du peson brute moyennée sur les 10  mesures 
}


// lancer 10 x au setup.
// ---------------------------------------------------------------------------*
// Retourne val non compensée                             
// in  : Numéro de la jauge de contrainte (0..3)
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
// ---------------------------------------------------------------------------*
float nonMESURESHX711GetStrainGauge(int numJauge,HX711 scale,int sample)    // N° de jauges des balances 0 à 3
{ int i;                                   // setup sample = 10, 1 sinon 
  static long readSample[4][11] = {{0,0,0,0,0,0,0,0,0,0,0},
                                   {0,0,0,0,0,0,0,0,0,0,0}, 
                                   {0,0,0,0,0,0,0,0,0,0,0}, 
                                   {0,0,0,0,0,0,0,0,0,0,0}}; // 10 lect. [0..9]; moyenne dans [10]
  static int numSample[4] = {0,0,0,0};

  if (!Peson[config.materiel.Num_Carte][numJauge])             // pas de balance connectée
    return (0); // pas de jauge déclarée        // sortie code retour 0
//  scale.power_up();
    for (i=0; i<sample; i++)          // Lecture du Nb echantillon prévu
    { 
      readSample[numJauge][numSample[numJauge]] = scale.read();
      numSample[numJauge]++;
      if (numSample[numJauge] == 10)
        numSample[numJauge] = 0;
    }
// scale.power_down();  
// recalcul de la moyenne                                         
  readSample[numJauge][10] = 0;
  for (i=0; i<10; i++)
    readSample[numJauge][10] += readSample[numJauge][i];
  readSample[numJauge][10] = readSample[numJauge][10]/10;  

  if (logPeson)
  {
    if (numJauge == 0)
    {   
      sprintf(serialbuf,"tatata Jauge: %c Peson: %d reading: %d  sample: %d, ",
              numJauge+65,Peson[config.materiel.Num_Carte][numJauge],readSample[numJauge][numSample[numJauge]/*-1*/],numSample[numJauge]); 
      debugSerial.print(serialbuf);         // readSample[numJauge][numSample[numJauge]]
      sprintf(serialbuf," average: %d Tare: %.0f Scale: %.0f", 
                          readSample[numJauge][10],pesonTare(numJauge), pesonScale(numJauge)); 
      debugSerial.println(serialbuf);                                        
    }
  }
    
   return(readSample[numJauge][10]);   // valeur du peson brute moyennée sur les 10 dernières mesures
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
  float lum = (mVolts * VLumScale_List[config.materiel.Num_Carte]);
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
  VBat[v] *= VBatScale_List[config.materiel.Num_Carte];  // on applique la résolution
 
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

// rendre local les 10+1 mesures


  VSol[v] = analogRead(VSOL_MEASURE) * VSolScale_List[config.materiel.Num_Carte]; // on fait une mesure

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

/*
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
*/
