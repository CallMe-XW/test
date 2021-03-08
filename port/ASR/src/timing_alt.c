
#include "../../../port/ASR/inc/timing_alt.h"
#include "ql_rtos.h"

struct timeval { 
	int tv_sec; 
	int tv_usec;
};

struct _hr_time
{
    struct timeval start;
};

typedef unsigned int uint32_t;

static int hardclock_init = 0;
static struct timeval tv_init;

unsigned long mbedtls_timing_hardclock( void )
{
    struct timeval tv_cur;

    if( hardclock_init == 0 )
    {
        gettimeofday( &tv_init, NULL );
        hardclock_init = 1;
    }

    gettimeofday( &tv_cur, NULL );
    return( ( tv_cur.tv_sec  - tv_init.tv_sec  ) * 1000000
          + ( tv_cur.tv_usec - tv_init.tv_usec ) );
}

unsigned long mbedtls_timing_get_timer( struct mbedtls_timing_hr_time *val, int reset )
{
    struct _hr_time *t = (struct _hr_time *) val;

    if( reset )
    {
        gettimeofday( &t->start, NULL );
        return( 0 );
    }
    else
    {
        unsigned long delta;
        struct timeval now;
        gettimeofday( &now, NULL );
        delta = ( now.tv_sec  - t->start.tv_sec  ) * 1000ul
              + ( now.tv_usec - t->start.tv_usec ) / 1000;
        return( delta );
    }
}

volatile int mbedtls_timing_alarmed = 0;

static void alarmhandler(u32 argv)
{
	mbedtls_timing_alarmed = 1;
}

int mbedtls_set_alarm( int seconds )
{
	ql_timer_t alarm = NULL;

	mbedtls_timing_alarmed = 0;

	if(seconds == 0)
	{
		mbedtls_timing_alarmed = 1;
		return 0;
	}
	
	ql_rtos_timer_create(&alarm);
	ql_rtos_timer_start(alarm, seconds * 1000, 0, alarmhandler, NULL);
	return ( int )alarm;
}

void mbedtls_free_alarm( int alarm )
{
	if(alarm) ql_rtos_timer_delete((ql_timer_t) alarm);
}

void mbedtls_timing_set_delay( void *data, uint32_t int_ms, uint32_t fin_ms )
{
    mbedtls_timing_delay_context *ctx = (mbedtls_timing_delay_context *) data;

    ctx->int_ms = int_ms;
    ctx->fin_ms = fin_ms;

    if( fin_ms != 0 )
        (void) mbedtls_timing_get_timer( &ctx->timer, 1 );
}

int mbedtls_timing_get_delay( void *data )
{
    mbedtls_timing_delay_context *ctx = (mbedtls_timing_delay_context *) data;
    unsigned long elapsed_ms;

    if( ctx->fin_ms == 0 )
        return( -1 );

    elapsed_ms = mbedtls_timing_get_timer( &ctx->timer, 0 );

    if( elapsed_ms >= ctx->fin_ms )
        return( 2 );

    if( elapsed_ms >= ctx->int_ms )
        return( 1 );

    return( 0 );
}

