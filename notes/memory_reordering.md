# Memory Reordering

### Hardware Memory Ordering in x86 Processors

The term memory ordering refers to the order in which the processor issues 
reads (loads) and writes (stores) through the system bus to system memory.

For example, the Intel386 processor enforces program ordering 
(generally referred to as strong ordering), where reads and writes are issued 
on the system in the order thev occur in the instruction stream.

But the hardware may reorder the instructions for some optimizations. Sometimes
reads could go ahead of buffered writes.

<mark> Reads may be reordered with older writes to different memory locations 
but not with older writes to same memory location.</mark>

That is, if we write to location 1 and read from location 2, then the read from
location 2 could become globally visible before write to location 1.

```
let x = y = 0

processor 0                     processor 1
x = 1                           y = 1
print y                         print x

output = (0, 0) is possible
```

<mark> Stores are usually buffered before being sent to memory (L1 cache). We 
prioritise loads more than stores. Since they are on critical path. The instructions
are waiting for the data to be loaded before they can run. 
Although if a store followed by a load are for same memory location then we will
definitely follow program order.</mark>

### Software Reordering
Compiler can also sometimes reorder instructions in our program for optimizations.
For example, store to 2 different memory locations can be reordered by our
compiler.

### Avoid memory reordering
Using fence instructions. 

#### Asking compiler not to reorder
`asm volatile("" : : : "memory");` Fake instruction that asks compiler to not
reorder any memory instruction around this barrier. A hint to compiler that 
whole of the memory can be touched by this instruction: hence don't do any
reordering. 

*In this case the hardware can still reorder instructions, even though we asked
our compiler to not reorder! Hence we will have to use hardware barriers.*

```cpp
#include <emmintrin.h>
void _mm_mfence (void) // Use this instruction as a barrier to prevent re-ordering in the hardware!
```

Pertorm serializing operation on all `load-from-memory` and `store-to-memory` 
instructions that were issued prior to this instruction. 
Guarantees that every memory access that precedes, in program order the memory 
fence instruction is globally visible before any memory instruction which 
follows the fence in program order.

*It drains the `store buffer`, before any following `loads` can go into memory.*




