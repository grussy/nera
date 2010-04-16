#define	XTAL		16e6			// 16MHz

#define PHASE_A		(PINC & 1<<PC7)
#define PHASE_B		(PINC & 1<<PC6)

volatile int8_t enc_delta;			// -128 ... 127
volatile static int8_t last;

void encode_init( void );
int8_t encode_read1( void );
int8_t encode_read2( void );


