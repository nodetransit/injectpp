#ifndef INJECTPP_INSTANCE_CONTAINER_HPP_HEADER__
#define INJECTPP_INSTANCE_CONTAINER_HPP_HEADER__

#include "typeid.hpp"
#include "typemap.hpp"
#include "abstractinstancecontainer.hpp"


namespace nt { namespace ioc {
template<class T, class Deleter>
class InstanceContainer :
      public AbstractInstanceContainer
{
public:
    explicit InstanceContainer(std::unique_ptr<T, Deleter>&& p) :
          pointer_(std::move(p))
    {
    }

    void* get()
    {
        return static_cast<void*>(pointer_.get());
    }

private:
    std::unique_ptr<T, Deleter> pointer_;
};

template<class T, class Deleter>
std::unique_ptr<AbstractInstanceContainer>
wrap_into_instance_container(
      std::unique_ptr<T, Deleter>&& ptr)
{
    //return std::unique_ptr<T>(new T(std::forward<Deleter>(std::move(ptr))));
    return nt::ioc::make_unique<InstanceContainer<T, Deleter>>(std::move(ptr));
}

}}

#endif /* INJECTPP_INSTANCE_CONTAINER_HPP_HEADER__ */
