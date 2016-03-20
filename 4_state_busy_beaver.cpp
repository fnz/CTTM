// 4-state 2-symbol busy beaver
// outputs 13 ones in 107 steps

#include "CTTM.h"

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
