#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

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
  uint64 oldaddr, newaddr;
  int n;
  struct proc *p;
  p = myproc();
  if(argint(0, &n) < 0)
    return -1;
  oldaddr = p->sz;
  newaddr = oldaddr + n;
  if(n < 0){
    //n绝对值比addr大，将进程的全部用户空间释放，新的大小为0
    if(newaddr > oldaddr){
      newaddr = 0;
      uvmunmap(p->pagetable, 0, PGROUNDUP(oldaddr)/PGSIZE, 1);
    }
    else 
      uvmunmap(p->pagetable, PGROUNDUP(newaddr), 
          (PGROUNDUP(oldaddr)-PGROUNDUP(newaddr))/PGSIZE, 1);
    
  }else if(oldaddr + n >= MAXVA){//MAXVA为sbrk所能分配的最高地址2^38
    return -1;
  }
  p->sz = newaddr;//n大于零，只增加进程大小而不分配内存(延迟分配)
  return oldaddr;
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
