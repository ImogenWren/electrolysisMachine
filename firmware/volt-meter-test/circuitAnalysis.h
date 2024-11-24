




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

float averaged_IP_one;
float averaged_IP_two;
void sample_average_inputs(){
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

void calculate_and_report(){
    char buffer[64];
    char v_one_buf[8];
    char v_two_buf[8];
    char v_drop_buf[8];
    char I_shunt_buf[8];
    char R_load_buf[8];
    // char r_load_buf[8];
    float V_one = averaged_acd_to_voltage(averaged_IP_one);
    float V_two = averaged_acd_to_voltage(averaged_IP_two);
    float V_drop = V_one - V_two;
    float I_shunt = calculate_I_shunt(V_two, SHUNT_RESISTOR);
    int32_t Rload = calculate_load_resistor(V_drop, I_shunt);
    dtostrf(V_one, 3, 3, v_one_buf);
    dtostrf(V_two, 3, 3, v_two_buf);
    dtostrf(V_drop, 3, 3, v_drop_buf);
    dtostrf(I_shunt * 1000.0, 3, 3, I_shunt_buf);  // scale to mA
    dtostrf(float(Rload/1000.0), 3, 3, R_load_buf);
    sprintf(buffer, "V1: %4s, V2: %4s, Vdrop: %4s, Ishunt: %4s, Rload: %4s kohm", v_one_buf, v_two_buf, v_drop_buf, I_shunt_buf, R_load_buf);
    Serial.println(buffer);
}
