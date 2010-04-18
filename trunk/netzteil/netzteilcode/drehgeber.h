#define	XTAL		16e6			// 16MHz

#define PHASE_A		(PINC & 1<<PC6)
#define PHASE_B		(PINC & 1<<PC7)

#define PHASE_A2	(PINC & 1<<PC0)
#define PHASE_B2	(PINC & 1<<PC1)

volatile int8_t enc_delta;			// -128 ... 127
volatile static int8_t last;

volatile int8_t enc_delta2;			// -128 ... 127
volatile static int8_t last2;

void encode_init( void );
int8_t encode_read1( void );
int8_t encode_read2( void );
int8_t encode_read4( void );


