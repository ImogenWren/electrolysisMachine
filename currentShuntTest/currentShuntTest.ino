/*
    Test Program for testing current detection using shunt resistor


*/



/* Circuit Specifications:

    Vin <= 20v

    Fixed Resistor = 8k2
    Current Set Resistor = 100-500k
    Shunt Resistor = 500r

    A5 = I measure

    Voltage Divider for Vin Voltage Measurement:
    R1 =  33k
    R2 = 10k

    A7 = Vin Measure


    Circuit Protection:

   15v zener diode with 100r resistor between Vin and GNDs. Clamps Vin to 15v for testing
   5.1v zener diode with 100r resistor between Isense pin and GND. Clamps Detection voltage to <=5v


   Expected Values:

   A5 ADC     =   19    - 195
   A5 Voltage =   0.09  -   0.952
   A5 Current =   0.2   -   1.9

   A7 ADC = 238 - 953
   A7 Voltage = 1.16 - 953
   Vin Voltage = 5 - 20v

*/

#define VIN_SAMPLE_PIN A7
#define SHUNT_SAMPLE_PIN A5

#include <autoDelay.h>

autoDelay sampleDelay;

#include "returnStrings.h"


uint16_t u_Vin_ADC;            // Value returned from ACD
float f_Vin_sampled;          // Actual measured voltage
float f_Vin_scaled;           // True Vin Voltage (scaled)


char cs_Vin_sample[20];   // Measured voltage as a char string
char cs_Vin_scaled[20];          // True Scaled voltage as char string

#define ADC_MAX 5.0   // Max voltage for ADC 

#define VIN_ADC_MAX 960
#define VIN_ADC_MIN 0

#define VIN_SAMPLE_MIN 1.16
#define VIN_SAMPLE_MAX 15.0

uint16_t u_shuntADC;               // Value returned from ADC
float f_shunt_Vdrop;            // Measured voltage drop across shunt resistor
float f_shuntI;              // Calculated current through shunt resistor

#define SHUNT_RESISTOR 500.0   // Shunt resistance in ohms
#define SHUNT_SAMPLE_GAIN 1.0  // gain factor of shunt voltage sampling
#define SHUNT_CURRENT_OFFSET -0.01


char cs_shunt_Vdrop[20];          // Measured voltage drop as char string
char cs_shuntI[20];          // Calculated current as char string


char printBuffer[64];   // buffer to hold all printable values

float voltsPerADC = 0.01;


/* Measured Values & Calibration

    Vin = 5.010   Vin Sampled voltage = 1.18
    Vin = 9.00    Vin Sampled Voltage = 2.065
    Vin = 15.00   Vin Sampled Voltage = 3.448
    15/3.5 = 4.2857
    Therefore
*/

#define VIN_SCALER 4.3
#define VIN_ADC_OFFSET_HIGH -0.20   // offset value between true voltage and calculated voltage at Vmax (15v)
#define VIN_ADC_OFFSET_LOW -0.05      // offset value between true voltage and calculated voltage at Vmin (5v)
float vin_ADV_offset = 0;         // calculated average during setup

void setupVinConversions() {
  voltsPerADC = ADC_MAX / 1024.0;
  Serial.print("Volts Per ADC: ");
  Serial.println(voltsPerADC, 6);
  vin_ADV_offset = (VIN_ADC_OFFSET_HIGH + VIN_ADC_OFFSET_LOW) / 2;
  Serial.print("Vin ADC Offset: ");
  Serial.println(vin_ADV_offset, 6);

}

void setupCurrentConversions() {

}


void sampleADCs() {
  u_Vin_ADC = analogRead(VIN_SAMPLE_PIN);
  u_shuntADC = analogRead(SHUNT_SAMPLE_PIN);
  //  Serial.print("Vin ADC: ");
  //  Serial.print(u_Vin_ADC);
  // Serial.print("Shunt ADC: ");
  //Serial.print(u_shuntADC);
}





void calcVin() {
  f_Vin_sampled = u_Vin_ADC * voltsPerADC;
  f_Vin_scaled = (f_Vin_sampled * VIN_SCALER) + vin_ADV_offset;
  //  Serial.print("  Vin Sample: ");
  //  Serial.print(f_Vin_sampled);
  Serial.print("  Vin Scaled: ");
  Serial.print(f_Vin_scaled);
  Serial.print(" |");
}


void calcShunt() {
  f_shunt_Vdrop = u_shuntADC * voltsPerADC;            // Measured voltage drop across shunt resistor
  f_shuntI = ((f_shunt_Vdrop / SHUNT_RESISTOR) * 1000) + SHUNT_CURRENT_OFFSET;
  Serial.print(" Shunt Vdrop: ");
  Serial.print(f_shunt_Vdrop);
  Serial.print(" Shunt Current: ");
  Serial.print(f_shuntI);
  Serial.print(" mA ");
}



void printOut() {
  // char buffer[64];
  // sprintf(buffer, "%i, %i, %i ,%i, %i, %i", Vin_ADC, Vin_V_Sampled, Vin_Voltage, I_ADC, shunt_Voltage_Drop, I_Current );
}


float testValue = -96.12345;


void setup() {
  Serial.begin(115200);
  setupVinConversions();
}


void loop() {
  // char buffer[64];
  // CHARFLOAT returnedString = {floatToString(testValue)};
  //
  //strcpy(
  // Serial.println(returnedString.fString);
  sampleADCs();
  calcVin();
  calcShunt(); +
  Serial.println(" ");
  delay(500);

}
