#define ADCMAXINC 1023
#define ADCMAXVOL 3300
#define DACMAXINC 1004
#define ADCMAXCUR 200
#define ADCMAXCURINC	1000
void setdac(int value);
void init_ADC(void);
int getadc_u(void);
int getadc_i(void);
long getadc_Volts(void);
long getadc_Current(void);
long getVoltage(void);
void setVoltage(long voltage100);
long getCurrent(void);
void setCurrent(long current100);
