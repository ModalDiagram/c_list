#include <stdio.h>
#include <stdlib.h>
#include "../list/list.h"

int main(int argc, char *argv[])
{
  pvoid plist;

  if((plist = malloc_list(type_list_dynamic, "INT", 1)) == NULL) printf("Non creata.\n");
  return 0;
}
