// A simple machine with only one state that turns zeroes
// into ones and vice versa until it reaches a blank symbol

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