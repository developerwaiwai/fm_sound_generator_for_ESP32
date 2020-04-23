
#include <stdio.h>
#include <math.h>


void main(void)
{
 int I = 0;
 for (float i = 0; i < 2 * M_PI; i=i+(2*M_PI/1000.)){
  float s = sin(i);
  printf("%f, ", s);
  if(I%10 == 9) printf("\n");
  I++;
 }
 printf("\n");

}
