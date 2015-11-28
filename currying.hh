#pragma once
#include "function_traits.hh"
 
template <typename FuncT>
auto curry(FuncT);

template <
    typename... LeftArgs, 
    typename FuncT, 
    typename FirstArg, 
    typename = typename std::enable_if<std::is_reference<FirstArg>::value>::type, 
    typename = void
>
auto foward_first(FuncT func, FirstArg&& first)
{
    return curry([func, &first](LeftArgs... args) mutable 
        -> decltype(func(std::forward<FirstArg>(first), std::forward<LeftArgs>(args)...))
    {
        return func(std::forward<FirstArg>(first), std::forward<LeftArgs>(args)...);
    });
}

template <
    typename... LeftArgs, 
    typename FuncT, 
    typename FirstArg,
    typename = typename std::enable_if<!std::is_reference<FirstArg>::value>::type 
>
auto foward_first(FuncT func, FirstArg&& first)
{
    return curry([func, first](LeftArgs... args) mutable -> decltype(func(first, std::forward<LeftArgs>(args)...))
    {
        return func(first, std::forward<LeftArgs>(args)...);
    });
}

template <typename FuncT>
auto curryImpl(FuncT func, std::index_sequence<>)
{
    return func;
}

template <typename FuncT, size_t... Is, typename = typename std::enable_if<(sizeof...(Is) > 0)>::type>
auto curryImpl(FuncT func, std::index_sequence<Is...>)
{
    return [func](typename function_traits<FuncT>::template args<0>::type first) mutable
    {
        return foward_first<typename function_traits<FuncT>::template args<Is + 1>::type...>
        (func, std::forward<typename function_traits<FuncT>::template args<0>::type>(first));
    };
}

template <typename FuncT>
auto curry(FuncT func)
{
    return curryImpl(func, std::make_index_sequence<function_traits<FuncT>::arity - 1>());
}
