#include "types.h"
#include "stat.h"
#include "user.h"

#define TEST_CHILD_NAME "p2_print_repeat"
#define TEST_CHILD_PRINTER 0
#define TEST_CHILD_SLEEPER 1

char* argv[][7] = {
  {TEST_CHILD_NAME, "printer", "2000000", "400000", "0", "0" , 0},
  {TEST_CHILD_NAME, "sleeper", "4000000", "400000", "400000", "1", 0},
};

/*
  먼저 생성된 프로세스가 먼저 실행되어야 합니다.
*/
void test_basic() {
  for(int i=0; i<5; i++) {
    int pid = fork();
    if (pid == 0) {
      sleep(10); // wait for all children fork()ed
      exec(TEST_CHILD_NAME, argv[TEST_CHILD_PRINTER]);
    } else {
      printf(1, "proc %d created\n", pid);
    }
  }
  while(wait()!=-1); // wait all children
}
/*
  가장 먼저 생성된 프로세스가 주기적으로 잠듭니다.
  잠들고나면 다음으로 생성된 프로세스가 실행되어야하고, 깨어나면 다시 선점합니다.
*/
void test_sleep() {
  for(int i=0; i<5; i++) {
    int pid = fork();
    if (pid == 0) {
      sleep(10); // wait for all children fork()ed
      if (i == 0) 
        exec(TEST_CHILD_NAME, argv[TEST_CHILD_SLEEPER]);
      else 
        exec(TEST_CHILD_NAME, argv[TEST_CHILD_PRINTER]);
    } else {
      printf(1, "proc %d created\n", pid);
    }
  }
  while(wait()!=-1); // wait all children
}

int 
main(int argc, char* argv[]) {
  printf(1, "-----------test_basic----------\n");
  test_basic();
  printf(1, "-----------test_sleep----------\n");
  test_sleep();
  printf(1, "-----------test_finish----------\n");
  exit();
}
