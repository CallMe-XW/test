
#include "../../../port/ASR/inc/threading_alt.h"
#include "../../../include/mbedtls/threading.h"
#include "../../../include/mbedtls/platform.h"

/*
 * Placeholder platform setup that does nothing by default
 */
int mbedtls_platform_setup( mbedtls_platform_context *ctx )
{
    (void)ctx;

	mbedtls_threading_set_alt(
		mbedtls_port_threading_mutex_init,
		mbedtls_port_threading_mutex_free,
		mbedtls_port_threading_mutex_lock,
		mbedtls_port_threading_mutex_unlock);

	mbedtls_platform_set_exit(mbedtls_port_threading_exit);

	mbedtls_platform_set_fprintf(ql_fprintf);

	mbedtls_platform_set_printf(uart_printf);

	mbedtls_platform_set_snprintf(snprintf);

    return( 0 );
}

/*
 * Placeholder platform teardown that does nothing by default
 */
void mbedtls_platform_teardown( mbedtls_platform_context *ctx )
{
    (void)ctx;
}

