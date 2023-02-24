#ifndef MAIN_U_H__
#define MAIN_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_satus_t etc. */


#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OCALL_MAIN_SAMPLE_DEFINED__
#define OCALL_MAIN_SAMPLE_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_Main_sample, (const char* str));
#endif
#ifndef MY_UNTRUSTED_FUNC_DEFINED__
#define MY_UNTRUSTED_FUNC_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, my_untrusted_func, (const char* myPtr));
#endif
#ifndef MYFILE_UNTRUSTED_FUNC_DEFINED__
#define MYFILE_UNTRUSTED_FUNC_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, myfile_untrusted_func, (const char* myPrt));
#endif

sgx_status_t ecall_Main_sample(sgx_enclave_id_t eid, int* retval);
sgx_status_t my_func(sgx_enclave_id_t eid, int* retval, const char* myPtr);
sgx_status_t myfile_trusted_func(sgx_enclave_id_t eid, int* retval, const char* myPtr);
sgx_status_t ecall_lib1_sample(sgx_enclave_id_t eid, int* retval);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
