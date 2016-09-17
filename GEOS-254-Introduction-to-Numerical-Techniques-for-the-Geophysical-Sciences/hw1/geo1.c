#include <stdio.h>

int main(int argc, char* argv[])
{
  float e = 1.0;
  int i = 0;

  
  for(i = 0; i < 1450; i++) {
    printf("Var e = %f | 1.0 + %f = %f\n", e, e, 1.0f+e);

    e = 0.99*e;
  }
  
  return 0;
}
