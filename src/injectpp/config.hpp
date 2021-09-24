#ifndef INJECTPP_DICONFIG_HPP_HEADER__
#define INJECTPP_DICONFIG_HPP_HEADER__

#include "injectppcommon.hpp"
#include "injector.hpp"


namespace nt { namespace ioc {
class Config
{
public:
    template<class InstanceType, class Deleter, class ...Deps>
    void add(InstanceFactoryFunction<InstanceType, Deleter, Deps...> instance_factory);

    Injector __InjectppPort__ build_injector();

private:
    using InitializerFn = std::function<void(Injector & )>;

    struct DependencyNode
    {
        enum class Mark
        {
            Unmarked,
            Temp,
            Marked
        };

        Mark mark_ = Mark::Unmarked;

        /**
         * @brief Name of the service class needed to display more useful error messages
         */
        std::string debug_type_name;

        /**
         * @brief A function that invokes the instance factory and adds the created service to the given injector.
         */
        InitializerFn initializer;

        bool has_iniliatizer = false;

        std::vector<int> dependencies;
    };

    void __InjectppPort__ toposort_visit_node(int node_id, Injector& injector);

    std::unordered_map<int, DependencyNode> graph;
};

template<class InstanceType, class Deleter, class ...Deps>
void
Config::add(InstanceFactoryFunction<InstanceType, Deleter, Deps...> instance_factory)
{
    int instance_type_id = nt::ioc::type_id<typename nt::ioc::remove_const_t<InstanceType>>();
    DependencyNode& node = graph[instance_type_id];

    node.initializer = [instance_factory](Injector& inj) {
        auto instance = nt::ioc::wrap_into_instance_container(inj.inject(instance_factory));

        inj.instance_map_.put<InstanceType>(std::move(instance));
    };

    node.has_iniliatizer = true;
    node.debug_type_name = typeid(typename nt::ioc::remove_const_t<InstanceType>).name();

    node.dependencies = {nt::ioc::type_id<typename nt::ioc::remove_const_t<Deps>>()...};
}

}}

#endif /* INJECTPP_DICONFIG_HPP_HEADER__ */
