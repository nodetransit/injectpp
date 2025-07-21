#ifndef INJECTPP_COMMON_HPP_HEADER__
#define INJECTPP_COMMON_HPP_HEADER__

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <atomic>
#include <functional>


namespace nt { namespace ioc {
#if __cplusplus < 201402L

template<typename T, typename... Args>
std::unique_ptr<T>
make_unique(Args&& ... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
using remove_const_t = typename std::remove_const<T>::type;

#else

//template<typename T, typename... Args>
//constexpr T*(make_unique)(...) = &std::make_unique<T, Args>;
template<typename T, typename... Args>
std::unique_ptr<T>
make_unique(Args&& ... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using remove_const_t = typename std::remove_const_t<T>;

#endif

template<class InstanceType, class Deleter, class... Deps>
using InstanceFactoryFunction = std::unique_ptr<InstanceType, Deleter>(*)(Deps* ...);

}}

#endif /* INJECTPP_COMMON_HPP_HEADER__ */
