
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



// Include any local headers here
#include "circuitAnalysis.h"
#include "oledFunctions.h"