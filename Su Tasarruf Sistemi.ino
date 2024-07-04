#define BLYNK_TEMPLATE_ID "TMPL6Od32Xz8q"
#define BLYNK_TEMPLATE_NAME "deneme"
#define BLYNK_AUTH_TOKEN "7iS_WEAWGRKONatYWjn2xXWtBScY7ZXV"

#include <BlynkSimpleEsp32.h>

char auth[] = "7iS_WEAWGRKONatYWjn2xXWtBScY7ZXV";
char ssid[] = "gece";
char pass[] = "12345678";

#define FLOW_SENSOR_PIN 4
#define RELAY_PIN 2

volatile unsigned long pulseCount = 0;
float flowRate = 0.0;
float totalLitres = 0.0;
unsigned long oldTime = 0;

BlynkTimer timer;

BLYNK_WRITE(V2) {
  int switchState = param.asInt();
  digitalWrite(RELAY_PIN, switchState);
}

void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);

  timer.setInterval(1000L, sendFlowData);
}

void sendFlowData() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - oldTime;

  if (elapsedTime > 0) {
    // Saniyede kaç litre su geçtiğini hesapla
    flowRate = ((1000.0 / elapsedTime) * pulseCount) / 7.5; // Sensörün K Faktörü: 7.5
    totalLitres += (flowRate / 60); // Toplam litre hesabı

    Blynk.virtualWrite(V1, flowRate);
    Blynk.virtualWrite(V0, totalLitres);

    Serial.print("Flow rate: ");
    Serial.print(flowRate);
    Serial.println(" L/min");

    Serial.print("Total: ");
    Serial.print(totalLitres);
    Serial.println(" L");

    oldTime = currentTime;
    pulseCount = 0;
  }
}

void loop() {
  Blynk.run();
  timer.run();
}