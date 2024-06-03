#include <string.h>
#include <unity/unity.h>
#include <unity/unity_internals.h>
#include "arena.h"

void setUp() {}
void tearDown() {}

void arenatest1_allocanddestroy() {
  Arena_t *arena = arenaInit();
  char *mystringpointer = arenaAlloc(arena, sizeof(char)*3);
  mystringpointer = "hi";
  TEST_ASSERT_TRUE(!strcmp(mystringpointer, "hi"));
  arenaDestroy(arena);
}

void arenatest2_realloc() {
  Arena_t *arena = arenaInit();
  int *myarray = arenaAlloc(arena, sizeof(int)*8);
  int *myarraypre;
  myarray[0] = 0;
  myarray[1] = 1;
  myarraypre = myarray;
  myarray = arenaRealloc(arena, myarray, sizeof(int)*8, sizeof(int)*16);
  TEST_ASSERT_EQUAL_INT_ARRAY(myarraypre, myarray, 2);
  arenaDestroy(arena);
}

void arenatest3_astrdup() {
  Arena_t *arena = arenaInit();
  char mystring[3] = "hi";
  char *dollysstring = astrdup(arena, mystring);
  TEST_ASSERT_TRUE(!strcmp(mystring, dollysstring));
  arenaDestroy(arena);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(arenatest1_allocanddestroy);
  RUN_TEST(arenatest2_realloc);
  RUN_TEST(arenatest3_astrdup);
  UNITY_END();
}

