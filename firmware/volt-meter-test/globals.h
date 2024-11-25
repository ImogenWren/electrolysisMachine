
// Include all external libraries here

#include <autoDelay.h>


autoDelay sampleDelay;
autoDelay reportDelay;

#define SAMPLE_RATE_HZ 10
#define REPORT_RATE_HZ 2

#define REPORT_ACTIVE true

#define NUM_SAMPLES 10

#define SHUNT_RESISTOR 2200

int16_t sample_delay_mS = 1000 / SAMPLE_RATE_HZ;
int16_t report_delay_mS = 1000 / REPORT_RATE_HZ;




float averaged_IP_one;  // averaged samples from ADCs
float averaged_IP_two;

#define V_ONE_FUDGE_FACTOR 0.5
#define V_TWO_FUDGE_FACTOR 0.4
#define I_SHUNT_FUDGE_FACTOR 0.0
#define R_LOAD_FUDGE_FACTOR 0.0

float V_one;    //    = averaged_acd_to_voltage(averaged_IP_one);
float V_two;    //    = averaged_acd_to_voltage(averaged_IP_two);
float V_drop;   //   = V_one - V_two;
float I_shunt;  //  = calculate_I_shunt(V_two, SHUNT_RESISTOR);
int32_t Rload;  //  = calculate_load_resistor(V_drop, I_shunt);

char V_one_buf[8];   // to hold values for printing neatly
char V_two_buf[8];
char V_drop_buf[8];
char I_shunt_buf[8];
char R_load_buf[8];

// Include any local headers here (at bottom of globals.h)
#include "circuitAnalysis.h"
#include "oledFunctions.h"