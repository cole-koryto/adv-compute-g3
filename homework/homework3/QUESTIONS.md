# Homework 3 – Questions

## Part 1 – Dynamic Allocation Basics

1. What happens if you use `delete` on the array?
> Undefined behavior. Compiler warns you but then "zsh: trace trap  ./build/main" happens when you run the program.
2. What happens if you forget to call `delete` at all?
> memory leak but no actual errors.
3. Can you call `delete` twice on the same pointer? Try it. What did you observe?
> Undefined behavior. gave "trade destroyed: " which is not expected. Then you get a sigtrap again.
---

## Part 3 – RAII Wrapper (`TradeHandle`)

1. Why do we delete the copy constructor?
> delete copy constructor to prevent double deletion issues. If we allow copying, two `TradeHandle` instances could point to the same `Trade`, leading to double deletion when both destructors run.
2. Why is move semantics allowed?
> Move semantics allows us to transfer ownership of the `Trade` resource without copying. This is fine because we delete the original object after moving (since in moving we dont care about the original object).
3. What happens if you don't define a destructor?

> the default destructor will be called and it may not properly release the `Trade` resource, leading to memory leaks. In our case, since `TradeHandle` manages a raw pointer, we need to define a destructor to ensure that the allocated memory is freed when the `TradeHandle` goes out of scope.

---

## Part 5 – Reflection & Discussion

1. What are the 3 biggest risks of manual memory management?
> 1. Memory leaks
> 2. Double deletion
> 3. Dangling pointers
2. What real-world problems could occur in HFT systems due to memory leaks or fragmentation?
> Memory leaks can lead to systems slowing down or crashing due to exhaustion of available memory. Fragmentation can lead to inefficient use of memory and increased latency in memory allocation, which is critical in HFT systems

3. How does RAII reduce the risk of bugs?
> RAII ties resource management to object lifetime, ensuring we don't have to manually free resources when we are done. we avoid memory leaks, double deletions, dangling pointers, accessing freed memory, etc.


4. What tradeoffs exist between manual and automatic memory management in performance-critical code?

> Manual: Pros are potentially better performance and more control over automatic. Cons are higher risk of bugs and more complex code.
> Automatic: pros are ease of use and reduced risk of bugs cons are potential performance overhead and less control over resource management.
> Essentialy, trading off ease of use and safety for potential perforamnce gains.

