#include <stdio.h>
#include "InterruptDelegate.h"

template <class T>
struct foobar
{
  T i;
  void foo() { printf("%s %d\n",__PRETTY_FUNCTION__, i);}
  void bar() const{ printf("%s %d\n",__PRETTY_FUNCTION__, i);}
};

void hello() { printf("%s\n",__PRETTY_FUNCTION__);}
void helloWELT() { printf("%s\n",__PRETTY_FUNCTION__);}

GenInterrupt(MICHAEL);

int main()
{
  foobar<int> fb={0};
  foobar<int> *pfb = &fb;
  
  redirectISRF(MICHAEL,&hello);
  call(MICHAEL);
  redirectISRF(MICHAEL,&helloWELT);
  call(MICHAEL);
  
  redirectISRMF(MICHAEL, foobar<int> ,&foobar<int>::foo, pfb);
  call(MICHAEL);
  
  redirectISRMF(MICHAEL, foobar<int> const,&foobar<int>::bar, pfb);
  call(MICHAEL);
  
  return 0;
}
