#include "Main_u.h"
#include <errno.h>

typedef struct ms_ecall_Main_sample_t {
	int ms_retval;
} ms_ecall_Main_sample_t;

typedef struct ms_my_func_t {
	int ms_retval;
	const char* ms_myPtr;
	size_t ms_myPtr_len;
} ms_my_func_t;

typedef struct ms_myfile_trusted_func_t {
	int ms_retval;
	const char* ms_myPtr;
	size_t ms_myPtr_len;
} ms_myfile_trusted_func_t;

typedef struct ms_ecall_lib1_sample_t {
	int ms_retval;
} ms_ecall_lib1_sample_t;

typedef struct ms_ocall_Main_sample_t {
	const char* ms_str;
} ms_ocall_Main_sample_t;

typedef struct ms_my_untrusted_func_t {
	const char* ms_myPtr;
} ms_my_untrusted_func_t;

typedef struct ms_myfile_untrusted_func_t {
	const char* ms_myPrt;
} ms_myfile_untrusted_func_t;

static sgx_status_t SGX_CDECL Main_ocall_Main_sample(void* pms)
{
	ms_ocall_Main_sample_t* ms = SGX_CAST(ms_ocall_Main_sample_t*, pms);
	ocall_Main_sample(ms->ms_str);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Main_my_untrusted_func(void* pms)
{
	ms_my_untrusted_func_t* ms = SGX_CAST(ms_my_untrusted_func_t*, pms);
	my_untrusted_func(ms->ms_myPtr);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Main_myfile_untrusted_func(void* pms)
{
	ms_myfile_untrusted_func_t* ms = SGX_CAST(ms_myfile_untrusted_func_t*, pms);
	myfile_untrusted_func(ms->ms_myPrt);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[3];
} ocall_table_Main = {
	3,
	{
		(void*)Main_ocall_Main_sample,
		(void*)Main_my_untrusted_func,
		(void*)Main_myfile_untrusted_func,
	}
};
sgx_status_t ecall_Main_sample(sgx_enclave_id_t eid, int* retval)
{
	sgx_status_t status;
	ms_ecall_Main_sample_t ms;
	status = sgx_ecall(eid, 0, &ocall_table_Main, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t my_func(sgx_enclave_id_t eid, int* retval, const char* myPtr)
{
	sgx_status_t status;
	ms_my_func_t ms;
	ms.ms_myPtr = myPtr;
	ms.ms_myPtr_len = myPtr ? strlen(myPtr) + 1 : 0;
	status = sgx_ecall(eid, 1, &ocall_table_Main, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t myfile_trusted_func(sgx_enclave_id_t eid, int* retval, const char* myPtr)
{
	sgx_status_t status;
	ms_myfile_trusted_func_t ms;
	ms.ms_myPtr = myPtr;
	ms.ms_myPtr_len = myPtr ? strlen(myPtr) + 1 : 0;
	status = sgx_ecall(eid, 2, &ocall_table_Main, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_lib1_sample(sgx_enclave_id_t eid, int* retval)
{
	sgx_status_t status;
	ms_ecall_lib1_sample_t ms;
	status = sgx_ecall(eid, 3, &ocall_table_Main, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

