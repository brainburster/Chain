#ifndef _CHAINCALL_HPP
#define _CHAINCALL_HPP

#include <tuple>

namespace chaincall
{
    namespace impl
    {
        template <size_t N>
        struct Apply
        {
            template <typename F, typename T, typename... A>
            static inline auto apply(F &&f, T &&t, A &&... a)
                -> decltype(Apply<N - 1>::apply(
                    ::std::forward<F>(f), ::std::forward<T>(t),
                    ::std::get<N - 1>(::std::forward<T>(t)),
                    ::std::forward<A>(a)...))
            {
                return Apply<N - 1>::apply(::std::forward<F>(f),
                                           ::std::forward<T>(t),
                                           ::std::get<N - 1>(::std::forward<T>(t)),
                                           ::std::forward<A>(a)...);
            }
        };

        template <>
        struct Apply<0>
        {
            template <typename F, typename T, typename... A>
            static inline auto apply(F &&f, T &&, A &&... a)
                -> decltype(::std::forward<F>(f)(::std::forward<A>(a)...))
            {
                return ::std::forward<F>(f)(::std::forward<A>(a)...);
            }
        };

        template <typename F, typename T>
        inline auto apply(F &&f, T &&t)
            -> decltype(Apply<::std::tuple_size<
                            typename ::std::decay<T>::type>::value>::apply(::std::forward<F>(f),
                                                                           ::std::forward<T>(t)))
        {
            return Apply<::std::tuple_size<
                typename ::std::decay<T>::type>::value>::apply(::std::forward<F>(f),
                                                               ::std::forward<T>(t));
        }

        template <typename ValueType>
        struct Chain
        {
            ValueType value;
        };

        template <>
        struct Chain<void>
        {
        };

        template <typename RHS, typename ParamType>
        inline auto operator>>(Chain<ParamType> &&lhs, RHS &&rhs) -> Chain<decltype(rhs(lhs.value))>
        {
            return {std::forward<RHS>(rhs)(std::move(lhs.value))};
        };

        template <typename RHS, typename ParamType>
        inline auto operator>>(Chain<ParamType> &&lhs, RHS &&rhs) -> Chain<decltype(rhs())>
        {
            return {std::forward<RHS>(rhs)()};
        };

        template <typename RHS, typename ParamType, size_t... n>
        inline auto operator>>(Chain<ParamType> &&lhs, RHS &&rhs) -> Chain<decltype(apply(rhs, lhs.value))>
        {
            return {apply(std::forward<RHS>(rhs), std::move(lhs.value))};
        };
    } // namespace impl
    
    template <typename T>
    impl::Chain<T> chain(T &&value)
    {
        //return {static_cast<T &&>(value)};
        return *reinterpret_cast<impl::Chain<T>*>(&value);
    };

    template <typename... T>
    impl::Chain<std::tuple<T...>> chain(T &&... value)
    {
        return {std::forward_as_tuple(std::forward<T>(value)...)};
    };

    impl::Chain<void> chain()
    {
        return {};
    }

} // namespace chaincall

#endif // _CHAINCALL_HPP