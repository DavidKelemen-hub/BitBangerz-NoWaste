#include <BlynkSimpleEsp8266.h>
#include "ESP8266WiFi.h"
#include <Servo.h>
#include "DHT.h"


#define blue_led D0   /* led_blue */
#define white_led D1  /* led_white */
#define yellow_led D2 /*led_yellow */
#define dht_dpin 0    //D3
#define red_led D4    /*led_red */
#define DHTTYPE DHT11
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLUJo4BWDv"
#define BLYNK_DEVICE_NAME "Hackathon"
#define BLYNK_AUTH_TOKEN "uLLfQZ3NZ9FJ3vkFly9pC4dX8a3621To"
#define ONE 1
#define ZERO 0

DHT dht(dht_dpin, DHTTYPE);
uint8_t SETPOINT_TEMPERATURE;

BlynkTimer timer;
BlynkTimer timeCount;



char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "HUAWEIwalter";
char pass[] = "walter2001";
char auxiliar;
int on_off_control;
int time_counter = 0;
int miliAmps_led;
int led_b, led_y, led_r, led_w;
float consumption_reduction;
int aux_b, aux_y, aux_r, aux_w;

/* 1-relay 2-servo_doors 3-led_band */

char output_data[4] = "000"; /* send output data to ATMega */
char input_data[1];          /* receive input data from ATMega */
/* ----------------------------------- */

void myTimer() {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Blynk.virtualWrite(V13, temperature);
  Blynk.virtualWrite(V14, humidity);

  if (temperature >= (SETPOINT_TEMPERATURE + 2)) {
    output_data[0] = '1';
  } else if (temperature <= (SETPOINT_TEMPERATURE - 2)) {
    output_data[0] = '0';
  }

  Serial.write(output_data, 3); /*write output data to ATMega */
  Serial.readBytes(input_data, 1);

  if (input_data[0] >= 49 && input_data[0] <= 55)
    auxiliar = input_data[0];
  switch (auxiliar) {
    case '1': Blynk.virtualWrite(V9, "Chicken Quesadilla (Pui, Branza)"); break;
    case '2': Blynk.virtualWrite(V9, "Paste Carbonara (Ou, Branza)"); break;
    case '3': Blynk.virtualWrite(V9, "Salata Caprese (Branza, Rosii)"); break;
    case '4': Blynk.virtualWrite(V9, "Chicken Burrito (Pui, Branza, Rosii)"); break;
    case '5': Blynk.virtualWrite(V9, "Scrambled Eggs (Oua, Branza)"); break;
    case '6': Blynk.virtualWrite(V9, "Toasted Sandwich (Pui, Branza, Ou, Rosii)"); break;
    default: Blynk.virtualWrite(V9, "DEFAULT");
  }

  if (on_off_control == 0) {
    analogWrite(blue_led, aux_b * consumption_reduction);
    led_b = map(aux_b * consumption_reduction, 0, 255, 0, 25);
    analogWrite(red_led, aux_r * consumption_reduction);
    led_r = map(aux_r * consumption_reduction, 0, 255, 0, 25);
    analogWrite(yellow_led, aux_y * consumption_reduction);
    led_y = map(aux_y * consumption_reduction, 0, 255, 0, 25);
    analogWrite(white_led, aux_w * consumption_reduction);
    led_w = map(aux_w * consumption_reduction, 0, 255, 0, 25);
  } else {
    digitalWrite(blue_led, LOW);
    digitalWrite(red_led, LOW);
    digitalWrite(yellow_led, LOW);
    digitalWrite(white_led, LOW);
  }


}

void myTimeCounter() {
  time_counter++;
  static float value;
  miliAmps_led = led_b + led_y + led_r + led_w;
  if (consumption_reduction != 0)

    value = (float)(miliAmps_led * 5 * time_counter) / 1000 / 3600;
  Blynk.virtualWrite(V8, value);
  Blynk.virtualWrite(V12, value * 2.13);
  Blynk.virtualWrite(V11, consumption_reduction);
}


BLYNK_WRITE(V0) /* servo_doors */
{

  int aux = param.asInt();

  if (ONE == aux) {
    output_data[1] = '1';
  } else {
    output_data[1] = '0';
  }
}

BLYNK_WRITE(V1) /* led_band */
{

  int aux = param.asInt();
  if (on_off_control == 0) {
    if (ONE == aux) {
      output_data[2] = '1';
    } else {
      output_data[2] = '0';
    }
  } else {
    output_data[2] = '0';
  }
}

BLYNK_WRITE(V2) {
  SETPOINT_TEMPERATURE = param.asInt();
}

/* ----------------------------------------------------------- */

BLYNK_WRITE(V3) /* blue_led */
{

  aux_b = param.asInt();
  if (on_off_control == 0) {
    analogWrite(blue_led, aux_b * consumption_reduction);
    led_b = map(aux_b * consumption_reduction, 0, 255, 0, 25);
  }
}

BLYNK_WRITE(V4) /* led_red */
{

  aux_r = param.asInt();
  if (on_off_control == 0) {
    analogWrite(red_led, aux_r * consumption_reduction);
    led_r = map(aux_r * consumption_reduction, 0, 255, 0, 25);
  }
}

BLYNK_WRITE(V5) /* led_yellow */
{

  aux_y = param.asInt();
  if (on_off_control == 0) {
    analogWrite(yellow_led, aux_y * consumption_reduction);
    led_y = map(aux_y * consumption_reduction, 0, 255, 0, 25);
  }
}

BLYNK_WRITE(V6) /* led_white */
{

  aux_w = param.asInt();
  if (on_off_control == 0) {
    analogWrite(white_led, aux_w * consumption_reduction);
    led_w = map(aux_w * consumption_reduction, 0, 255, 0, 25);
  }
}

BLYNK_WRITE(V7) /* on_off_control */
{

  on_off_control = param.asInt();
}

BLYNK_WRITE(V10) /* on_off_control */
{

  consumption_reduction = (float)(param.asInt()) / (float)100;
}

void setup() {

  pinMode(blue_led, OUTPUT);
  pinMode(white_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  Serial.begin(9600);
  dht.begin();
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  pinMode(blue_led, OUTPUT);
  timer.setInterval(200, myTimer);
  timeCount.setInterval(1000, myTimeCounter);
}

void loop() {

  Blynk.run();
  timer.run();
  timeCount.run();
}