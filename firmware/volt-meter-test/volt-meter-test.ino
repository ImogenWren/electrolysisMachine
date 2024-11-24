/*
 Arduino Voltmeter

 Voltages are buffered through op-amp then scaled using resistive divider

  ## Resistive Dividers Calculations
  _Calculate all dividers based on max input voltage_

  Vo = Vi(R2/R1+R2)

  R1 = ((Vi*R1)/Vo)-R2
  R2 = (Vo*R1)/(Vi-Vo)

  Rt = R1 + R2
  I(Rt) = Vi/Rt
  P = I*Vi 

 ## ADC calculations
  ADC_max = 2^(bitness) = 2^10 = 1024
  V(ADC) = 5/1024 = 4.8828125 mV     // mV per ADC val
  ADC(V) = 1024/5 = 204.8              // ADV val per volt (i.e if 1 volt is detected at ADC, returned value should be this)

  ### Worked Example
  Vi = 30 v
  Vo = 5 v 

  Assume that the low value resistor (R2) will be 10k

  R1 = ((30*10k)/5)-10k = (300k/5)-10k = 50k 
  47k would be too low, so next series up is 56k

  Check Output
  Vo = 30(10k/66k) = 4.54 v [solved]

  Check Power through resistors
  I(Rt) = 30/66k = 0.00046 A = 0.46 mA
  P = 0.00046*30 = 0.0136 W = 13.6 mW  [solved, any greater than 125 mW may need more careful resistor selection]

  #### Scaling ADC value
  Vmeasured = ADC/204.8
  Vtrue = ADC*(6.6/204.8) or ADC*0.03223  (where did 6.6 come from? -> (Rt/R2) )

  #### Final Maths Test Worked Example

  If Input voltage of 25v
  Vo = 25*(10/66) = 3.79 V at input to ADC
  ADC = 3.79*204.8 = 776 ADC reading returned by microcontroller

  Vmeasured(ADC) = 3.79 V   Value returned for measured voltage measured by ADC
  Vtrue(ACD) = 25.00        Correct value should be returned for voltage before divider

  ## Calculating I through Load Resistor
  Using 2k2 as the shunt?
  I = V/R

  V = V2
  R = 2k2

  ## Calculating Load Resistor
  #TODO first we need to calculate I (to be continued) -> for now, assume I = 2mA

  Vdrop = voltage drop over load resistor =  V1-V2
  Rload = Vdrop/I 


*/

#define V1_SENSE A7
#define V2_SENSE A6

#include "globals.h"



void setup() {
  Serial.begin(115200);
  OLEDsetupSimple();  // OLED setup & Splash Screen
                      //  display.setFont(&FreeSansBold9pt7b);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  //  drawDot();   // actually draws square like a boss
  display.print("Test");
  display.display();
}





void loop() {
  if (sampleDelay.millisDelay(sample_delay_mS)) {
    sample_average_inputs();
  }

#if REPORT_ACTIVE == true
  if (reportDelay.millisDelay(report_delay_mS)) {
    calculate_and_report();
  }
#endif
}
