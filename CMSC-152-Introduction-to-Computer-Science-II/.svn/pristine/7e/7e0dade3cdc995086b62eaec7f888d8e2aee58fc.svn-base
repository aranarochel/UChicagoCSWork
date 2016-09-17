/* Jaime Arana-Rochel */
/* CS152, Winter 2013 */
/* Lab3 */

#include <stdio.h>
#include "expt.h"

void evidence_expt()
{
  printf("*** testing expt\n");
  printf("- expecting 1: %ld\n", expt(2,0));
  printf("- expecting 8: %ld\n", expt(2,3));
  printf("- expecting 25: %ld\n", expt(5,2));
}

void evidence_ss()
{
  printf("*** testing ss\n");
  printf("- expecting 1: %ld\n", ss(2,0));
  printf("- expecting 8: %ld\n", ss(2,3));
  printf("- expecting 25: %ld\n", ss(5,2));
}

void evidence_ssm()
{
  printf("*** testing ssm\n");
  printf("- expecting 24: %d\n", ssm(2,10,1000));
  printf("- expecting 0: %d\n", ssm(5,2,5));
  printf("- expecting 0: %d\n", ssm(2,15,16));
}

int main(int argc, char *argv[])
{
  evidence_expt();
  evidence_ss();
  evidence_ssm();
  return 0;
}

