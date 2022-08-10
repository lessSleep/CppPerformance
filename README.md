# CppPerformance

C and cpp Performance test

```
lingzhang@lingzhang-e470:~/a00-1$ g++ test_vtable.cpp -O3
lingzhang@lingzhang-e470:~/a00-1$ ./a.out 
run_dynamic(d)                                              :  1340293238 ns total,  1340293248.00 ns per byte  or 0.00 GB/s 
run_crtp<CRTPImplementation>(crtp)                          :  170734272 ns total,  170734272.00 ns per byte  or 0.00 GB/s 
10665866680000
10665866680000
lingzhang@lingzhang-e470:~/a00-1$ ./a.out 
run_dynamic(d)                                              :  1346602423 ns total,  1346602368.00 ns per byte  or 0.00 GB/s 
run_crtp<CRTPImplementation>(crtp)                          :  157252348 ns total,  157252352.00 ns per byte  or 0.00 GB/s 
10665866680000
10665866680000
lingzhang@lingzhang-e470:~/a00-1$ g++ test_vtable.cpp -O2
lingzhang@lingzhang-e470:~/a00-1$ ./a.out 
run_dynamic(d)                                              :  1348599309 ns total,  1348599296.00 ns per byte  or 0.00 GB/s 
run_crtp<CRTPImplementation>(crtp)                          :  320050275 ns total,  320050272.00 ns per byte  or 0.00 GB/s 
10665866680000
10665866680000
lingzhang@lingzhang-e470:~/a00-1$ ./a.out 
run_dynamic(d)                                              :  1345130214 ns total,  1345130240.00 ns per byte  or 0.00 GB/s 
run_crtp<CRTPImplementation>(crtp)                          :  359081790 ns total,  359081792.00 ns per byte  or 0.00 GB/s 
10665866680000
10665866680000
```
