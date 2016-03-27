## Compile Time Turing Machine
##### Going medieval on your compiler

Everyone interested in C++ template metaprogramming probably heard about Turing-completeness of C++ templates, and this project shows how to actually build a Turing Machine with templates and constant expressions, allowing things like this:

```cpp
ADD_STATE(A);
ADD_STATE(B);
ADD_STATE(C);
ADD_STATE(D);

ADD_RULE(A, Blank, 1, Right, B);
ADD_RULE(A, 1, 1, Left, B);

ADD_RULE(B, Blank, 1, Left, A);
ADD_RULE(B, 1, Blank, Left, C);

ADD_RULE(C, Blank, 1, Right, Stop);
ADD_RULE(C, 1, 1, Left, D);

ADD_RULE(D, Blank, 1, Right, D);
ADD_RULE(D, 1, Blank, Right, A);

using tape = Tape<Blank>;
using machine = Machine<A, 0, tape>;
using result = Run<machine>::type;

int main() {
    print(result());
    return 0;
}
```

This is a 4-state 2-symbol busy beaver example (https://en.wikipedia.org/wiki/Busy_beaver). The result is
```
1 _ 1 1 1 1 1 1 1 1 1 1 1 1 
```

You can check out ideone run here: https://ideone.com/MvBU3Z. All the work is performed before the program is even launched!

Here is a blog post with a thorough explanation of the underlying mechanics: http://victorkomarov.blogspot.ru/2016/03/compile-time-turing-machine.html
