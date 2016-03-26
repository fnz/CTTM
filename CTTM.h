#pragma once

#include <iostream>
#include <type_traits>

#pragma mark - Blank

constexpr int Blank = -1;

#pragma mark - const_string

#include <array>

template<char... cs>
struct const_string {
  const static size_t length = sizeof...(cs);
  constexpr static std::array<char, length+1> toCStr() {
    return {{cs...}};
  }
};

template<class... ss>
struct const_concat {};

template<char... cs1, char... cs2, class... ss>
struct const_concat<const_string<cs1...>, const_string<cs2...>, ss...>
  : public const_concat<const_string<cs1..., cs2...>, ss...> {};

template<char... cs>
struct const_concat<const_string<cs...>>
{
  using type = const_string<cs...>;
};

struct positive{};
struct negative{};

template<int i, class Sign = positive, char... cs>
struct itocs : public itocs<i/10, Sign, '0'+i%10, cs...> {};

template<char... cs>
struct itocs<0, positive, cs...> {
  using type = const_string<cs...>;
};

template<char... cs>
struct itocs<0, negative, cs...> {
  using type = const_string<'-', cs...>;
};

template<>
struct itocs<Blank, positive> {
  using type = const_string<'_'>;
};

template<>
struct itocs<0, positive> {
  using type = const_string<'0'>;
};

template<int i>
struct itocs<i, typename std::enable_if<i < 0, positive>::type> : public itocs<-i, negative> {};

#pragma mark - Tape

template<int... xs>
class Tape;

template<class T, class S=const_string<>>
struct tape_to_cstring {};

template<int x, char... cs>
struct tape_to_cstring<Tape<x>, const_string<cs...>> {
  using type = typename const_concat< const_string<cs...>, const_string<' '>, typename itocs<x>::type>::type;
};

template<int x, int... xs, char... cs>
struct tape_to_cstring<Tape<x, xs...>,const_string<cs...>>
  : tape_to_cstring<Tape<xs...>, typename const_concat<const_string<cs...>, const_string<' '>, typename itocs<x>::type>::type>{};

template<int x, int... xs>
struct tape_to_cstring<Tape<x, xs...>,const_string<>>
  : tape_to_cstring<Tape<xs...>, typename const_concat<typename itocs<x>::type>::type>{};

template<int... xs>
class Tape {
public:
    using type = Tape<xs...>;
    constexpr static int length = sizeof...(xs);
    static void print() {
      std::cout << tape_to_cstring<Tape>::type::toCStr().data() << std::endl;
    }
};

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
