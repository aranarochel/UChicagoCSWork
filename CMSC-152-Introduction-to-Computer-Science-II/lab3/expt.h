/* Jaime Arana-Rochel , aranarochel */
/* CS152, Winter 2013 */
/* Lab3 */ 

/* expt : int (unsigned int) -> (long int) */
/* linear time recursive exponentiation */
long int expt(int a, unsigned int n);

/* ss : int (unsigned int) -> (long int) */
/* exponentiation by successive squaring */
long int ss(int a, unsigned int n);

/* ssm : int (unsigned int) (unsigned int) -> int */
/* exponentiation modulo m by successive squaring */
int ssm(int a, unsigned int n, unsigned int m);
