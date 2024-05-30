#include <string.h>
#include <unity/unity.h>
#include <unity/unity_internals.h>
#include "arena.h"

void setUp() {}
void tearDown() {}

void arenatest() {
  Arena_t arena = {0};
  char *mystringpointer = arenaAlloc(&arena, sizeof(char)*3);
  mystringpointer = "hi";
  UNITY_TEST_FAIL(strcmp("hi", mystringpointer), "oh no");
  arenaDestroy(&arena);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(arenatest);
}

