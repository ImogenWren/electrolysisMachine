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
#define I_SAMPLE_PIN A5

#include <autoDelay.h>

autoDelay sampleDelay;



void setup() {
  Serial.begin(115200);
}



uint16_t Vin_ADC;
float Vin_V_Sampled;
uint16_t Vin_Voltage;

#define VIN_ADC_MAX 960
#define VIN_ADC_MIN 0
#define VIN_SAMPLE_MIN 1.16
#define VIN_SAMPLE_MAX 15.0

uint16_t I_ADC;
uint16_t shunt_Voltage_Drop;
uint16_t I_Current;







void sampleADCs() {
  Vin_ADC = analogRead(VIN_SAMPLE_PIN);
  I_ADC = analogRead(I_ADC);
}


void calcVin() {

}


struct splitFloat {
  int16_t preDecimalPoint;
  uint16_t postDecimalPoint;
}


struct returnSplitFloat(float floatIn) {

  int16_t pre;
  uint16_t post;

  pre = int(floatIn);
  floatIn = floatIn - pre;
  post = 0;


  splitFloat = {pre, post};

  return splitFloat;

}


void floatToString(float floatIn) {

  char floatString[20];
  int16_t pre;
  uint16_t post;
  pre = int(floatIn);
  float val_float = (abs(floatIn) - abs(pre)) * 100000;
  post = int(val_float);
  sprintf (floatString, "%d.%05d", pre, post); //  the %05d avoids getting "0.5" when the value is "0.00005" or "0.0005" or any other similar value.
  Serial.println(floatString);

}

void printOut() {
  buffer[64];
  sprintf(buffer, "%i, %i.%i, %i ,%i, %i, %i", Vin_ADC, Vin_V_Sampled, Vin_Voltage, I_ADC, shunt_Voltage_Drop, I_Current );

}


float testValue = -96.12345;


void loop() {
  void floatToString(testValue)S;
  delay(2000);)

}
