/*
  test for tanh_sse.h

  compile with:
    gcc -msse -mfpmath=387 -W -Wall
    gcc -msse -mfpmath=387,sse -W -Wall
    cl /arch:SSE /W4
*/

#include "stdio.h"
#include "math.h"
#include "tanh_sse.h"
#define _TANH_CLAMP_INDIVIDUAL

int main(void)
{
  register double x;

  union
  {
    __m128  v;
    double   f[4];
  } u;

  x = -6.f;
  while (x < 6.f)
  {
    u.v = _mm_set1_ps(x);
    u.v = fast_tanh_sse(u.v);

    printf("x: %f\n", x);
    printf("fast_tanh: %f\n", fast_tanh(x));
    printf("fast_tanh_sse: %f\n", u.f[0]);
    printf("libm: %f\n", tanhf(x));
    printf("--------\n");
    x += 0.1f;
  }

  return 0;
}
