#ifndef _CHAINCALL_HPP
#define _CHAINCALL_HPP

#include <tuple>
#include <type_traits>

namespace chaincall
{
	namespace impl
	{
		namespace cpp11
		{
			template <size_t...>
			struct index_sequence
			{
			};

			template <size_t N, size_t... M>
			struct make_index_sequence : make_index_sequence<N - 1, N - 1, M...>
			{
			};

			template <size_t... M>
			struct make_index_sequence<0, M...> : index_sequence<M...>
			{
			};
		} // namespace cpp11

		template <typename F, typename T, size_t... I>
		inline auto _apply_impl(F&& f, T&& t, cpp11::index_sequence<I...>) -> decltype(f(std::get<I>(std::forward<T>(t))...))
		{
			return f(std::get<I>(std::forward<T>(t))...);
		}

		template <typename F, typename... Args, typename Indices = cpp11::make_index_sequence<sizeof...(Args)>>
		inline auto _apply(F&& f, std::tuple<Args...>&& t) -> decltype(_apply_impl(std::forward<F>(f), std::forward<std::tuple<Args...>>(t), Indices()))
		{
			return _apply_impl(std::forward<F>(f), std::forward<std::tuple<Args...>>(t), Indices());
		}

		template <typename ValueType>
		struct Chain
		{
			ValueType value;
		};

		template <>
		struct Chain<void>
		{
			static Chain<void>&& getInstance()
			{
				static Chain<void> instance{};
				return std::move(instance);
			}
		};

		template <typename RHS, typename ParamType>
		inline auto operator>>(Chain<ParamType>&& lhs, RHS&& rhs) -> typename std::enable_if<!std::is_void<decltype(rhs(std::move(lhs.value)))>::value, Chain<decltype(rhs(std::move(lhs.value)))>>::type
		{
			auto temp = std::forward<RHS>(rhs)(reinterpret_cast<typename std::decay<decltype(lhs.value)>::type&&>(*reinterpret_cast<typename std::decay<decltype(lhs.value)>::type*>(&lhs)));
			return std::move(*reinterpret_cast<Chain<decltype(temp)> *>(&temp));
		};

		template <typename RHS, typename ParamType>
		inline auto operator>>(Chain<ParamType>&& lhs, RHS&& rhs) -> typename std::enable_if<std::is_void<decltype(rhs(std::move(lhs.value)))>::value, Chain<void>>::type
		{
			std::forward<RHS>(rhs)(reinterpret_cast<typename std::decay<decltype(lhs.value)>::type&&>(*reinterpret_cast<typename std::decay<decltype(lhs.value)>::type*>(&lhs)));
			return Chain<void>::getInstance();
		};

		template <typename RHS, typename ParamType>
		inline auto operator>>(Chain<ParamType>&& lhs, RHS&& rhs) -> typename std::enable_if<!std::is_void<decltype(rhs())>::value, Chain<decltype(rhs())>>::type
		{
			auto temp = std::forward<RHS>(rhs)();
			return std::move(*reinterpret_cast<Chain<decltype(temp)> *>(&temp));
		};
		template <typename RHS, typename ParamType>
		inline auto operator>>(Chain<ParamType>&& lhs, RHS&& rhs) -> typename std::enable_if<std::is_void<decltype(rhs())>::value, Chain<void>>::type
		{
			std::forward<RHS>(rhs)();
			return Chain<void>::getInstance();
		};

		template <typename RHS, typename ParamType>
		inline auto operator>>(Chain<ParamType>&& lhs, RHS&& rhs) -> typename std::enable_if<!std::is_void<decltype(_apply(rhs, std::move(lhs.value)))>::value, Chain<decltype(_apply(rhs, std::move(lhs.value)))>>::type
		{
			auto temp = _apply(std::forward<RHS>(rhs), reinterpret_cast<typename std::decay<decltype(lhs.value)>::type&&>(*reinterpret_cast<typename std::decay<decltype(lhs.value)>::type*>(&lhs)));
			return std::move(*reinterpret_cast<Chain<decltype(temp)> *>(&temp));
		};
		template <typename RHS, typename ParamType>
		inline auto operator>>(Chain<ParamType>&& lhs, RHS&& rhs) -> typename std::enable_if<std::is_void<decltype(_apply(rhs, std::move(lhs.value)))>::value, Chain<void>>::type
		{
			_apply(std::forward<RHS>(rhs), reinterpret_cast<typename std::decay<decltype(lhs.value)>::type&&>(*reinterpret_cast<typename std::decay<decltype(lhs.value)>::type*>(&lhs)));
			return Chain<void>::getInstance();
		};
	} // namespace impl

	template <typename T>
	impl::Chain<T> chain(T&& value)
	{
		//return {static_cast<T &&>(value)};
		return *reinterpret_cast<impl::Chain<T> *>(&value);
	};

	template <typename... T>
	impl::Chain<std::tuple<T...>> chain(T &&... value)
	{
		return { std::forward_as_tuple(std::forward<T>(value)...) };
	};

	impl::Chain<void> chain()
	{
		return  impl::Chain<void>::getInstance();
	}
} // namespace chaincall

#endif // _CHAINCALL_HPP
