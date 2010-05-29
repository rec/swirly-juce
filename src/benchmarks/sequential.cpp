#include <time.h>
#include <stdio.h>
#include <stdlib.h>

typedef long long int int64;

static const int64 PRIME1 = 51234577L;
static const int64 PRIME2 = 20000003L;

int64 test(bool linear) {
  char* buffer = (char*) malloc(PRIME1);
  int64 time = -int64(clock());

  if (linear) {
    for (int64 i = 0; i < PRIME1; ++i) {
      int64 nonlinear = (i * PRIME2) % PRIME1;
      buffer[i] = char(nonlinear);
    }
  } else {
    for (int64 i = 0; i < PRIME1; ++i) {
      int64 nonlinear = (i * PRIME2) % PRIME1;
      buffer[nonlinear] = char(nonlinear);
    }
  }

  time += clock();
  free(buffer);

  return time;
}

int main() {
  printf("nonlinear: %lld\n", test(false));
  printf("linear: %lld\n", test(true));
}
