#define	XTAL		16e6			// 16MHz

#define PHASE_A		(PIND & 1<<PA3)
#define PHASE_B		(PIND & 1<<PA2)

volatile int8_t enc_delta;			// -128 ... 127
volatile static int8_t last;

void encode_init( void );
int8_t encode_read1( void );
int8_t encode_read2( void );


