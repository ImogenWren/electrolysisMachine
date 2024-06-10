/* Electrolysis Machine - Charge Pump Driver Test


  Functions:
  - Pot read to PWM duty cycle
  - PWM output to charge pump
  - Voltage meter for output of charge pump


  Circuit:
  - PWM output goes to NOT gate to produce !PWM signal. 
  - PWM & !PWM to 555 Bistable Timer to produce large voltage swing from Vsuppply
  - Charge Pump Circuit driven from 555 output & Vsupply

*/

#define PWM_PUMP_PIN 3
#define POT_PIN A1

#include <autoDelay.h>

autoDelay printDelay;
#define PRINT_DELAY_mS 1000

// Voltmeter Variables
#define VOLTAGE_SENSE_PIN A4
#define V_MAX 30       // Max voltage to measure
#define VMAX_BUFFER 2  // buffer zone to avoid damage
#define V_ADC 5        // Voltage of the ADC
#define R_ONE 10000    // Resistor from Vin
// Next values must be calculated
#define R_TWO 1500                      // Resister to GND calculated by: R_TWO = R_ONE/((V_MAX+VMAX_BUFFER)/V_ADC)
#define ADC_MAX_V 4.8                   // calculated by ADC_MAX_V = (V_MAX+VMAX_BUFFER)/(R_ONE/R_TWO)
#define ADC_TICKS 1024                  // Calculated from 10 bit ADC
#define VOLTMETER_CURRENT_mA 2.67       //mA   // calculated from I = (V_MAX / (R_ONE + R_TWO))*1000
#define ADC_TO_MEAS_V_SCALER 0.00469    // Calculated as    (ADC_MAX_V/ADC_TICKS)
#define ADC_TO_SOURCE_V_SCALER 0.03125  // Calculated as    (ADC_MAX_V/ADC_TICKS)*(R_ONE/R_TWO)
#define MEAS_V_TO_SOURCE_V_SCALER 6.6   // Calculated as (V_meas * (R_ONE/R_TWO))

float V_meas = 1.4;
float V_source = 9.3;


void calculate_voltage_divider() {
}

void sampleVoltage() {
  int adcRead = analogRead(VOLTAGE_SENSE_PIN);
  V_meas = adcRead * ADC_TO_MEAS_V_SCALER;
  V_source = adcRead * ADC_TO_SOURCE_V_SCALER;
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nCharge Pump Test - Prototype\n");
  TCCR1B = TCCR1B & B11111000 | B00000001;  // for PWM frequency of 31372.55 Hz
  pinMode(PWM_PUMP_PIN, OUTPUT);
  analogWrite(PWM_PUMP_PIN, 0);
}

int potRead;
int dutyPercent;
int pwmValue;
int pot_to_pwm() {
  potRead = analogRead(POT_PIN);
  dutyPercent = potRead / 10.23;
  pwmValue = map(dutyPercent, 0, 100, 0, 255);
  return pwmValue;
}

void print_stats() {
  if (printDelay.millisDelay(PRINT_DELAY_mS)) {
    char buffer[128];
    char V_meas_buffer[8];
    char V_source_buffer[8];
    dtostrf(V_meas, 4, 2, V_meas_buffer);
    dtostrf(V_source, 4, 2, V_source_buffer);
    sprintf(buffer, "dutyPercent: %4i, pwmValue: %4i, V_meas: %s, V_source: %s", dutyPercent, pwmValue, V_meas_buffer, V_source_buffer);
    Serial.println(buffer);
  }
}

void loop() {
  sampleVoltage();
  analogWrite(PWM_PUMP_PIN, pot_to_pwm());
  print_stats();
}