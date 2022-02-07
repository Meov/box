#include <stdlib.h>
#include <stdio.h>
int main(void) {
  char* s=NULL;
  s=getenv("COMSPEC"); /* get the comspec environment parameter */
  printf("Command processor: %s\n",s); /* display comspec parameter */
  return 0;
}
