/* Header file for returning char strings from functions using structures
 * 
 * 
 */




// For dealing with Floats

typedef struct CHARFLOAT{
  char fString[20];
};


CHARFLOAT floatToString(float floatIn) {
  char floatString[20];
  int16_t pre;
  uint16_t post;
  pre = int(floatIn);
  float val_float = (abs(floatIn) - abs(pre)) * 100000;
  post = int(val_float);
  sprintf (floatString, "%d.%05d", pre, post); //  the %05d avoids getting "0.5" when the value is "0.00005" or "0.0005" or any other similar value.
  //strcpy(nameList[0].name, "FIRSTNAME");  
  CHARFLOAT charFloat;
  strcpy(charFloat.fString, floatString);
  return charFloat;
}
