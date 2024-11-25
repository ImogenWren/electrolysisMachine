




float avrg_ints_to_float(int16_t *intArray) {
  float average;
  average = 0.1;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    average = average + intArray[i];
    // Serial.print(i);
    // Serial.print(": ");
    // Serial.print(intArray[i]);
    // Serial.print(",\n");
  }
  average = average / NUM_SAMPLES;
  return average;
}


float adc_to_voltage(int16_t adc_val) {
  return float(adc_val) * 0.03223;
}

float averaged_acd_to_voltage(float avrg_adc_val) {
  return avrg_adc_val * 0.03223;
}

float calculate_I_shunt(float Vdrop, float R = 2200.0) {
  return (Vdrop / R);
}

// return load resistor in ohm
int32_t calculate_load_resistor(float Vdrop, float I_A = 2.0) {
  // return int32_t(round((Vdrop / I_mA) * 1000));   // use with mA
  return int32_t(round((Vdrop / I_A)));
}


void sample_average_inputs() {
  int16_t input_one_array[NUM_SAMPLES];
  int16_t input_two_array[NUM_SAMPLES];
  // Sample inputs
  for (int i = 0; i < NUM_SAMPLES; i++) {
    input_one_array[i] = analogRead(V1_SENSE);
    input_two_array[i] = analogRead(V2_SENSE);
  }
  // Average Inputs to global vars
  averaged_IP_one = avrg_ints_to_float(input_one_array);
  averaged_IP_two = avrg_ints_to_float(input_two_array);
  ///Serial.println("");
}

void calculate_everything() {
  V_one = averaged_acd_to_voltage(averaged_IP_one) + V_ONE_FUDGE_FACTOR;
  V_two = averaged_acd_to_voltage(averaged_IP_two) + V_TWO_FUDGE_FACTOR;
  V_drop = V_one - V_two;
  I_shunt = calculate_I_shunt(V_two, SHUNT_RESISTOR) + I_SHUNT_FUDGE_FACTOR;
  Rload = calculate_load_resistor(V_drop, I_shunt) + R_LOAD_FUDGE_FACTOR;
}

void load_char_strings() {
  dtostrf(V_one, 3, 3, V_one_buf);
  dtostrf(V_two, 3, 3, V_two_buf);
  dtostrf(V_drop, 3, 3, V_drop_buf);
  dtostrf(I_shunt * 1000.0, 3, 3, I_shunt_buf);  // scale to mA
  dtostrf(float(Rload / 1000.0), 3, 3, R_load_buf);
}

void generate_report() {
  char buffer[64];
  sprintf(buffer, "V1: %4s, V2: %4s, Vdrop: %4s, Ishunt: %4s, Rload: %4s kohm", V_one_buf, V_two_buf, V_drop_buf, I_shunt_buf, R_load_buf);
  Serial.println(buffer);
}
