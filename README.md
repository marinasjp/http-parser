# http-parser

1. slow path removal:
int64_t errorFlags;
...
if(!errorFlags)
    sendOrderToExchange();

else
    HandleError(errorFlags);

2. template-based configuration instead of virtual functions 

(if you know the complete set of classes that will be instanciated)

virtual functions are fine if you do not know the possible classes that are going to be instanciated.

hooking it up using a factory function.

3. Lambda functions


4. Memory allocation is slow. Reuse and recycle objects. 

free is very expensive

reusing objects also helps avoid memory fragmentation

5. Exceptions are zero cost if they do not throw. 

Do not use exceptions for control flow

6. Branching is bad.

use template

do not remove every if, but in the hot path you can remove branches.

7. Multi-threading best avoided for latency-sensitive code

Keep data that you share to the absolute minimum when you need it.
Share copy of data instead of all of the data. 

8. Data lookups

if you are going to read something about the struct, ex: reading price from Instrument, then you are reading the whole 64 bytes because it is a float.

Put information that you always need to read on the cache line. If you always need to look at the marketId (from the Market struct), put that inside the instrument.

does not trample cache. 
worse on memory but faster.

9. Fast associative containers

std::unordered_map is fastest in c++

10. ((always_inline)) and ((noinlne))

inline means multiple definitions are permitted

you do not want logging code inline polluting the code - use noinline

11. keep cache hot

hotpath is exercised infrequently so cache has been trampled by non-hotpath data and instructions

we pretend everything we do is an order sent to the exchange. simulate an order going through system and stop it in the code. 

also trains hardware path predictor

12. do not share L3 cache. disable all but 1 core (or lock cache)

if multiple cores: choose neightbours carefully. what processes are running on the others. noisy neighbours should be moved to a different physical CPU

13. placement new can be slightly inefficient

unnecesary null pointer check 

14. small string optimization support

15. Static local variable initialiazion 

16. std::function may allocate and deallocate

find inplace function implementation

17. std:pow can be slow

glibc can be slow

function is transcedental, meaning it tries to be accurate.

18. try to avoid system calls. do not go into the kernel. 

How to measure

1. Profiling (particularly bottlenecks)
2. Benchmarking: timing speed of system

bad: gprof missed key events
bad: callgrind. does not catch IO
bad: microbenchmarking. not representative

they can be used but 

best: set up production system with a fake exchange. record the time of every packet that is going into the wire. after a day, analise packets and see which triggered an event.

hard to set up. 

