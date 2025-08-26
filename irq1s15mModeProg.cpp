
/*


La solution 1 (multiplexage temporel) est la plus simple et fiable :

Une seule sortie DS3231 utilisée
Compteur logiciel précis pour 15 minutes
Code simple et maintenable
Pas de composants externes

ci dessous en mode prog : 

volatile bool rtcInterrupt = false;
volatile unsigned long secondCount = 0;

void setup() {
    Serial.begin(9600);
    
    if (!rtc.begin()) {
        Serial.println("RTC non trouvé");
        while(1);
    }
    
    // Configuration pour SQW 1Hz
    configureDS3231_1Hz();
    
    // Une seule interruption physique (IRQ1)
    pinMode(3, INPUT_PULLUP);  // SQW/INT → Pin 3 (IRQ1)
    attachInterrupt(1, rtcISR, FALLING);
    
    Serial.println("DS3231 configuré: 1Hz sur IRQ1, compteur 15min logiciel");
}

void configureDS3231_1Hz() {
    Wire.beginTransmission(0x68);
    Wire.write(0x0E); // Control register
    Wire.write(0x00); // INTCN=0, SQW=1Hz
    Wire.endTransmission();
}

// Une seule ISR physique
void rtcISR() {
    rtcInterrupt = true;
    secondCount++;
}

void loop() {
    if (rtcInterrupt) {
        rtcInterrupt = false;
        
        // Action chaque seconde (simule IRQ1)
        handleSecondTick();
        
        // Action toutes les 15 minutes (simule IRQ0)
        if (secondCount >= 900) { // 15 * 60 = 900 secondes
            secondCount = 0;
            handle15MinTick();
        }
    }
}

*/
