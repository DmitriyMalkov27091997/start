#define NORMAL_STATE 0 // нормальное состояние релле (когда реле выключена) 0 или 1

#define COUNT_RELAY 3   // количество релле 
#define FIX_BUTTON_PIN 13 // пин подключения тумблера (пин самой ESP8266, а не платы (смотреть по распиновке платы))
#define BUTTON_ON 0     // пин подключения кнопки включения (!!!___подтяжка к плюсу___!!!) (пин самой ESP8266, а не платы (смотреть по распиновке платы))
#define BUTTON_OFF 2    // пин подключения кнопки отключения (!!!___подтяжка к плюсу___!!!) (пин самой ESP8266, а не платы (смотреть по распиновке платы))

#define DELAY_RELAY 100 // задержка включений релле между собой (чтобы включались не одновременно) в милисекундах
#define INPUT_TIME 50  //время фиксации для устранения дребезга  

uint8_t PinRelayMass [COUNT_RELAY][3] = 
{
//  1-й столбец - пин релле, 2-й - пин датчика (кроме первой строки), 3-й - включить или выключить при включении тумблера
    {5, 0, 1},    // для релле без датчика       
    {4, 14, 0},     // для релле с датчиками 
    {15, 12, 1}
};
uint8_t sizeMas, stateRelay = 0, bufRelay = 0;
uint32_t tmrRelay;
uint8_t InputSearch (uint8_t pin, uint8_t state = 1) {
    static uint32_t tmrInput;
    tmrInput = millis() + INPUT_TIME;
    while (digitalRead(pin) == state) {if(tmrInput < millis()) return(1);}
    return(0);
}
void setup() {
    pinMode(FIX_BUTTON_PIN, INPUT);
    pinMode(BUTTON_ON, INPUT);
    pinMode(BUTTON_OFF, INPUT);
    sizeMas = sizeof(PinRelayMass)/3;
    for (uint8_t i = 0; i < sizeMas; i++) {
        pinMode(PinRelayMass[i][0], OUTPUT);
        digitalWrite(PinRelayMass[i][0], NORMAL_STATE);
        pinMode(PinRelayMass[i][1], INPUT);   
    } 
} 
void ModeControl() {
    if (stateRelay == bufRelay || tmrRelay > millis()) return;
    tmrRelay = millis() + DELAY_RELAY;
    if (stateRelay == 0) {
        if (bitRead(bufRelay, 0)) { digitalWrite(PinRelayMass[0][0], NORMAL_STATE); bitClear(bufRelay, 0); }
        else {
            for (uint8_t i = 1; i < sizeMas; i++) {
                if (bitRead(bufRelay, i)) { digitalWrite(PinRelayMass[i][0], NORMAL_STATE); bitClear(bufRelay, i); return; }
            }
        }
    }
    else {
        for (uint8_t i = 1; i < sizeMas; i++) {
            if (!bitRead(stateRelay, i) && bitRead(bufRelay, i)) { digitalWrite(PinRelayMass[i][0], NORMAL_STATE); bitClear(bufRelay, i); return; }
            if (bitRead(stateRelay, i) && !bitRead(bufRelay, i)) { digitalWrite(PinRelayMass[i][0], !NORMAL_STATE); bitSet(bufRelay, i); return; }
        }
        digitalWrite(PinRelayMass[0][0], !NORMAL_STATE);
        bitSet(bufRelay, 0);
    }
}
void loop() {
    if (InputSearch(BUTTON_OFF, 0)) stateRelay = 0;
    else if (InputSearch(BUTTON_ON, 0) && stateRelay == 0) {
        for (uint8_t bitS = 0; bitS < sizeMas; bitS++) {
            if (digitalRead(FIX_BUTTON_PIN)) bitWrite(stateRelay, bitS, PinRelayMass[bitS][2]);
            else bitSet(stateRelay, bitS);
        }
    }
    ModeControl();
    for (uint8_t i = 1; i < sizeMas; i++) {
        if (InputSearch(PinRelayMass[i][1]) && bitRead(bufRelay, i) && bitRead(stateRelay, i)) bitClear(stateRelay, i);
        if (stateRelay == 1) stateRelay = 0;
    }
}