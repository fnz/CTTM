// Given a tape with a binary number this machine will perform an increment
//
// This example illustrates how to wrap a particular setup into operation class
// and perform several consecutive runs.

#include "CTTM.h"

ADD_STATE(S0);
ADD_STATE(S1);
ADD_STATE(S2);

ADD_RULE(S0, Blank, Blank, Left, S1);
ADD_RULE(S0, 0, 0, Right, S0);
ADD_RULE(S0, 1, 1, Right, S0);

ADD_RULE(S1, Blank, 1, Right, S2);
ADD_RULE(S1, 0, 1, Left, S2);
ADD_RULE(S1, 1, 0, Left, S1);

ADD_RULE(S2, Blank, Blank, Hold, Stop);
ADD_RULE(S2, 0, 0, Right, S2);
ADD_RULE(S2, 1, 1, Right, S2);

template<class>
class Increment { };

template<int... xs>
class Increment<Tape<xs...>> {
public:
    using type = typename Run<Machine<S0, Tape<xs...>::length - 1, Tape<xs...>>>::type;
};

using tape = Tape<Blank, 1, 1, 0, Blank>;

int main() {
    print(tape());
    print(Increment<tape>::type());
    print(Increment<Increment<tape>::type>::type());
    print(Increment<Increment<Increment<tape>::type>::type>::type());
    print(Increment<Increment<Increment<Increment<tape>::type>::type>::type>::type());
    print(Increment<Increment<Increment<Increment<Increment<tape>::type>::type>::type>::type>::type());
    return 0;
}