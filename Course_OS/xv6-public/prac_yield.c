#include "types.h"
#include "defs.h"
// yield is signed for systemcall
void sys_yield(void)
{
	yield();
}
