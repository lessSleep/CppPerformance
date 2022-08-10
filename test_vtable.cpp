#include "bits/stdc++.h"
using namespace std;
#include "benchmark.h"
// g++ test_vtable.cpp -O2 
// ./a.out

constexpr unsigned N = 40000;

class DynamicInterface {
public:
  virtual void tick(uint64_t n) = 0;
  virtual uint64_t getvalue() = 0;
};

class DynamicImplementation : public DynamicInterface {
  uint64_t counter;

public:
  DynamicImplementation() : counter(0) {}

  virtual void tick(uint64_t n) { counter += n; }

  virtual uint64_t getvalue() { return counter; }
};

template <typename Implementation> class CRTPInterface {
public:
  void tick(uint64_t n) { impl().tick(n); }

  uint64_t getvalue() { return impl().getvalue(); }

private:
  Implementation &impl() { return *static_cast<Implementation *>(this); }
};

class CRTPImplementation : public CRTPInterface<CRTPImplementation> {
  uint64_t counter;

public:
  CRTPImplementation() : counter(0) {}

  void tick(uint64_t n) { counter += n; }

  uint64_t getvalue() { return counter; }
};

void run_dynamic(DynamicInterface *obj) {
  for (unsigned i = 0; i < N; ++i) {
    for (unsigned j = 0; j < i; ++j) {
      obj->tick(j);
    }
  }
}

template <typename Implementation>
void run_crtp(CRTPInterface<Implementation> *obj) {
  for (unsigned i = 0; i < N; ++i) {
    for (unsigned j = 0; j < i; ++j) {
      obj->tick(j);
    }
  }
}

void pre() { return; }

int main() {

  // DynamicInterface *d =new DynamicImplementation();
  DynamicImplementation *d = new DynamicImplementation();

  BEST_TIME_NS(run_dynamic(d), pre(), 1, 1, 1);

  

  CRTPInterface<CRTPImplementation> *crtp =
      new CRTPInterface<CRTPImplementation>();

  BEST_TIME_NS(run_crtp<CRTPImplementation>(crtp), pre(), 1, 1, 1);
  cout<<d->getvalue()<<endl;
  cout<< crtp->getvalue()<<endl;

  return 0;
}