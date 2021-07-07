#ifndef INJECTPP_TYPEID_HPP_HEADER__
#define INJECTPP_TYPEID_HPP_HEADER__

#include "injectppcommon.hpp"

namespace injectpp {
std::atomic<int> g_type_id_counter(0);

template<class Key>
static int
type_id()
{
    static int id = ++g_type_id_counter;
    return id;
}

}

#endif /* INJECTPP_TYPEID_HPP_HEADER__ */
