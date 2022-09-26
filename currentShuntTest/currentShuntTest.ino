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
#define SHUNT_SAMPLE_PIN A4

#include <autoDelay.h>
#include <buttonObject.h>



autoDelay sampleDelay;
autoDelay printDelay;
autoDelay inputDelay;
autoDelay outputDelay;

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
int16_t shuntI_uA;           // Calculated current in micro amps

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



/* Adding code for trigger button input
    current set pot input, and
    transistor control output

    At the moment no feedback control, trigger pin sets transistor output high
    Current set pot is read then scaled to mA value but this value is not used yet

*/

#define CONTROL_POT_PIN A0
#define TRIGGER_INPUT_PIN 7
#define TRANSISTOR_OUTPUT_PIN 3
#define INDICATOR_PIN 9   // Mirrors output pin for LED indicator or other indication method
uint16_t controlPotValue;
uint16_t uA_setpoint;

buttonObject button(TRIGGER_INPUT_PIN, BUTTON_PULL_HIGH);    // Set up instance of buttonObject. Pass Button Pin & whether it pulls HIGH, or LOW when pressed.

/*
    PID controller functions

    PID controller will control the current through the shunt resistor using PWM

    pidController library will be used for maths


*/

#include <pidController.h>

#define P_GAIN 1.0
#define I_GAIN 0.0
#define D_GAIN 0.0

pidController PID;

void setupPID() {
  PID.begin();
  PID.updateGain(P_GAIN, I_GAIN, D_GAIN);
}


void setupDigitalPins() {
  pinMode(TRIGGER_INPUT_PIN, INPUT);
  pinMode(TRANSISTOR_OUTPUT_PIN, OUTPUT);
}

void updateOutput(uint8_t output_value) {
  analogWrite(TRANSISTOR_OUTPUT_PIN, output_value);
  analogWrite(INDICATOR_PIN, output_value);    // Mirror output for easy indication
}

void checkButton() {
  //  button.buttonLoop();
  digitalWrite(TRANSISTOR_OUTPUT_PIN, button.detectButton());
}

void controlPot() {
  controlPotValue = analogRead(CONTROL_POT_PIN);   // Sample the ADC
  // This then needs to be scaled to 0 to 2ma. Instead going to scale from 0 to 2000uA to avoid floats
  uA_setpoint = map(controlPotValue, 0, 1024, 0, 2000);
  Serial.print("uA setpoint: ");
  Serial.print(uA_setpoint);
  Serial.print(" uA  |");

}

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
  shuntI_uA = int(f_shuntI * 1000);
  Serial.print(" Shunt Vdrop: ");
  Serial.print(f_shunt_Vdrop);
  Serial.print(" Shunt Current: ");
  Serial.print(f_shuntI);
  Serial.print(" mA, ");
  Serial.print(shuntI_uA);
  Serial.print(" uA");
}



void printOut() {
  // char buffer[64];
  // sprintf(buffer, "%i, %i, %i ,%i, %i, %i", Vin_ADC, Vin_V_Sampled, Vin_Voltage, I_ADC, shunt_Voltage_Drop, I_Current );
}


float testValue = -96.12345;


void setup() {
  Serial.begin(115200);
  setupVinConversions();
  setupDigitalPins();
  setupPID();
}


void loop() {


  // Input Functions
  // Check the PID Controller Settings and State
  if (inputDelay.millisDelay(PID.g_input_delay_mS)) {
    controlPot();
    PID.updateSetpoint(uA_setpoint);
  }

  if (sampleDelay.microsDelay(PID.g_sample_delay_uS)) {
    sampleADCs();
    calcVin();
    calcShunt();
    checkButton();
    PID.updateInput(shuntI_uA);
    Serial.println(" ");
  }
  // Physical Output
  if (outputDelay.microsDelay(PID.g_output_delay_uS)) {
    PID.g_output_value = PID.PIDcontroller(PID.g_setpoint, PID.g_sensor_value, PID.g_output_value);
    //  error_value = PID.g_setpoint - PID.g_sensor_value;
    // average.addDataPoint(error_value);
    // average_error = average.calcMean();
    updateOutput(PID.g_output_value);
  }


 
  //delay(200);

}
