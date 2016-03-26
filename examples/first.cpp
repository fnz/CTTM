// According to Wikipedia, this is Alan Turing's first example
//
// A cyclic transition to Q1 is replaced with Stop
// to avoid infinite recursion

#include "CTTM.h"

ADD_STATE(Q1);
ADD_STATE(Q2);
ADD_STATE(Q3);
ADD_STATE(Q4);

ADD_RULE(Q1, Blank, 0, Right, Q2);
ADD_RULE(Q2, Blank, Blank, Right, Q3);
ADD_RULE(Q3, Blank, 1, Right, Q4);
ADD_RULE(Q4, Blank, Blank, Hold, Stop);

using tape = Tape<Blank>;
using machine = Machine<Q1, 0, tape>;
using result = Run<machine>::type;

int main() {
    result::print();
    return 0;
}
