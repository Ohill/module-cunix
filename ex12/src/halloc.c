#include <errno.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

#define NALLOC 1024

union header
{
  struct i
  {
    union header *next;
    unsigned len;
  } meta;
  long x; 
};

static union header list;
static union header *first = NULL;

void free(void* ptr)
{
  if (ptr == NULL)
    return;
  
  union header *iter, *block;
  
  iter = first;
  block = (union header*)ptr - 1;
  
  while (block <= iter || block >= iter->meta.next) 
  {
    if ((block > iter || block < iter->meta.next) &&
        iter >= iter->meta.next) 
      break;
    iter = iter->meta.next;
  }
  
  if (block + block->meta.len == iter->meta.next)
  {
    block->meta.len += iter->meta.next->meta.len;
    block->meta.next = iter->meta.next->meta.next;
  } else   block->meta.next = iter->meta.next;
  
  if (iter + iter->meta.len == block)
  {
    iter->meta.len += block->meta.len;
    iter->meta.next = block->meta.next;
  } else  iter->meta.next = block;
  first = iter;
}

void *halloc(size_t size)
{
  if (size == 0)
    return NULL;
  
  union header *p, *prev;
  
  prev = first;
  unsigned true_size = (size+sizeof(union header)-1)/sizeof(union header)+1;
  
  if (first == NULL)
  {
    prev = &list;
    first = prev;
    list.meta.next = first;
    list.meta.len = 0;
  }
  
  p = prev->meta.next;
  
  while (1)
  {
    if (p->meta.len >= true_size)
    {
      if (p->meta.len == true_size)
        prev->meta.next = p->meta.next;
      else
      {
        p->meta.len -= true_size;
        p += p->meta.len;
        p->meta.len = true_size;
      }
      first = prev;
      return (void *)(p+1);
    }
    if (p == first)
    {
      char *page;
      union header *block;
      if (true_size < NALLOC) 
        true_size = NALLOC;
      page = sbrk((intptr_t) (true_size * sizeof(union header)));
      if (page == (char *)-1)
      {
        errno = ENOMEM;
        return NULL;
      }
      block = (union header *)page;
      block->meta.len = true_size;
      free((void *)(block + 1));
      p = first;
    }
    prev = p;
    p = p->meta.next;
  }
  
  return NULL;
}
