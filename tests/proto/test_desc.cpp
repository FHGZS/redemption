/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Product name: redemption, a FLOSS RDP proxy
Copyright (C) Wallix 2010-2019
Author(s): Jonathan Poelen
*/

#include "proto/desc.hpp"
#include "utils/only_type.hpp"
#include "utils/stream.hpp"
#include "utils/sugar/bytes_view.hpp"
#include "utils/sugar/numerics/safe_conversions.hpp"
#include "mpl/kvasir/mpl/mpl.hpp"

#include <iomanip>
#include <iostream>
#include <string_view>


template<class T>
std::string_view type_name()
{
    std::string_view sv(__PRETTY_FUNCTION__);
#ifdef __clang__
    sv.remove_suffix(1);
    sv.remove_prefix(34);
#elif defined(__GNUG__)
    sv.remove_suffix(50);
    sv.remove_prefix(39);
#endif
    return sv;
}

template<class T>
std::string_view type_name(T const&)
{
    return type_name<T>();
}

template<class... Xs>
void print(Xs const&... xs)
{
    ((std::cout << xs), ...);
}

template<class... Xs>
void println(Xs const&... xs)
{
    print(xs..., "\n");
}

template<class Xs>
void print_type(char const* prefix = "")
{
    println(prefix, type_name<Xs>());
}

template<class... Xs>
void print_def(proto::tuple<Xs...> const&)
{
    ((println("  ", type_name<Xs>())), ...);
}


namespace proto
{
    class RawData;

    template<template<class...> class Concept>
    struct concept_check
    {
        template<class... Xs>
        struct t
        {
            static_assert(Concept<Xs...>::value);
            static const bool value = true;
        };
    };

#define PROTO_ASSERT_TYPES(concept, ...) static_assert(( \
    ::proto::concept_check<concept>::t<__VA_ARGS__>::value && ...));

    template<class ...Data>
    auto raw_data(Data const& ...data)
    {
        PROTO_ASSERT_TYPES(is_data, Data);
        return tuple<Data...>{data...};
    }

    template<class T> struct type_only {};
    template<class T> struct value_only {};
    struct start_range { using proto_basic_type = start_range; };
    struct stop_range { using proto_basic_type = stop_range; };
}

template<class...> struct names {};

template<class T>
struct extract_name_impl;

template<class T, class... Names>
struct extract_name_impl<proto::value<T, Names...>>
{
    using type = names<Names...>;
};

template<class T, class... Names>
struct extract_name_impl<proto::param<T, Names...>>
{
    using type = names<Names...>;
};

template<class T>
using extract_name = typename extract_name_impl<T>::type;

using proto::name_t;

template<class T>
struct wrap
{
    T value;
};

template<class... Name>
void print_names(names<Name...>)
{
    (print(type_name<Name>(), ", "), ...);
}

template<class Name, class X>
X& get_value(wrap<proto::value<X, Name> const&> const& v)
{
    return v.get().value;
}

template<class Tuple, class... Name>
void print_variable(Tuple const& t, names<Name...>)
{
    (print(get_value<Name>(t), ", "), ...);
}


template<class BasicType>
struct basic_type_to_cpp_type_impl;

template<class Integral, class Endianess>
struct basic_type_to_cpp_type_impl<proto::datas::types::Integer<Integral, Endianess>>
{
    using type = Integral;
};

template<class Size, class Data, class ZeroPolicy>
struct basic_type_to_cpp_type_impl<proto::datas::types::String<Size, Data, ZeroPolicy>>
{
    using type = bytes_view;
};

template<class BasicType>
using basic_type_to_cpp_type = typename basic_type_to_cpp_type_impl<BasicType>::type;

template<class Data, class... Names, class Tuple>
auto writable_variable(proto::param<Data, Names...> const&, Tuple&& t)
{
    return basic_type_to_cpp_type<typename Data::proto_basic_type>(get_value<Names>(t)...);
}

template<class Int, class Endianess>
void write_buf(proto::datas::types::Integer<Int, Endianess> const&, OutStream& out, Int x)
{
    if constexpr (sizeof(Int) == 1) {
        out.out_uint8(x);
    }
    else {
        out.out_uint16_be(x);
    }
}

template<class Buf, class... Param, class... Xs>
void inplace_emit(Buf& buf, proto::tuple<Param...> const& def, Xs const&... xs)
{
    PROTO_ASSERT_TYPES(proto::is_param, Param);
    PROTO_ASSERT_TYPES(proto::is_value, Xs);

    proto::tuple<wrap<Xs const&>...> t{{xs}...};

    OutStream out(buf);

    ([&]{
        using data_type = typename Param::data_type;
        using basic_type = typename data_type::proto_basic_type;

        println(
            "  ", type_name<name_t<Param>>(),
            "| ", type_name<basic_type>(),
            " -> ", get_value<name_t<Param>>(t)
        );

        write_buf(basic_type{}, out, writable_variable(static_cast<Param const&>(def), t));
    }(), ...);
}


class native {};
class view_buffer {};
template<class T> struct ref { T& x; };
template<class T> ref(T&) -> ref<T>;

PROTO_IS_TYPE1(ref);


inline std::ostream& operator<<(std::ostream& out, native const&)
{
    return out << "native";
}

template<class T>
inline std::ostream& operator<<(std::ostream& out, ref<T> const& ref)
{
    return out << "ref(" << ref.x << ")";
}

template<class BasicType, class Binding, class UnCVBind>
struct real_type_impl;

template<class BasicType, class Binding>
struct real_type_impl<BasicType, Binding, native>
{
    using type = basic_type_to_cpp_type<BasicType>;
};

template<class BasicType, class Binding>
struct real_type_impl<BasicType, Binding, view_buffer>
{
    using type = bytes_view;
};

template<class BasicType, class Binding>
using real_type = typename real_type_impl<BasicType, Binding,
    std::remove_const_t<std::remove_reference_t<Binding>>>::type;

template<class... Name, class F>
void each_name(names<Name...>, F&& f)
{
    return (f(Name()), ...);
}

template<class Int, class Endianess>
auto read_buf(proto::datas::types::Integer<Int, Endianess>, InStream& in, native)
{
    if constexpr (sizeof(Int) == 1) {
        return in.in_uint8();
    }
    else {
        return in.in_uint16_be();
    }
}

template<class Integer, class charset>
auto read_buf(proto::datas::types::StringSize<Integer, charset>, InStream& in, native)
{
    return read_buf(Integer{}, in, native());
}

template<class StringSize, class StringData, class ZeroPolicy>
auto read_buf(proto::datas::types::String<StringSize, StringData, ZeroPolicy>, InStream& in, native)
{
    auto n = read_buf(StringSize{}, in, native());
    return in.in_skip_bytes(n);
}

template<class Int, class Endianess>
auto read_buf(proto::datas::types::Integer<Int, Endianess>, InStream& in, view_buffer)
{
    return in.in_skip_bytes(sizeof(Int));
}

template<class Data>
auto readable_variable_impl(native)
{
    return native();
}

template<class Data>
auto readable_variable_impl(view_buffer)
{
    return view_buffer();
}

template<class Data, class Name, class Tuple>
auto readable_variable(proto::param<Data, Name> const&, Tuple&& t)
{
    return readable_variable_impl<Data>(get_value<Name>(t));
}

template<class Buf, class... Params, class... Xs>
auto inplace_recv(Buf& buf, proto::tuple<Params...> const& def, Xs const&... xs)
{
    PROTO_ASSERT_TYPES(proto::is_param, Params);
    PROTO_ASSERT_TYPES(proto::is_value, Xs);
    proto::tuple<std::reference_wrapper<Xs const>...> t{{xs}...};

    InStream in(buf);

    auto f = [&](auto const& p){
        using Param = std::remove_const_t<std::remove_reference_t<decltype(p)>>;
        using data_type = typename Param::data_type;
        using basic_type = typename data_type::proto_basic_type;

        println(
            "  ", type_name<name_t<Param>>(),
            "| ", type_name<decltype(get_value<name_t<Param>>(t))>(),
            " : ", type_name<basic_type>(),
            " => ", type_name<real_type<basic_type, decltype(get_value<name_t<Param>>(t))>>()
        );

        return typename decltype(Param::name::mem()(proto::wrap_type<decltype(
            read_buf(basic_type{}, in, readable_variable(static_cast<Param const&>(def), t))
        )>()))::type{
            read_buf(basic_type{}, in, readable_variable(static_cast<Param const&>(def), t))
        };
    };
    return proto::tuple{f(static_cast<Params const&>(def))...};

    // (println("  ",
    //     type_name<typename Data::name>(), " = ",
    //     type_name<typename decltype(Data::name::mem()(proto::wrap_type<real_type<typename Data::proto_basic_type, decltype(get_value<typename Data::name>(t))>>()))::type>(),
    //     "] :",
    //     type_name<typename decltype(Data::name::mem()(proto::wrap_type<real_type<typename Data::proto_basic_type, decltype(get_value<typename Data::name>(t))>>()))::type::type>()
    //     // " <- ",
    //     // xs.value
    // ), ...);
}

// type => has_proto_basic_type
//      proto_basic_type_list (optional) -> group type pack<u8, u8>
//
// name => has_proto_label_name
//
// a := label<name>
// d := data<type>
//
// d(x) := var<T>
// nx := a(x) => named<name, value<void, x>> ????
// dn := d[a] => data<type, name>
//
// d(x) => value<type, x>
// d(nx) => value<type, named<name, x>>
// dn(x) => named<name, value<type, x>>
//

#include <array>

inline std::ostream& operator<<(std::ostream& out, bytes_view const& av)
{
    out << "[" << av.size() << "]";
    auto t = "0123456789abcdef";
    for (auto c : av) {
        out << t[c>>4] << t[c&0xf];
    }
    return out;
}

template<class Buf>
void dump(Buf& buf)
{
    auto t = "0123456789abcdef";
    print("dump(size=", buf.size(), "): 0x");
    for (auto c : buf) {
        print(t[c>>4], t[c&0xf]);
    }
    println();
}


namespace mp = kvasir::mpl;
namespace mpe = kvasir::mpl::eager;

// template<class F, class... args>
// struct bind_front
// {
//     template<class... xs>
//     using f = typename F::template f<args..., xs...>;
// };
//
// template<class F, class... args>
// struct bind_back
// {
//     template<class... xs>
//     using f = typename F::template f<xs..., args...>;
// };

template<class C = mp::identity>
struct va_plus
{
    template<class... xs>
    using f = typename C
      ::template f<mp::integral_constant<decltype((xs::value + ...)), (xs::value + ...)>>;
};

namespace test
{
    using namespace proto;

    template<class C = mp::identity> using mp_tpl0 = mp::unpack<mp::front<C>>;
    template<class C = mp::identity> using mp_tpl1 = mp::unpack<mp::at1<C>>;

    template<class Params, class Values>
    struct Definition;

    namespace detail
    {
        class discard {};
        template<class T> using is_discard = typename std::is_same<T, discard>::type;
        template<class T> inline constexpr bool is_discard_v = std::is_same<T, discard>::value;


        template<class X, class Name>
        struct value_to_definition_param;

        template<class T, class Name>
        struct value_to_definition_param<data<T>, Name>
        {
            using type = param<T, Name>;
        };

        template<class T, class Name>
        struct value_to_definition_param<type_only<T>, Name>
        {
            using type = param<T, Name>;
        };

        template<template<class...> class Tpl, class Name>
        struct value_to_definition_param<value_data<Tpl>, Name>
        {
            using type = discard;
        };

        template<class Name>
        struct value_to_definition_param<start_range, Name>
        {
            using type = discard;
        };

        template<class Name>
        struct value_to_definition_param<stop_range, Name>
        {
            using type = discard;
        };


        template<class X, class Name>
        struct value_to_definition_value;

        template<class T, class Name>
        struct value_to_definition_value<type_only<T>, Name>
        {
            using type = discard;
        };

        template<class T, class Name>
        struct value_to_definition_value<data<T>, Name>
        {
            using type = lazy_value<as_param, Name>;
        };

        template<template<class...> class Tpl, class Name>
        struct value_to_definition_value<value_data<Tpl>, Name>
        {
            using type = lazy_value<Tpl<as_param>, Name>;
        };

        template<class Name>
        struct value_to_definition_value<start_range, Name>
        {
            using type = lazy_value<start_range, Name>;
        };

        template<class Name>
        struct value_to_definition_value<stop_range, Name>
        {
            using type = lazy_value<stop_range, Name>;
        };


        template<class V>
        using value_to_definition_value_t = typename detail::value_to_definition_value<
            std::decay_t<value_type_t<V>>, name_t<V>>::type;

        template<class V>
        using value_to_definition_param_t = typename detail::value_to_definition_param<
            std::decay_t<value_type_t<V>>, name_t<V>>::type;


        template<class X>
        auto extract_lazy_value2(X const& x)
        {
            if constexpr (is_lazy_value_v<X>)
            {
                return x;
            }
            else if constexpr (is_param_and_lazy_value_v<X>)
            {
                return lazy_value<as_param, name_t<X>>{};
            }
            else
            {
                return discard{};
            }
        }

        template<class T> using type_t = typename T::type;
        template<class T> using index_t = typename T::index;
        template<class T> using names_t = typename T::names;

        template<class C = mp::identity> using mp_indexed_type_t = mp::unpack<mp::front<C>>;
        template<class C = mp::identity> using mp_indexed_index_t = mp::unpack<mp::at1<C>>;

        template<class C, class... xs>
        using mpc_remove_discard = mp::call<mp::remove_if<mp::same_as<detail::discard>, C>, xs...>;

        template<class X, class I>
        struct indexed : X
        {
            using type = X;
            using index = I;
        };

        template<class T, int n>
        using filled_sequence = mp::call<
            mp::make_int_sequence<mp::transform<mp::always<T>>>,
            mp::int_<n>>;

        template<class... x>
        struct set_difference
        {
            template<class... y>
            using f = mp::call<mp::join<>,
                mp::call<mp::conditional<std::is_same_v<
                    mp::call<mp::transform<mp::same_as<x>>, y...>,
                    filled_sequence<mp::false_, sizeof...(y)>
                >>, mp::list<x>, mp::list<>
            >...>;
        };

        template<class T>
        struct flat_sequence_impl
        {
            using type = mp::list<T>;
        };

        template<class... xs>
        struct flat_sequence_impl<mp::list<xs...>>
        {
            using type = mp::call<mp::join<>, typename flat_sequence_impl<xs>::type...>;
        };

        template<class C>
        struct flat_sequence
        {
            template<class... xs>
            using f = mp::call<mp::join<C>, typename flat_sequence_impl<xs>::type...>;
        };

        template<class... p>
        struct param_list
        {
            template<class C = mp::listify>
            using names = mp::call<C, mp::call<mp_tpl1<>, p>...>;

            struct param_must_be_unique : mp::call<mp_tpl1<>, p>...
            {
                using list = mp::list<p...>;
            };
        };

        using mp_lazy_value_names = mp::unpack<mp::pop_front<>>;

        template<class... v>
        struct lazy_value_list
        {
            template<class C = mp::listify>
            using names = mp::call<mp::transform<mp_lazy_value_names, mp::join<C>>, v...>;

            using list = mp::list<v...>;
        };

        struct no_error
        {
            template<class...>
            using f = no_error;

            static const bool checked = true;
        };

        template<class E>
        constexpr bool check_error(E)
        {
            static_assert(E::checked); return true;
        }

        template<class x, class y, class Error>
        void check_unused_params()
        {
            using l = typename x::template names<
                mp::cfe<detail::set_difference,
                    mp::cfe<y::template names>>>;

            using errtype = typename mp::conditional<std::is_same<l, mp::list<>>::value>
                ::template f<no_error, mp::unpack<Error>>;

            static_assert(check_error(mp::call<errtype, l>{}));
        }

        template<class x, class Pred, class Error>
        void check_pred()
        {
            using errtype = typename mp::conditional<mp::call<Pred, x>::value>
                ::template f<no_error, Error>;

            static_assert(check_error(mp::call<errtype, x>{}));
        }
    }

    using detail::type_t;

#define PROTO_ERROR_C(name) struct name { template<class... ctx> struct f{}; }

    namespace errors
    {
        PROTO_ERROR_C(some_parameters_are_unused);
        PROTO_ERROR_C(some_values_are_unused);
    }

    // TODO LazyValues
    template<class Params, class Values>
    struct Definition
    {
        static Params params() { return Params{}; }
        static Values values() { return Values{}; }
    };

    template<class... Xs>
    Definition(Xs const&...) -> Definition<Xs...>;

    template<class T>
    auto type(data<T> const&)
    {
        return type_only<T>{};
    }

    inline auto start()
    {
        return start_range{};
    }

    inline auto stop()
    {
        return stop_range{};
    }

    template<class... Xs>
    auto definition(Xs const&... /*xs*/)
    {
        using params = detail::mpc_remove_discard<
            mp::cfe<detail::param_list>,
            detail::value_to_definition_param_t<Xs>...>;

        using lazy_values = detail::mpc_remove_discard<
            mp::cfe<detail::lazy_value_list>,
            detail::value_to_definition_value_t<Xs>...>;

        detail::check_unused_params<params, lazy_values, errors::some_parameters_are_unused>();
        detail::check_unused_params<lazy_values, params, errors::some_values_are_unused>();

        return Definition<
            typename params::param_must_be_unique::list,
            typename lazy_values::list
        >{};
    }


    template<class Value, class Name>
    struct variable
    {
        using value_type = Value;
        using name = Name;
        Value value;
    };

    template<class Name, class X>
    auto& get_var(variable<X, Name>& v)
    {
        return v;
    }

    template<class Int, class Endianess>
    void write_buf2(OutStream& out, proto::datas::types::Integer<Int, Endianess>, Int x)
    {
        if constexpr (sizeof(Int) == 1) {
            out.out_uint8(x);
        }
        else {
            out.out_uint16_be(x);
        }
    }

    template<class Integer, class charset>
    void write_buf2(OutStream& out, proto::datas::types::StringSize<Integer, charset>, bytes_view view)
    {
        write_buf2(out, Integer{}, value_type_t<Integer>(view.size()));
    }

    template<class charset>
    void write_buf2(OutStream& out, proto::datas::types::StringData<charset>, bytes_view view)
    {
        out.out_copy_bytes(view);
    }

    template<class StringSize, class StringData, class ZeroPolicy>
    void write_buf2(
        OutStream& out,
        proto::datas::types::String<StringSize, StringData, ZeroPolicy>,
        bytes_view view)
    {
        write_buf2(out, StringSize{}, view);
        write_buf2(out, StringData{}, view);
    }

    template<class... Ts, class F>
    auto apply(mp::list<Ts...>, F&& f)
    {
        return f(Ts{}...);
    }

    class none {};
    PROTO_IS_SAME_TYPE(none);

    namespace errors
    {
        PROTO_ERROR_C(missing_parameters);
        PROTO_ERROR_C(unknown_parameters);
    }

    namespace detail
    {
        template<class Traits, class Param, class X>
        auto make_variable2(proto::value<X, name_t<Param>> const& v)
        {
            using builder = typename Traits::template value_variable_builder<
                data_type_t<Param>,
                std::remove_reference_t<X>>;
            // TODO v.value&&
            return variable<
                decltype(builder::make(v.value)),
                name_t<Param>
            >{builder::make(v.value)};
        }

        template<class Traits, class Params, class... Xs>
        auto build_params2(Xs const&... xs)
        {
            PROTO_ASSERT_TYPES(proto::is_value, Xs);

            using params = mp::call<mp::unpack<mp::cfe<detail::param_list>>, Params>;
            using value_params = detail::param_list<Xs...>;

            detail::check_unused_params<params, value_params, errors::missing_parameters>();
            detail::check_unused_params<value_params, params, errors::unknown_parameters>();

            // TODO value_type_t must be rvalue or lvalue
            proto::tuple<Xs...> t{Xs{static_cast<value_type_t<Xs>>(xs.value)}...};

            return apply(Params{}, [&](auto... p){
                return tuple{make_variable2<Traits, decltype(p)>(t)...};
            });
        }
    }

    namespace traits
    {
        template<class T> using static_size_t = typename T::static_size;
        template<class T> using context_value_list_t = typename T::context_value_list;

        template<class> class lazy {};
        template<class Data, class T> struct val { T x; };

        template<class T>
        std::ostream& operator<<(std::ostream& out, lazy<T> const&)
        {
            return out << "lazy<" << type_name<T>() << ">";
        }

        template<class Data, class T>
        std::ostream& operator<<(std::ostream& out, val<Data, T> const& v)
        {
            return out << v.x;
        }


        template<class> class data_to_value_data_impl;

        template<class T>
        using data_to_value_data
            = typename data_to_value_data_impl<proto_basic_type_t<T>>::type;

        template<class StringSize, class StringData, class ZeroPolicy>
        struct data_to_value_data_impl<datas::types::String<StringSize, StringData, ZeroPolicy>>
        {
            using type = StringData;
        };

        template<class> class data_to_value_size_impl;

        template<class T>
        using data_to_value_size
            = typename data_to_value_size_impl<proto_basic_type_t<T>>::type;

        template<class StringSize, class StringData, class ZeroPolicy>
        struct data_to_value_size_impl<datas::types::String<StringSize, StringData, ZeroPolicy>>
        {
            using type = StringSize;
        };


        template<class TParams, class TCtxValues>
        struct CtxW
        {
            OutStream out;
            TParams params;
            TCtxValues ctx_values;
        };

        struct stream_writable
        {
            template<class Data, class BasicType>
            struct value_variable_builder_impl;

            template<class Data, class T>
            using value_variable_builder
                = value_variable_builder_impl<Data, proto_basic_type_t<Data>>;

            template<class BasicType, class... NamedValues>
            struct next_value;

            template<class T>
            struct final_value;

            template<class T>
            struct write_data_impl;

            template<class T>
            using write_data = write_data_impl<proto_basic_type_t<T>>;

            template<class TParams, class>
            struct LazyValueToNextValue;

            template<class TParams, class Data, class... Name>
            struct LazyValueToNextValue<TParams, lazy_value<Data, Name...>>
            {
                using type = next_value<
                    proto_basic_type_t<Data>,
                    std::remove_reference_t<decltype(get_var<Name>(std::declval<TParams>()))>...
                >;
            };

            template<class Params_, class Values_, class... Xs>
            static auto make_context(writable_bytes_view buf, Xs const&... xs)
            {
                using Params = mpe::flatten<Params_>;
                using Values = mpe::flatten<Values_>;

                using tparams = decltype(detail::build_params2<stream_writable, Params>(xs...));
                using states_list = mp::call<
                    mp::unpack<
                        mp::transform<
                            mp::push_front<tparams&, mp::cfl<LazyValueToNextValue>>,
                            mp::fork<
                                mp::transform<mp::cfe<context_value_list_t>,
                                    mp::join<mp::cfe<tuple>>>,
                                mp::listify
                            >
                        >
                    >,
                    Values
                >;

                using ctx_values = mp::eager::at<states_list, 0>;

                return CtxW<tparams, ctx_values>{
                    OutStream{buf},
                    detail::build_params2<stream_writable, Params>(xs...),
                    ctx_values{} /** TODO uninit<ctx_values> ? **/
                };
            }

            template<class Ctx, class Data, class... Name>
            static auto next_state(Ctx& ctx, lazy_value<Data, Name...>)
            {
                using Next = next_value<
                    proto_basic_type_t<Data>,
                    std::remove_reference_t<decltype(get_var<Name>(ctx.params))>...
                >;
                return Next::make(ctx, get_var<Name>(ctx.params).value...);
            }

            template<class Ctx>
            static auto final(Ctx& ctx)
            {
                return ctx.out.get_bytes();
            }
        };


        template<class Data, class Int, class Endianess>
        struct stream_writable::value_variable_builder_impl<
            Data, datas::types::Integer<Int, Endianess>>
        {
            static auto make(safe_int<Int> x)
            {
                return val<Data, Int>{x.underlying()};
            }
        };

        template<class Data, class StringSize, class StringData>
        struct stream_writable::value_variable_builder_impl<
            Data, datas::types::String<StringSize, StringData, datas::types::no_zero>>
        {
            static auto make(bytes_view str)
            {
                return val<Data, bytes_view>{str};
            }
        };

        template<class Data, class Size>
        struct stream_writable::value_variable_builder_impl<
            Data, datas::types::PktSize<Size>>
        {
            static auto make(native)
            {
                return Data{};
            }
        };


        template<class Data, class T, class Name>
        struct stream_writable::next_value<as_param, variable<val<Data, T>, Name>>
        {
            using context_value_list = mp::list<>;

            template<class Ctx>
            static bool make(Ctx& ctx, val<Data, T> const& v)
            {
                write_data<Data>::write(ctx.out, v.x);
                return true;
            }
        };

        template<class Name, class Data, class Bytes>
        struct stream_writable::next_value<
            datas::values::types::SizeBytes<as_param>,
            variable<val<Data, Bytes>, Name>>
        {
            using context_value_list = mp::list<>;

            template<class Ctx>
            static bool make(Ctx& ctx, val<Data, Bytes> const& v)
            {
                write_data<data_to_value_size<Data>>::write(ctx.out, v.x.size());
                return true;
            }
        };

        template<class Name, class Data, class Bytes>
        struct stream_writable::next_value<
            datas::values::types::Data<as_param>,
            variable<val<Data, Bytes>, Name>>
        {
            using context_value_list = mp::list<>;

            template<class Ctx>
            static bool make(Ctx& ctx, val<Data, Bytes> const& v)
            {
                write_data<data_to_value_data<Data>>::write(ctx.out, v.x);
                return true;
            }
        };

        template<class DataSize, class Name>
        using pkt_size_variable = variable<val<DataSize, std::array<uint8_t*, 2>>, Name>;

        template<class DataSize, class Name>
        struct stream_writable::next_value<as_param, variable<datas::types::PktSize<DataSize>, Name>>
        {
            using var_size = pkt_size_variable<DataSize, Name>;
            using context_value_list = mp::list<var_size>;

            template<class Ctx>
            static bool make(Ctx& ctx, datas::types::PktSize<DataSize>)
            {
                static_cast<var_size&>(ctx.ctx_values).value.x[0] = ctx.out.get_current();
                ctx.out.out_skip_bytes(sizeof(value_type_t<DataSize>));
                return true;
            }
        };

        template<class DataSize, class Name>
        struct stream_writable::next_value<start_range, variable<datas::types::PktSize<DataSize>, Name>>
        {
            using context_value_list = mp::list<>;

            template<class Ctx>
            static bool make(Ctx& ctx, datas::types::PktSize<DataSize>)
            {
                static_cast<pkt_size_variable<DataSize, Name>&>(ctx.ctx_values).value.x[1]
                    = ctx.out.get_current();
                return true;
            }
        };

        template<class DataSize, class Name>
        struct stream_writable::next_value<stop_range, variable<datas::types::PktSize<DataSize>, Name>>
        {
            // TODO automatically write size if packet is static
            // TODO compute static distance with a static packet
            using context_value_list = mp::list<>;

            template<class Ctx>
            static bool make(Ctx& ctx, datas::types::PktSize<DataSize>)
            {
                auto& ptrs = static_cast<pkt_size_variable<DataSize, Name>&>(ctx.ctx_values).value.x;
                assert(ptrs[1] <= ctx.out.get_current());
                OutStream out({ptrs[0], sizeof(value_type_t<DataSize>)});
                write_data<DataSize>::write(out, ctx.out.get_current() - ptrs[1]);
                return true;
            }
        };

        template<class Int, class Endianess>
        struct stream_writable::write_data_impl<datas::types::Integer<Int, Endianess>>
        {
            static void write(OutStream& out, Int x)
            {
                auto* p = out.out_skip_bytes(sizeof(Int)).data();
                using rng = mpe::make_int_sequence<mp::int_<sizeof(Int)>>;
                using is_little_endian = std::is_same<Endianess, datas::types::le_tag>;
                apply(rng{}, [&](auto... i) {
                    ((
                        *p++ = x >> ((is_little_endian::value ? i.value : sizeof(Int)-1u-i.value) * 8u)
                    ), ...);
                });
            }
        };

        template<>
        struct stream_writable::write_data_impl<datas::types::StringData<proto::datas::types::ascii_charset>>
        {
            static void write(OutStream& out, bytes_view x)
            {
                out.out_copy_bytes(x);
            }
        };


        template<class TParams, class TCtxValues, class ErrorFn>
        struct Ctx
        {
            InStream in;
            std::size_t unchecked_size;
            TParams params;
            TCtxValues ctx_values;
            ErrorFn err;
        };


        template<class Ctx, class R>
        struct State
        {
            Ctx& ctx;
            R result;
        };

        template<class Ctx, class R>
        State(Ctx&, R&&) -> State<Ctx, R>;

        struct stream_readable2
        {
            template<class Data, class BasicType>
            struct value_variable_builder_impl;

            template<class Data, class T>
            using value_variable_builder
                = value_variable_builder_impl<Data, proto_basic_type_t<Data>>;

            template<class BasicType, class... NamedValues>
            struct next_value;

            struct reader_impl;

            template<class T>
            using reader = reader_impl;

            template<class T>
            struct read_data_impl;

            template<class T>
            using read_data = read_data_impl<proto_basic_type_t<T>>;

            template<class TParams, class>
            struct LazyValueToNextValue;

            template<class TParams, class Data, class... Name>
            struct LazyValueToNextValue<TParams, lazy_value<Data, Name...>>
            {
                using type = next_value<
                    proto_basic_type_t<Data>,
                    std::remove_reference_t<decltype(get_var<Name>(std::declval<TParams>()))>...>;
            };

            template<class Params, class Values, class ErrorFn, class... Xs>
            static auto make_context(bytes_view buf, ErrorFn& error, Xs const&... xs)
            {
                using tparams = decltype(detail::build_params2<stream_readable2, Params>(xs...));
                using states_list = mp::call<
                    mp::unpack<
                        mp::transform<
                            mp::push_front<tparams&, mp::cfl<LazyValueToNextValue>>,
                            mp::fork<
                                mp::transform<mp::cfe<context_value_list_t>,
                                    mp::join<mp::cfe<tuple>>>,
                                mp::transform<mp::cfe<static_size_t>, va_plus<>>,
                                mp::listify
                            >
                        >
                    >,
                    Values
                >;

                using ctx_values = mp::eager::at<states_list, 0>;
                using size = mp::eager::at<states_list, 1>;

                if (buf.size() < size::value) {
                    error();
                }

                return Ctx<tparams, ctx_values, ErrorFn&>{
                    InStream{buf}, buf.size() - size::value,
                    detail::build_params2<stream_readable2, Params>(xs...),
                    ctx_values{} /** TODO uninit<ctx_values> ? **/, error
                };
            }

            template<class Ctx>
            static auto make_state(Ctx& ctx)
            {
                return State{ctx, tuple<>{}};
            }

            template<class State, class Data, class... Name>
            static auto next_state(State& state, lazy_value<Data, Name...>)
            {
                using Next = next_value<
                    proto_basic_type_t<Data>,
                    std::remove_reference_t<decltype(get_var<Name>(state.ctx.params))>...>;
                return Next::make(state, get_var<Name>(state.ctx.params).value...);
            }

            template<class State>
            static auto final(State&& state)
            {
                // TODO sort by alignement ?
                return state.result;
            }
        };


        template<class Data, class Int, class Endianess>
        struct stream_readable2::value_variable_builder_impl<
            Data, datas::types::Integer<Int, Endianess>>
        {
            static auto make(native)
            {
                return lazy<Data>{};
            }

            static auto make(ref<Int> r)
            {
                return val<Data, Int&>{r.x};
            }
        };

        template<class Data, class StringSize, class StringData, class ZeroPolicy>
        struct stream_readable2::value_variable_builder_impl<
            Data, datas::types::String<StringSize, StringData, ZeroPolicy>>
        {
            static auto make(native)
            {
                return val<Data, lazy<bytes_view>>{};
            }

            static auto make(writable_bytes_view str)
            {
                return val<Data, writable_bytes_view>{str};
            }
        };


        template<class Name, class T>
        auto make_mem(T&& x)
        {
            return typename decltype(Name::mem()(wrap_type<T>()))::type{static_cast<T&&>(x)};
        }

        template<class... Ts, class X>
        auto tuple_add(tuple<Ts...>& t, X&& x)
        {
            return tuple<Ts..., X>{static_cast<Ts&>(t)..., {static_cast<X&&>(x)}};
        }

        template<class Name, class Data>
        struct stream_readable2::next_value<as_param,
            variable<lazy<Data>, Name>>
        {
            using static_size = static_size_t<read_data<Data>>;
            using context_value_list = mp::list<>;

            template<class St>
            static auto make(St& state, lazy<Data>)
            {
                return State{
                    state.ctx,
                    tuple_add(state.result, make_mem<Name>(read_data<Data>::read(state.ctx.in)))
                };
            }
        };

        template<class Name, class Data, class T>
        struct stream_readable2::next_value<as_param,
            variable<val<Data, T&>, Name>>
        {
            using static_size = static_size_t<read_data<Data>>;
            using context_value_list = mp::list<>;

            template<class St>
            static auto make(St& state, val<Data, T&> v)
            {
                v.x = read_data<Data>::read(state.ctx.in);
                return State{
                    state.ctx,
                    tuple_add(state.result, make_mem<Name>(v.x))
                };
            }
        };

        template<class Name, class Data, class Bytes>
        struct stream_readable2::next_value<
            datas::values::types::SizeBytes<as_param>,
            variable<val<Data, Bytes>, Name>>
        {
            using DataSize = data_to_value_size<Data>;
            using var_size = variable<
                value_type_t<DataSize>,
                datas::values::types::SizeBytes<Name>>;

            using static_size = static_size_t<read_data<DataSize>>;
            using context_value_list = mp::list<var_size>;

            template<class St>
            static auto& make(St& state, val<Data, Bytes>&)
            {
                static_cast<var_size&>(state.ctx.ctx_values).value
                    = read_data<DataSize>::read(state.ctx.in);
                return state;
            }
        };

        template<class Name, class Data>
        struct stream_readable2::next_value<
            datas::values::types::Data<as_param>,
            variable<val<Data, writable_bytes_view>, Name>>
        {
            using static_size = mp::int_<0>;
            using context_value_list = mp::list<>;

            template<class St>
            static auto make(St& state, val<Data, writable_bytes_view>& v)
            {
                auto n = get_var<datas::values::types::SizeBytes<Name>>(state.ctx.ctx_values).value;

                if (state.ctx.unchecked_size < n) {
                    state.ctx.err();
                }
                state.ctx.unchecked_size -= n;

                v.x = {v.x.data(), std::size_t(n)};
                state.ctx.in.in_copy_bytes(v.x);
                return State{
                    state.ctx,
                    tuple_add(state.result, make_mem<Name>(writable_bytes_view(v.x)))
                };
            }
        };

        template<class Name, class Data>
        struct stream_readable2::next_value<
            datas::values::types::Data<as_param>,
            variable<val<Data, lazy<bytes_view>>, Name>>
        {
            using static_size = mp::int_<0>;
            using context_value_list = mp::list<>;

            template<class St>
            static auto make(St& state, val<Data, lazy<bytes_view>>&)
            {
                auto n = get_var<datas::values::types::SizeBytes<Name>>(state.ctx.ctx_values).value;

                if (state.ctx.unchecked_size < n) {
                    state.ctx.err();
                }
                state.ctx.unchecked_size -= n;

                return State{
                    state.ctx,
                    tuple_add(state.result, make_mem<Name>(state.ctx.in.in_skip_bytes(n)))
                };
            }
        };

        template<class Int, class Endianess>
        struct stream_readable2::read_data_impl<datas::types::Integer<Int, Endianess>>
        {
            using static_size = mp::int_<sizeof(Int)>;

            static Int read(InStream& in)
            {
                uint8_t const* p = in.in_skip_bytes(sizeof(Int)).data();
                using rng = mpe::make_int_sequence<mp::int_<sizeof(Int)>>;
                using is_little_endian = std::is_same<Endianess, datas::types::le_tag>;
                return apply(rng{}, [&](auto... i) {
                    return ((
                        p[i.value] << ((is_little_endian::value ? i.value : sizeof(Int)-1u-i.value) * 8u)
                     ) | ...);
                });
            }
        };
    }

    template<class Tag, class BasicType, class T>
    struct variable_builder;

    class readable_variable;
    class writable_variable;
    class native_variable;

    template<class BasicType, class T>
    struct variable_builder<readable_variable, BasicType, T>
    {
        static auto make(T const& x)
        {
            return basic_type_to_cpp_type<BasicType>{x};
        }
    };

    template<class BasicType, class T>
    struct variable_builder<writable_variable, BasicType, T&>
    {
        static T& make(T& x)
        {
            static_assert(std::is_same_v<basic_type_to_cpp_type<BasicType>, T>);
            return x;
        }
    };

    template<class T>
    struct is_writable_view : std::false_type
    {};

    template<> struct is_writable_view<writable_bytes_view> : std::true_type {};
    template<class T> struct is_writable_view<array_view<T>> : std::true_type {};
    template<class T> struct is_writable_view<array_view<const T>> : std::false_type {};

    namespace errors
    {
        PROTO_ERROR_C(is_not_writable_view);
    }

    template<class BasicType, class T>
    struct variable_builder<writable_variable, BasicType, T>
    {
        using View = std::remove_reference_t<T>;

        static auto make(View v)
        {
            detail::check_pred<View, mp::cfe<is_writable_view>, errors::is_not_writable_view>();
            return v;
        }
    };

    namespace detail
    {
        template<class Traits, class State>
        auto recursive_inplace_recv2(State&& state)
        {
            return Traits::final(static_cast<State&&>(state));
        }

        template<class Traits, class State, class V, class... Vs>
        auto recursive_inplace_recv2(State&& state, V&& v, Vs&&... vs)
        {
            // println("  ", type_name<decltype(Traits::next_state(state, v))>());
            return recursive_inplace_recv2<Traits>(Traits::next_state(state, v), vs...);
        }
    }

    template<class Params, class Values, class... Xs>
    writable_bytes_view inplace_emit(writable_bytes_view buf, Definition<Params, Values> const&, Xs const&... xs)
    {
        using Traits = traits::stream_writable;

        auto ctx = Traits::make_context<Params, Values>(buf, xs...);

        return apply(Values{}, [&](auto... v){
            (Traits::next_state(ctx, v), ...);
            return Traits::final(ctx);
        });
    }

    template<class ErrorFn, class Params, class Values, class... Xs>
    auto inplace_recv2(bytes_view buf, ErrorFn&& error, Definition<Params, Values> const&, Xs const&... xs)
    {
        using Traits = traits::stream_readable2;

        auto ctx = Traits::make_context<Params, Values>(buf, error, xs...);

        // ctx.params.apply([&](auto... vars){
        //     (println("  ", type_name(vars), " = ", vars.value), ...);
        // });

        return apply(Values{}, [&](auto... v){
            return detail::recursive_inplace_recv2<Traits>(Traits::make_state(ctx), v...);
        });
    }

    template<class ErrorFn, class... Params, class Values, class... Xs>
    auto inplace_struct(bytes_view buf, ErrorFn&& error, Definition<mp::list<Params...>, Values> const& def)
    {
        return inplace_recv2(buf, error, def, value<native, name_t<Params>>{}...);
    }
}

namespace detail
{
    using mp::list;
    PROTO_IS_TYPE_VA(list);
}

using detail::is_list;
using detail::is_list_v;

int main()
{
    struct S {
        PROTO_LOCAL_NAME(a);
        PROTO_LOCAL_NAME(b);
        PROTO_LOCAL_NAME(c);
        PROTO_LOCAL_NAME(size);
        PROTO_LOCAL_NAME(size2);
    } s;

    struct SS {
        PROTO_LOCAL_NAME(a);
        PROTO_LOCAL_NAME(b);
    } ss;

    using namespace proto::datas;

    auto def = test::definition(
        ss.a = pkt_size(u16_be),
        ss.b = u8,
        ss.a = test::start(),

        s.c = test::type(ascii_string(u16_be)),

        s.size = test::start(),
        s.size = pkt_size(u16_be),

        s.a = u8,
        s.c = values::size_bytes,
        s.b = u16_le,
        s.c = values::data,

        s.size = test::stop(),
        ss.a = test::stop()
    );

    auto print_list = [](char const* s, auto l){
        print(s, ":\n");
        test::apply(l, [&](auto... xs){
            (println("  ", type_name(xs)), ...);
        });
    };

    uint8_t a = 0x78;
    uint16_t b = 0x1f23;

    // println('\n', type_name(test::definition2(u8[s.a])));
    println("definition:\n  ", type_name(def));
    print_list("params", def.params());
    print_list("values", def.values());

    print("\n\ninplace_emit:\n\n");
    std::array<uint8_t, 30> buf {};
    auto out = test::inplace_emit(buf, def, s.b = b, s.a = a, s.c = "plop"_av, s.size = native(), ss.a = native(), ss.b = a);
    dump(out);

    // auto error_fn = [](){
    //     throw std::runtime_error("buf is too short");
    // };
    //
    // {
    //     print("\n\ninplace_recv2:\n");
    //     char strbuf[10];
    //     a = '0';
    //     auto datas = test::inplace_recv2(out, error_fn, def, s.b = native(), s.a = ref{a}, s.c = make_array_view(strbuf), s.size = native());
    //     println("\n", type_name(datas));
    //     println("datas.a = ", std::hex, datas.a, " ", type_name<decltype(datas.a)>());
    //     println("datas.b = ", std::hex, datas.b, " ", type_name<decltype(datas.b)>());
    //     println("datas.c = ", datas.c, " ", type_name<decltype(datas.c)>());
    //     println("a = ", a);
    // }
    //
    // {
    //     print("\n\ninplace_struct:\n");
    //     auto datas = test::inplace_struct(out, error_fn, def);
    //     println("\n", type_name(datas));
    //     println("datas.a = ", std::hex, datas.a, " ", type_name<decltype(datas.a)>());
    //     println("datas.b = ", std::hex, datas.b, " ", type_name<decltype(datas.b)>());
    //     println("datas.c = ", datas.c, " ", type_name<decltype(datas.c)>());
    //
    //     datas.apply([](auto const&... xs) {
    //         (println(xs.proto_name(), ": ", xs.proto_value()), ...);
    //     });
    // }
}
