#ifndef MAIN_T_H__
#define MAIN_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */


#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

int ecall_Main_sample(void);
int my_func(const char* myPtr);
int myfile_trusted_func(const char* myPtr);
int ecall_lib1_sample(void);

sgx_status_t SGX_CDECL ocall_Main_sample(const char* str);
sgx_status_t SGX_CDECL my_untrusted_func(const char* myPtr);
sgx_status_t SGX_CDECL myfile_untrusted_func(const char* myPrt);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
