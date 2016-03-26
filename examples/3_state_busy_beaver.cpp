// 3-state 2-symbol busy beaver
// outputs 6 ones in 14 steps

#include "CTTM.h"

ADD_STATE(A);
ADD_STATE(B);
ADD_STATE(C);

ADD_RULE(A, Blank, 1, Right, B);
ADD_RULE(A, 1, 1, Right, Stop);

ADD_RULE(B, Blank, Blank, Right, C);
ADD_RULE(B, 1, 1, Right, B);

ADD_RULE(C, Blank, 1, Left, C);
ADD_RULE(C, 1, 1, Left, A);

using tape = Tape<Blank>;
using machine = Machine<A, 0, tape>;
using result = Run<machine>::type;

int main() {
    result::print();
    return 0;
}
