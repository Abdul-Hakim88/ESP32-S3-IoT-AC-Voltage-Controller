#define BLYNK_TEMPLATE_ID "TMPL2jJjRpiCR"
#define BLYNK_TEMPLATE_NAME "AC Voltage Controller"
#define BLYNK_AUTH_TOKEN "JQ-1ZAwYhOtt7DDLykOU33KT7STRmewM"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Your_WiFi_SSID";
char pass[] = "Your_WiFi_Password";

const int ZCD_PIN = 4;
const int TRIG1_PIN = 6;
const int TRIG2_PIN = 7;

volatile int powerLevel = 0;        
hw_timer_t *phaseTimer = NULL;

void IRAM_ATTR pulseTriac() {
  digitalWrite(TRIG1_PIN, HIGH);
  digitalWrite(TRIG2_PIN, HIGH);
  delayMicroseconds(20); 
  digitalWrite(TRIG1_PIN, LOW);
  digitalWrite(TRIG2_PIN, LOW);
}

void IRAM_ATTR onPhaseTimer() {
  pulseTriac();
}

void IRAM_ATTR zcd_isr() {
  if (powerLevel <= 0) return;
  if (powerLevel >= 100) {
    pulseTriac();
    return;
  }

  // Map 1-99% power to a 9200µs to 500µs delay based on 50Hz mains
  uint64_t delayUs = map(powerLevel, 1, 99, 9200, 500);

  timerAlarmWrite(phaseTimer, delayUs, false); 
  timerStart(phaseTimer);
}

// V0: Blynk Slider (0 to 100)
BLYNK_WRITE(V0) {
  powerLevel = constrain(param.asInt(), 0, 100);
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG1_PIN, OUTPUT);
  pinMode(TRIG2_PIN, OUTPUT);
  digitalWrite(TRIG1_PIN, LOW);
  digitalWrite(TRIG2_PIN, LOW);

  pinMode(ZCD_PIN, INPUT_PULLUP);

  // Initialize hardware timer at 1MHz (1 tick = 1µs)
  phaseTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(phaseTimer, &onPhaseTimer, true);
  attachInterrupt(digitalPinToInterrupt(ZCD_PIN), zcd_isr, FALLING);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
}