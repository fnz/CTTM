#pragma once

#include <iostream>

#pragma mark - Tape

constexpr int Blank = -1;

template<int... xs>
class Tape {
public:
    using type = Tape<xs...>;
    constexpr static int length = sizeof...(xs);
};

#pragma mark - Print

template<class T>
void print(T);

template<>
void print(Tape<>) {
    std::cout << std::endl;
}

template<int x, int... xs>
void print(Tape<x, xs...>) {
    if (x == Blank) {
        std::cout << "_ ";
    } else {
        std::cout << x << " ";
    }
    print(Tape<xs...>());
}

#pragma mark - Concatenate

template<class, class>
class Concatenate;

template<int... xs, int... ys>
class Concatenate<Tape<xs...>, Tape<ys...>> {
public:
    using type = Tape<xs..., ys...>;
};

#pragma mark - Invert

template<class>
class Invert;

template<>
class Invert<Tape<>> {
public:
    using type = Tape<>;
};

template<int x, int... xs>
class Invert<Tape<x, xs...>> {
public:
    using type = typename Concatenate<
        typename Invert<Tape<xs...>>::type,
        Tape<x>
    >::type;
};

#pragma mark - Read

template<int, class>
class Read;

template<int n, int x, int... xs>
class Read<n, Tape<x, xs...>> {
public:
    using type = typename std::conditional<
        (n == 0),
        std::integral_constant<int, x>,
        Read<n - 1, Tape<xs...>>
    >::type::type;
};

#pragma mark - N first and N last

template<int, class>
class NLast;

template<int n, int x, int... xs>
class NLast<n, Tape<x, xs...>> {
public:
    using type = typename std::conditional<
        (n == sizeof...(xs)),
        Tape<xs...>,
        NLast<n, Tape<xs...>>
    >::type::type;
};

template<int, class>
class NFirst;

template<int n, int... xs>
class NFirst<n, Tape<xs...>> {
public:
    using type = typename Invert<
        typename NLast<
            n, typename Invert<Tape<xs...>>::type
        >::type
    >::type;
};

#pragma mark - Write

template<int, int, class>
class Write;

template<int pos, int x, int... xs>
class Write<pos, x, Tape<xs...>> {
public:
    using type = typename Concatenate<
        typename Concatenate<
            typename NFirst<pos, Tape<xs...>>::type,
            Tape<x>
        >::type,
        typename NLast<(sizeof...(xs) - pos - 1), Tape<xs...>>::type
    >::type;
};

#pragma mark - Move

template<int, class>
class Hold;

template<int pos, int... xs>
class Hold<pos, Tape<xs...>> {
public:
    constexpr static int position = pos;
    using tape = Tape<xs...>;
};

template<int, class>
class Left;

template<int pos, int... xs>
class Left<pos, Tape<xs...>> {
public:
    constexpr static int position = typename std::conditional<
        (pos > 0),
        std::integral_constant<int, pos - 1>,
        std::integral_constant<int, 0>
    >::type();

    using tape = typename std::conditional<
        (pos > 0),
        Tape<xs...>,
        Tape<Blank, xs...>
    >::type;
};

template<int, class>
class Right;

template<int pos, int... xs>
class Right<pos, Tape<xs...>> {
public:
    constexpr static int position = pos + 1;

    using tape = typename std::conditional<
        (pos < sizeof...(xs) - 1),
        Tape<xs...>,
        Tape<xs..., Blank>
    >::type;
};

#pragma mark - States

template <int>
class Stop {
public:
    constexpr static int write = -1;
    template<int pos, class tape> using move = Hold<pos, tape>;
    template<int x> using next = Stop<x>;
};

#define ADD_STATE(_state_)      \
template<int>                   \
class _state_ { };

#define ADD_RULE(_state_, _read_, _write_, _move_, _next_)          \
template<>                                                          \
class _state_<_read_> {                                             \
public:                                                             \
    constexpr static int write = _write_;                           \
    template<int pos, class tape> using move = _move_<pos, tape>;   \
    template<int x> using next = _next_<x>;                         \
};

#pragma mark - Machine

template<template<int> class, int, class>
class Machine;

template<template<int> class State, int pos, int... xs>
class Machine<State, pos, Tape<xs...>> {
    constexpr static int symbol = typename Read<pos, Tape<xs...>>::type();
    using state = State<symbol>;

    template<int x>
    using nextState = typename State<symbol>::template next<x>;

    using modifiedTape = typename Write<pos, state::write, Tape<xs...>>::type;
    using move = typename state::template move<pos, modifiedTape>;

    constexpr static int nextPos = move::position;
    using nextTape = typename move::tape;

public:
    using step = Machine<nextState, nextPos, nextTape>;
};

#pragma mark - Run

template<class>
class Run;

template<template<int> class State, int pos, int... xs>
class Run<Machine<State, pos, Tape<xs...>>> {
    using step = typename Machine<State, pos, Tape<xs...>>::step;

public:
    using type = typename std::conditional<
        std::is_same<State<0>, Stop<0>>::value,
        Tape<xs...>,
        Run<step>
    >::type::type;
};
