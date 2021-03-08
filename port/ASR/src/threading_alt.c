
#include "../../../include/mbedtls/threading.h"

/*
 * Thread porting based on QL_RTOS.
 */
void mbedtls_port_threading_mutex_init(mbedtls_threading_mutex_t * mutex)
{
	if(mutex)
	{
		int ret = 0;
		ret = ql_rtos_mutex_create(&mutex->mutex);
		mutex->is_valid = !ret;
	}
}

void mbedtls_port_threading_mutex_free(mbedtls_threading_mutex_t * mutex)
{
	if(mutex)
	{
		int ret = 0;
		ret = ql_rtos_mutex_delete(mutex->mutex);
		mutex->is_valid = !!ret;
	}
}

int mbedtls_port_threading_mutex_lock(mbedtls_threading_mutex_t * mutex)
{
	if(mutex)
	{
		int ret = 0;
		ret = ql_rtos_mutex_lock(mutex->mutex, QL_WAIT_FOREVER);
		return (ret ? -1 : 0);
	}

	return -1;
}

int mbedtls_port_threading_mutex_unlock(mbedtls_threading_mutex_t * mutex)
{
	if(mutex)
	{
		int ret = 0;
		ret = ql_rtos_mutex_unlock(mutex->mutex);
		return (ret ? -1 : 0);
	}

	return -1;
}

void mbedtls_port_threading_exit(int status)
{
	(void)status;
	ql_rtos_task_delete(NULL);
}

