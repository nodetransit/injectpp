#ifndef INJECTPP_INJECTOR_HPP_HEADER__
#define INJECTPP_INJECTOR_HPP_HEADER__

#include "typeid.hpp"
#include "typemap.hpp"
#include "abstractinstancecontainer.hpp"
#include "instancecontainer.hpp"
#include "injectppcommon.hpp"


namespace injectpp {
class Injector
{
    friend class Config;

public:
    Injector(Injector&& other)
    {
        *this = std::move(other);
    }

    Injector& operator=(Injector&& other)
    {
        instance_map_ = std::move(other.instance_map_);

        return *this;
    }

    template<class T, class Y>
    Y* get_instance() const;

    template<class T>
    T* get_instance() const;

    template<class T, class Dependee = std::nullptr_t>
    T* get_object() const;

    template<class InstanceType, class Deleter, class ...Deps>
    std::unique_ptr<InstanceType, Deleter> inject(InstanceFactoryFunction<InstanceType, Deleter, Deps...> instance_factory) const;


private:
    Injector() = default;

    using InstanceMap = injectpp::TypeMap<std::unique_ptr<injectpp::AbstractInstanceContainer>>;

    InstanceMap instance_map_;
};

template<class T, class Y>
Y*
Injector::get_instance() const
{
    return dynamic_cast<Y*>(get_object<T>());
}

template<class T>
T*
Injector::get_instance() const
{
    return get_object<T>();
}

template<class T, class Dependee>
T*
Injector::get_object() const
{
    auto it = instance_map_.find<T>();

    if (it == instance_map_.end()) {
        throw std::runtime_error(
              std::string(typeid(T).name()) +
              ": unsatisfied dependency of "
              + std::string(typeid(Dependee).name()));
    }

    return static_cast<T*>(it->second->get());
}

template<class InstanceType, class Deleter, class ...Deps>
std::unique_ptr<InstanceType, Deleter>
Injector::inject(
      InstanceFactoryFunction<InstanceType, Deleter, Deps...> instance_factory) const
{
    return instance_factory(
          get_object<typename injectpp::remove_const_t<Deps>,
                     typename injectpp::remove_const_t<InstanceType>>()...);
}

}

#endif /* INJECTPP_INJECTOR_HPP_HEADER__ */
