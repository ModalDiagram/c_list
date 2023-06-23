#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  char tipo_max[1000];
  int  size_max=0;
  FILE* pf;

  if(sizeof(int) > size_max){sprintf(tipo_max, "int"); size_max=sizeof(int);}
  if(sizeof(float) > size_max){sprintf(tipo_max, "float"); size_max=sizeof(float);}
  if(sizeof(double) > size_max){sprintf(tipo_max, "double"); size_max=sizeof(double);}
  if(sizeof(void*) > size_max){sprintf(tipo_max, "void*"); size_max=sizeof(void*);}
  if(sizeof(char) > size_max){sprintf(tipo_max, "char"); size_max=sizeof(char);}
  if(sizeof(long double) > size_max){sprintf(tipo_max, "long double"); size_max=sizeof(long double);}
  if(sizeof(long int) > size_max){sprintf(tipo_max, "long int"); size_max=sizeof(long int);}
  if(sizeof(long long int) > size_max){sprintf(tipo_max, "long long int"); size_max=sizeof(long long int);}

  pf = fopen("define_all_type.txt", "w");
  fprintf(pf, "#ifndef ALL_TYPE");
  fprintf(pf, "#define ALL_TYPE %s", tipo_max);
  fprintf(pf, "#endif");

  return 0;
}
