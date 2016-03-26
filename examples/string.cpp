// According to Wikipedia, this is Alan Turing's first example
//
// A cyclic transition to Q1 is replaced with Stop
// to avoid infinite recursion

#include "CTTM.h"

int main() {
    using s1 = const_string<'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'>;
    using s2 = const_string<'!'>;
    std::cout << const_concat<s1, s2, s2, s2>::type::toCStr().data() << std::endl;
    std::cout << itocs<123>::type::toCStr().data() << std::endl;
    std::cout << itocs<-1>::type::toCStr().data() << std::endl;
    std::cout << itocs<0>::type::toCStr().data() << std::endl;
    std::cout << itocs<-1231412>::type::toCStr().data() << std::endl;
    Tape<1,2,3,0,-1,3,2,1,-3>::print();
}
