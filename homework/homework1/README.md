wsl* I tried to do direct pointer arithmatic to increase performance. It produced a speed up compared to the basic model, but was not faster than the final model. I think this method was slower since it resulted in two extra function calls to .data()
*((*(matrix.data() + i)).data() + j);

* Also tired doing a one loop version which was faster than basic, but not better than final model. I believe that this version was slower since it required two additional operations of / and % for every element access which offset the gain from removing a loop. 
sum += matrix[i / SIZE][i % SIZE];

* I suspect that compiler optimizations helped the code performance a lot since I wrote it in a very common format
* Also tried several different compiler optimizations to see performance improvements on the final model

## Compiler Performance differences
* g++ homework/homework1/homework1.cpp
  Basic Sum: 161102
  Basic Time: 112 milliseconds
  Optimized Sum: 161102
  Optimized Time: 76 milliseconds
* g++ homework/homework1/homework1.cpp -O3
Basic Sum: 56794
Basic Time: 6 milliseconds
Optimized Sum: 56794
Optimized Time: 3 milliseconds
* g++ homework/homework1/homework1.cpp -O3 -march=native -flto -funroll-loops
  Basic Sum: -98181
  Basic Time: 4 milliseconds
  Optimized Sum: -98181
  Optimized Time: 2 milliseconds