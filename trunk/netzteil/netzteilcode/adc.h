#define ADCMAXINC 963
#define ADCMAXVOL 1700
void setdac(int value);
void init_ADC(void);
int getadc(void);
int getadc_ready(void);
void setadc_ready(int ready);
long getadc_Volts(void);
long getVoltage(void);
void setVoltage(long voltage100);
