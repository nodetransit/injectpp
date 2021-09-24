#include "injectpp.hpp"

namespace nt { namespace ioc {

Injector::Injector(Injector &&other) noexcept
{
    *this = std::move(other);
}

Injector &
Injector::operator=(Injector &&other) noexcept
{
    instance_map_ = std::move(other.instance_map_);

    return *this;
}

}}
