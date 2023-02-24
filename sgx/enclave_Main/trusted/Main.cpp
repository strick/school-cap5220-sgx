#include <stdarg.h>
#include <stdio.h>      /* vsnprintf */

#include "Main.h"
#include "Main_t.h"  /* print_string */

/*
 * Defining new trusted function
 */
int my_func(const char *myPtr)
{

}
/* 
 * printf: 
 *   Invokes OCALL to display the enclave buffer to the terminal.
 */
void printf(const char *fmt, ...)
{
    char buf[BUFSIZ] = {'\0'};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_Main_sample(buf);
}

int ecall_Main_sample()
{
  printf("IN MAIN\n");

  printf("Calling untrusted my_func\n");
  my_untrusted_func("hello");

  printf("Calling myfile untrusted func\n");
  myfile_untrusted_func("Hello again");

  printf("Calling static trusted\n");
  ecall_lib1_sample();
  return 0;
}

