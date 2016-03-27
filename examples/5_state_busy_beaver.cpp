// 5-state 2-symbol busy beaver (actually best contender)
// According to Wikipedia it results in 4098 ones in 47176870 steps.
//
// Apple clang used about 20 gb swap and segfaulted during compilation on my machine *_*

#include "CTTM.h"

ADD_STATE(A);
ADD_STATE(B);
ADD_STATE(C);
ADD_STATE(D);
ADD_STATE(E);

ADD_RULE(A, Blank, 1, Right, B);
ADD_RULE(A, 1, 1, Left, C);

ADD_RULE(B, Blank, 1, Right, C);
ADD_RULE(B, 1, 1, Right, B);

ADD_RULE(C, Blank, 1, Right, D);
ADD_RULE(C, 1, Blank, Left, E);

ADD_RULE(D, Blank, 1, Left, A);
ADD_RULE(D, 1, 1, Left, D);

ADD_RULE(E, Blank, 1, Right, Stop);
ADD_RULE(E, 1, Blank, Left, A);


using tape = Tape<Blank>;
using machine = Machine<A, 0, tape>;
using result = Run<machine>::type;

int main() {
    print(result());
    return 0;
}