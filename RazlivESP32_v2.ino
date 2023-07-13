// #include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>

#define RELAY_STATE_ON 0
#define KEY_GOOD 44
#define EEPROM_SYZE 40
#define COUNT_RELAY_TWO 2//6
//Дефайны адрессов EEPROM памяти
#define ADDR_KEY_FIRST 				3
#define ADDR_KEY_CHANGE 			4
#define ADDR_DATCHIC_TIMER 			5
#define ADDR_COUNT_TICK_DATCHIC 	6
#define ADDR_COUNT_TIMER_RELAY 		7
#define ADDR_COUNT_TIMER_DELAY 		11
#define ADDR_RELAY_DATCHIC_STATE 	21

#define TICK_DELAY_MILSEC 350

#define PIN_TICK_DATCHIC 25
#define PIN_CONVEER_RELAY 13
#define PIN_NASOS_RELAY 23
#define PIN_BUTTON_ON 26
#define PIN_BUTTON_OFF 27

uint8_t GPIO_relay_datchic[COUNT_RELAY_TWO][2] = {{18,34}, {19,35}}; //{{16, 39},{17, 36},{18, 35},{19, 34},{21, 33},{22, 32}};                //18 и 19 - релле, 34 и 35 - датчики

const char* ssid     = "ESP32-Rozliv";
const char* password = "1234";
String header;
WiFiServer server(80);
 
uint8_t jobState = 0;
float TimeRelay = 5.4;
float TimeDelay = 1.1;
uint32_t TimeIntRelay = 0;
uint32_t TimeIntDelay = 0;
uint8_t datchicTimerState = 0;
uint8_t countTickDatchic = 7;
uint8_t byteFlagDatchic = 0;
uint8_t byteDoubleDatchic = 0;
uint32_t timerRelay;
uint8_t flagChangeEEPROM = 1;

// HTML web page elements
const char indexHead_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
<style>
html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
h1 {font-size: 38px;}
h2 {margin: 3px 10px; font-size: 20px;}
h3 {margin: 10px 3px 20px 3px;}
.button {background-color: #4CAF50; border: none; border-radius: 20px; color: white; padding: 16px 36px;text-decoration: none; font-size: 30px; margin: 0px 5px 22px 5px; cursor: pointer;}
.button2 {background-color: #555555;}
.buttonString {font-size: 12px; padding: 1px 36px;}
.counterCheck {font-size: 20px; margin: 2px 1px; cursor: auto;}
.counterCheck2 {padding: 14px 45px;font-size: 20px;} 
.inctrMargin {margin: auto;font-size: 20px;}
.inctrRelay {display: inline-flex; background-color: #aaac93; color: rgb(0, 0, 0); padding: 10px 20px;font-size: 12px; margin: 8px 1px; cursor: auto;}
.submit {padding: 18px auto;}
.button3 {background-color: #aaac93; color: rgb(0, 0, 0);}
</style>
</head>
<body>
<h1>ESP32 Setup "Rozliv"</h1>      
<h2>Источник управления</h2>
<p>
<a href="/datchicOn">
<button class="button)rawliteral";
//выбор раскраса кнопки датчик или таймер серый
const char indexTickCountStart_html[] PROGMEM = R"rawliteral(
Timer</button>
</a>
</p>
<h2>Релле счета (№7 GPIO-25)</h2>
<h3>Колличество срабатываний датчика :</h3>

<form method="get" action="/form">
<input type="number" size="3" step="1" name="coDa" min="1" max="10" class="button inctrMargin submit button3" placeholder=")rawliteral";
//вставить значение количества тиков датчика
const char indexTickCountEnd_html[] PROGMEM = R"rawliteral(
"/>
<input type="submit" class="button inctrMargin submit" value="Отправить" />
</form> 

)rawliteral";
const char indexTimerRellayStart_html[] PROGMEM = R"rawliteral(
<h2>Установка таймера</h2>
<h3>Установка таймера отключения релле на розлив :</h3>
<form method="get" action="/form">
<input type="number" size="3" step="0.1" name="tiRe" min="0.1" max="99.0" class="button inctrMargin submit button3" placeholder=")rawliteral";
//вставить значение таймера релле
const char indexTimerRellayDelay_html[] PROGMEM = R"rawliteral(
"/>
<input type="submit" class="button inctrMargin submit" value="Отправить" />
</form>    
</p>
<h3>Установка таймера включения конвейера :</h3>
<form method="get" action="/form">
<input type="number" size="3" step="0.1" name="tiDe" min="0.1" max="99.0" class="button inctrMargin submit button3" placeholder=")rawliteral";
//вставить значение таймера задержки
const char indexTimerDelay_html[] PROGMEM = R"rawliteral(
"/>
<input type="submit" class="button inctrMargin submit" value="Отправить" />
</form><p></p> )rawliteral";
const char indexRelaysConstr1_html[] PROGMEM = R"rawliteral(
<div class="button inctrRelay">
<div class="inctrMargin"><h2>Релле GPIO-)rawliteral";
//вставить GPIO-порт релле + " и датчик GPIO-" + GPIO-порт датчика
const char indexRelaysConstr2_html[] PROGMEM = R"rawliteral(
</h2></div>
<a href="/relays/)rawliteral";
//вставить номер пары релле-датчик с 1
const char indexRelaysConstr3_html[] PROGMEM = R"rawliteral(
">
<button class="button inctrMargin)rawliteral";
//вставить состояние пары ON or OFF
const char indexRelaysConstr4_html[] PROGMEM = R"rawliteral(
</button>
</a>
</div>)rawliteral";
//повторить конструктор по количеству пар 
const char indexEnd_html[] PROGMEM = R"rawliteral(
</body>
</html>)rawliteral";

void FirstEEPROM() {
	if (EEPROM.read(ADDR_KEY_FIRST) == KEY_GOOD) return;
	for (uint8_t s = 0; s < EEPROM_SYZE; s++) EEPROM.write(s, 0);
	EEPROM.commit();
	EEPROM.put(ADDR_KEY_FIRST, KEY_GOOD);
	EEPROM.put(ADDR_COUNT_TICK_DATCHIC, 7);
	EEPROM.put(ADDR_COUNT_TIMER_RELAY, TimeRelay);
	EEPROM.put(ADDR_COUNT_TIMER_DELAY, TimeDelay);
  TimeIntRelay = (uint32_t)TimeRelay * 10;
  TimeIntRelay *= 100;
  TimeIntDelay = (uint32_t)TimeDelay * 10;
  TimeIntDelay *= 100;
	for (uint8_t e = 0; e < COUNT_RELAY_TWO; e++) {
    bitSet(byteDoubleDatchic, e);
	}
	EEPROM.put(ADDR_RELAY_DATCHIC_STATE, byteDoubleDatchic);
	EEPROM.commit();
}

String PageConstructor1() {
	String page1 = "";
	page1 += indexHead_html;
	if (datchicTimerState) page1 += " button2\">Datchic</button></a><a href=\"/timerOn\"><button class=\"button\">";//к строке добавить серый окрас для таймера, таймер активный
	else page1 += "\">Datchic</button></a><a href=\"/timerOn\"><button class=\"button  button2\">";//к строке добавить серый окрас для датчика
	page1 += indexTickCountStart_html;
	page1 += countTickDatchic;
	page1 += indexTickCountEnd_html;
	return page1;
}

String PageConstructor2() {
	String timeReS(TimeRelay,1);
	String timeDeS(TimeDelay,1);
	String page2 = "";
	if (datchicTimerState) {
		page2 += indexTimerRellayStart_html;
		page2 += timeReS;
		page2 += indexTimerRellayDelay_html;
		page2 += timeDeS;
		page2 += indexTimerDelay_html;
	}
	for (uint8_t r = 1; r <= COUNT_RELAY_TWO; r++) {
		page2 += indexRelaysConstr1_html;
		page2 += GPIO_relay_datchic[r-1][0];
		page2 += " и датчик GPIO-";
		page2 += GPIO_relay_datchic[r-1][1];
		page2 += indexRelaysConstr2_html;
		page2 += r;
		if (bitRead(EEPROM.read(ADDR_RELAY_DATCHIC_STATE), r-1)) {
			page2 += "/off";
			page2 += indexRelaysConstr3_html;
			page2 += "\">ON";
		}	
		else {
			page2 += "/on";
			page2 += indexRelaysConstr3_html;
			page2 += " button2\">OFF";
		}
		page2 += indexRelaysConstr4_html;
		page2 += indexEnd_html;
	}
	return page2;
}

void EEPROM_update() {
  EEPROM.get(ADDR_DATCHIC_TIMER, datchicTimerState);
	EEPROM.get(ADDR_COUNT_TICK_DATCHIC, countTickDatchic);
	EEPROM.get(ADDR_COUNT_TIMER_RELAY, TimeRelay);
	EEPROM.get(ADDR_COUNT_TIMER_DELAY, TimeDelay);
  TimeIntRelay = (uint32_t)TimeRelay * 10;
  TimeIntRelay *= 100;
  TimeIntDelay = (uint32_t)TimeDelay * 10;
  TimeIntDelay *= 100;
  EEPROM.get(ADDR_RELAY_DATCHIC_STATE, byteDoubleDatchic);
	EEPROM.commit();
}

void setup() {
	Serial.begin(115200);
	EEPROM.begin(EEPROM_SYZE);
	FirstEEPROM();
  EEPROM_update();
  pinMode(PIN_CONVEER_RELAY, OUTPUT);
  digitalWrite(PIN_CONVEER_RELAY, !RELAY_STATE_ON);
  pinMode(PIN_NASOS_RELAY, OUTPUT);
  digitalWrite(PIN_NASOS_RELAY, !RELAY_STATE_ON);
  pinMode(PIN_TICK_DATCHIC, INPUT);
  pinMode(PIN_BUTTON_OFF, INPUT);
  pinMode(PIN_BUTTON_ON, INPUT);
	for (uint8_t p = 0; p < COUNT_RELAY_TWO; p++) {
		pinMode(GPIO_relay_datchic[p][0], OUTPUT);
		pinMode(GPIO_relay_datchic[p][1], INPUT);
		digitalWrite(GPIO_relay_datchic[p][0], !RELAY_STATE_ON);
	}
	
	WiFi.mode(WIFI_AP);
	WiFi.softAP(ssid, password);

	Serial.print("Setting AP (Access Point)…");

	WiFi.softAP(ssid, password);
	
	IPAddress IP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(IP);
	
	server.begin();
}

uint8_t ButtonSearch(uint8_t On_Off, uint8_t pinCount) {
  uint8_t countDuo = 0;
  uint8_t count = 0;
  while(count < 50) {
      count++;
      if (digitalRead(pinCount)) countDuo++;
  }
  if (On_Off) {
      if (countDuo > 45) return(1);
      else return(0);
  }
  else {
      if (countDuo < 5) return(1);
      else return(0);
  }
}

void Job(){
  static uint8_t timeFlag;
  static uint8_t tickCount;
  static uint8_t tickFlag;
  static uint32_t tickTimer;
  if (jobState && ButtonSearch(1,PIN_BUTTON_OFF)) {
    digitalWrite(PIN_NASOS_RELAY, !RELAY_STATE_ON);
    digitalWrite(PIN_CONVEER_RELAY, !RELAY_STATE_ON);
    for (uint8_t rel = 0; rel < COUNT_RELAY_TWO; rel++) {
      digitalWrite(GPIO_relay_datchic[rel][0], !RELAY_STATE_ON);
    }
    jobState = 0;
  }
  else if (jobState == 0 && ButtonSearch(1,PIN_BUTTON_ON)) {
    jobState = 1;
    digitalWrite(PIN_CONVEER_RELAY, RELAY_STATE_ON);
    tickCount = 0;
    tickFlag = 1;
  }
  if (jobState == 1) {
    if (tickFlag == 0 && tickTimer <= millis() && ButtonSearch(1, PIN_TICK_DATCHIC)) tickFlag = 1;
    if (tickFlag && ButtonSearch(0, PIN_TICK_DATCHIC)) {
      tickTimer = millis() + TICK_DELAY_MILSEC;
      tickFlag = 0;
      tickCount++;
    }
    if (tickCount >= countTickDatchic) {
      digitalWrite(PIN_CONVEER_RELAY, !RELAY_STATE_ON);
      for (uint8_t reOn = 0; reOn < COUNT_RELAY_TWO; reOn++) {if (bitRead(byteDoubleDatchic, reOn)) digitalWrite(GPIO_relay_datchic[reOn][0], RELAY_STATE_ON);}
      digitalWrite(PIN_NASOS_RELAY, RELAY_STATE_ON);
      timerRelay = millis() + TimeIntRelay;
      timeFlag = 0;
      jobState = 2;
    }
  }
  if (jobState == 2) {
    if (datchicTimerState) {
      if (timeFlag == 1 && timerRelay + TimeIntDelay <= millis()) {
        timeFlag = 2;
        tickCount = 0;
        tickFlag = 1;
        jobState = 1;
        digitalWrite(PIN_CONVEER_RELAY, RELAY_STATE_ON);
      }
      else if (timeFlag == 0 && timerRelay <= millis()) {
        timerRelay = millis() + TimeIntDelay;
        timeFlag = 1;
        digitalWrite(PIN_NASOS_RELAY, !RELAY_STATE_ON);
        for (uint8_t reOff = 0; reOff < COUNT_RELAY_TWO; reOff++) digitalWrite(GPIO_relay_datchic[reOff][0], !RELAY_STATE_ON);
      }
    }
    else{
      uint8_t DatCounter = 0;
      for (uint8_t sigDat = 0; sigDat < COUNT_RELAY_TWO; sigDat++) {
        if (!bitRead(byteDoubleDatchic, sigDat)) DatCounter++;
        else if(bitRead(byteDoubleDatchic, sigDat) != bitRead(byteFlagDatchic, sigDat) && ButtonSearch(1, GPIO_relay_datchic[sigDat][1])) {
          bitSet(byteFlagDatchic, sigDat);
          digitalWrite(GPIO_relay_datchic[sigDat][0], !RELAY_STATE_ON);
          DatCounter++;
        }
        else if (bitRead(byteDoubleDatchic, sigDat) == bitRead(byteFlagDatchic, sigDat)) DatCounter++;
      }
      if (DatCounter == COUNT_RELAY_TWO) {
        digitalWrite(PIN_NASOS_RELAY, !RELAY_STATE_ON);
        for (uint8_t reOff = 0; reOff < COUNT_RELAY_TWO; reOff++) digitalWrite(GPIO_relay_datchic[reOff][0], !RELAY_STATE_ON);
        timeFlag = 2;
        byteFlagDatchic = 0;
        tickCount = 0;
        tickFlag = 1;
        jobState = 1;
        digitalWrite(PIN_CONVEER_RELAY, RELAY_STATE_ON);
      }
    }
  }
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                              // If a new client connects,
    // String header = "";
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {           // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          
          if (header.indexOf("GET /datchicOn") >= 0) {
            datchicTimerState = 0;
            if (EEPROM.read(ADDR_DATCHIC_TIMER) != 0) {
              EEPROM.put(ADDR_DATCHIC_TIMER, 0);
              EEPROM.commit();
            }
            Serial.println("datchic - On");

          } else if (header.indexOf("GET /timerOn") >= 0) {
            datchicTimerState = 1;
            if (EEPROM.read(ADDR_DATCHIC_TIMER) != 1) {
              EEPROM.put(ADDR_DATCHIC_TIMER, 1);
              EEPROM.commit();
            }
            Serial.println("timer - On");

          } else if (header.indexOf("GET /form?coDa=") >= 0) {
            header.remove(0, header.indexOf('=') + 1);     
            header.remove(header.indexOf("HTTP") - 1);
            countTickDatchic = (uint8_t) header.toInt();
            EEPROM.put(ADDR_COUNT_TICK_DATCHIC, countTickDatchic);
            EEPROM.commit();
            Serial.print("countTickDatchic =");
            Serial.println(countTickDatchic);

          } else if (header.indexOf("GET /form?tiRe=") >= 0) {
            header.remove(0, header.indexOf('=') + 1);     
            header.remove(header.indexOf("HTTP") - 1);
            TimeRelay = header.toFloat();
            TimeRelay *= 10;
            TimeIntRelay = (uint32_t)TimeRelay;
            TimeRelay /= 10;
            TimeIntRelay *= 100;
            Serial.println(TimeIntRelay);

          } else if (header.indexOf("GET /form?tiDe=") >= 0) {
            header.remove(0, header.indexOf('=') + 1);     
            header.remove(header.indexOf("HTTP") - 1);
            TimeDelay = header.toFloat();
            TimeDelay *= 10;
            TimeIntDelay = (uint32_t)TimeDelay;
            TimeDelay /= 10;
            TimeIntDelay *= 100;
            Serial.println(TimeIntDelay);


          } else if (header.indexOf("GET /relays") >= 0) {
            for (uint8_t k = 1; k <= COUNT_RELAY_TWO; k++) {
              String zapr = "/relays/";
              zapr += k;
              zapr += '/';
              if (header.indexOf(zapr) >= 0) {
                zapr += "off";
                if (header.indexOf(zapr) >= 0) {
                  if (bitRead(EEPROM.read(ADDR_RELAY_DATCHIC_STATE), k-1)) {
                    bitClear(byteDoubleDatchic, k-1);
                    EEPROM.put(ADDR_RELAY_DATCHIC_STATE, byteDoubleDatchic);
                    EEPROM.commit();
                    Serial.println("OFFFFFFFFFFFFFF");
                  }
                }
                else {
                  if (bitRead(EEPROM.read(ADDR_RELAY_DATCHIC_STATE), k-1) != 1) {
                    bitSet(byteDoubleDatchic, k-1);
                    EEPROM.put(ADDR_RELAY_DATCHIC_STATE, byteDoubleDatchic);
                    EEPROM.commit();
                    Serial.println("ONNNNNNNNNNNNN");
                  }
                }
              }
            }
          } 

          client.println(PageConstructor1());
          client.println(PageConstructor2());
          
          client.println();
          // Break out of the while loop
          break;
        } else { // if you got a newline, then clear currentLine
          currentLine = "";
        }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          	currentLine += c;      // add it to the end of the currentLine
        }
      }
      Job();
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
    Job();
  }
  Job();
}



