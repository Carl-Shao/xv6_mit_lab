#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

int pgaccess(pagetable_t pagetable, uint64 va);

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 addr;
  int n;
  int bitmask;

  if(argaddr(0, &addr) < 0) {
    return -1;
  }
  if(argint(1, &n) < 0) {
    return -1;
  }
  if(n < 0 || n > 32) {
    return -1;
  }
  if(argint(2, &bitmask) < 0) {
    return -1;
  }

  int result = 0;
  struct proc *p = myproc();
  for(int i = 0; i < n; i++) {
    int va = addr + i * PGSIZE;
    int abit = pgaccess(p->pagetable, va);
    result = result | abit << i;
  }

  if(copyout(p->pagetable, bitmask, (char*)&result, sizeof(result)) < 0){
    return -1;
  }
  return 0;
}


uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
