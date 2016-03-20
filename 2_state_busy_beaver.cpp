// 2-state 2-symbol busy beaver
// outputs 4 ones in 6 steps

#include "CTTM.h"

ADD_STATE(A);
ADD_STATE(B);

ADD_RULE(A, 0, 1, Right, B);
ADD_RULE(A, 1, 1, Left, B);

ADD_RULE(B, 0, 1, Left, A);
ADD_RULE(B, 1, 1, Right, Stop);

using tape = Tape<Blank>;
using machine = Machine<A, 0, tape>;
using result = Run<machine>::type;

int main() {
    print(result());
    return 0;
}
