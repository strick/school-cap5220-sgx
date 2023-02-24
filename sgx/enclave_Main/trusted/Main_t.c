#include "Main_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */
#include "sgx_lfence.h" /* for sgx_lfence */

#include <errno.h>
#include <mbusafecrt.h> /* for memcpy_s etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_ENCLAVE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_within_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define ADD_ASSIGN_OVERFLOW(a, b) (	\
	((a) += (b)) < (b)	\
)


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

static sgx_status_t SGX_CDECL sgx_ecall_Main_sample(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_Main_sample_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_Main_sample_t* ms = SGX_CAST(ms_ecall_Main_sample_t*, pms);
	sgx_status_t status = SGX_SUCCESS;



	ms->ms_retval = ecall_Main_sample();


	return status;
}

static sgx_status_t SGX_CDECL sgx_my_func(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_my_func_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_my_func_t* ms = SGX_CAST(ms_my_func_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	const char* _tmp_myPtr = ms->ms_myPtr;
	size_t _len_myPtr = ms->ms_myPtr_len ;
	char* _in_myPtr = NULL;

	CHECK_UNIQUE_POINTER(_tmp_myPtr, _len_myPtr);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_myPtr != NULL && _len_myPtr != 0) {
		_in_myPtr = (char*)malloc(_len_myPtr);
		if (_in_myPtr == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_myPtr, _len_myPtr, _tmp_myPtr, _len_myPtr)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

		_in_myPtr[_len_myPtr - 1] = '\0';
		if (_len_myPtr != strlen(_in_myPtr) + 1)
		{
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

	ms->ms_retval = my_func((const char*)_in_myPtr);

err:
	if (_in_myPtr) free(_in_myPtr);
	return status;
}

static sgx_status_t SGX_CDECL sgx_myfile_trusted_func(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_myfile_trusted_func_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_myfile_trusted_func_t* ms = SGX_CAST(ms_myfile_trusted_func_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	const char* _tmp_myPtr = ms->ms_myPtr;
	size_t _len_myPtr = ms->ms_myPtr_len ;
	char* _in_myPtr = NULL;

	CHECK_UNIQUE_POINTER(_tmp_myPtr, _len_myPtr);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_myPtr != NULL && _len_myPtr != 0) {
		_in_myPtr = (char*)malloc(_len_myPtr);
		if (_in_myPtr == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_myPtr, _len_myPtr, _tmp_myPtr, _len_myPtr)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

		_in_myPtr[_len_myPtr - 1] = '\0';
		if (_len_myPtr != strlen(_in_myPtr) + 1)
		{
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

	ms->ms_retval = myfile_trusted_func((const char*)_in_myPtr);

err:
	if (_in_myPtr) free(_in_myPtr);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_lib1_sample(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_lib1_sample_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_lib1_sample_t* ms = SGX_CAST(ms_ecall_lib1_sample_t*, pms);
	sgx_status_t status = SGX_SUCCESS;



	ms->ms_retval = ecall_lib1_sample();


	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* ecall_addr; uint8_t is_priv;} ecall_table[4];
} g_ecall_table = {
	4,
	{
		{(void*)(uintptr_t)sgx_ecall_Main_sample, 0},
		{(void*)(uintptr_t)sgx_my_func, 0},
		{(void*)(uintptr_t)sgx_myfile_trusted_func, 0},
		{(void*)(uintptr_t)sgx_ecall_lib1_sample, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[3][4];
} g_dyn_entry_table = {
	3,
	{
		{0, 0, 0, 0, },
		{0, 0, 0, 0, },
		{0, 0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL ocall_Main_sample(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_ocall_Main_sample_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_Main_sample_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(str, _len_str);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (str != NULL) ? _len_str : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_Main_sample_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_Main_sample_t));
	ocalloc_size -= sizeof(ms_ocall_Main_sample_t);

	if (str != NULL) {
		ms->ms_str = (const char*)__tmp;
		if (_len_str % sizeof(*str) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}
	
	status = sgx_ocall(0, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL my_untrusted_func(const char* myPtr)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_myPtr = myPtr ? strlen(myPtr) + 1 : 0;

	ms_my_untrusted_func_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_my_untrusted_func_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(myPtr, _len_myPtr);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (myPtr != NULL) ? _len_myPtr : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_my_untrusted_func_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_my_untrusted_func_t));
	ocalloc_size -= sizeof(ms_my_untrusted_func_t);

	if (myPtr != NULL) {
		ms->ms_myPtr = (const char*)__tmp;
		if (_len_myPtr % sizeof(*myPtr) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, myPtr, _len_myPtr)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_myPtr);
		ocalloc_size -= _len_myPtr;
	} else {
		ms->ms_myPtr = NULL;
	}
	
	status = sgx_ocall(1, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL myfile_untrusted_func(const char* myPrt)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_myPrt = myPrt ? strlen(myPrt) + 1 : 0;

	ms_myfile_untrusted_func_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_myfile_untrusted_func_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(myPrt, _len_myPrt);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (myPrt != NULL) ? _len_myPrt : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_myfile_untrusted_func_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_myfile_untrusted_func_t));
	ocalloc_size -= sizeof(ms_myfile_untrusted_func_t);

	if (myPrt != NULL) {
		ms->ms_myPrt = (const char*)__tmp;
		if (_len_myPrt % sizeof(*myPrt) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, myPrt, _len_myPrt)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_myPrt);
		ocalloc_size -= _len_myPrt;
	} else {
		ms->ms_myPrt = NULL;
	}
	
	status = sgx_ocall(2, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

