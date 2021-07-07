#ifndef INJECTPP_ABSTRACT_INSTANCE_CONTAINER_HPP_HEADER__
#define INJECTPP_ABSTRACT_INSTANCE_CONTAINER_HPP_HEADER__

#include "injectppcommon.hpp"


namespace injectpp {
class AbstractInstanceContainer
{
public:
    virtual ~AbstractInstanceContainer() = default;

    virtual void* get() = 0;
};

}

#endif /* INJECTPP_ABSTRACT_INSTANCE_CONTAINER_HPP_HEADER__ */


