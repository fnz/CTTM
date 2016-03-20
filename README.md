## Compile Time Turing Machine
##### Going medieval on your compiler

Everyone interested in C++ template metaprogramming probably heard about Turing-completeness of C++ templates, and this project shows how to actually build a Turing Machine with templates and constant expressions, allowing things like this:

```cpp
#include "CTTM.h"

ADD_STATE(A);

ADD_RULE(A, 1, 0, Right, A);
ADD_RULE(A, 0, 1, Right, A);
ADD_RULE(A, Blank, Blank, Hold, Stop);

using tape = Tape<1, 1, 0, 1, Blank>;
using machine = Machine<A, 0, tape>;
using result = Run<machine>::type;

int main() {
    print(tape());
    print(result());
    return 0;
}
```

This particular program makes an inversion of the tape until it reaches a blank symbol and outputs
```
1 1 0 1 _ 
0 0 1 0 _ 
```

All the work is performed before the program is even launched. Please look into examples for more advanced stuff like busy beavers. Your programs are welcome, feel free to make a pull request!

Here is a blog post with a thorough explanation of the underlying mechanics: http://victorkomarov.blogspot.ru/2016/03/compile-time-turing-machine.html
