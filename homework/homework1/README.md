* I tried to do direct pointer arithmatic to increase performance. It produced a speed up compared to the basic model, but was not faster than the final model.
*((*(matrix.data() + i)).data() + j);

* Also tired doing a one loop version which was faster than basic, but not better than final model
sum += matrix[i / SIZE][i % SIZE];

* I suspect that compiler optimizations helped the code performance a lot since I wrote it in a very common format
* Also tried O3 flag for max performance. 

* Mention final perform metrics
* 