#ifndef INJECTPP_DICONFIG_HPP_HEADER__
#define INJECTPP_DICONFIG_HPP_HEADER__

#include "injectppcommon.hpp"
#include "injector.hpp"


namespace nt { namespace ioc {
class Config
{
public:
    template<class InstanceType, class Deleter, class ...Deps>
    void add(InstanceFactoryFunction<InstanceType, Deleter, Deps...> instance_factory)
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

    // Create Instances
    Injector build_injector()
    {
        Injector injector;

        for (auto& node : graph) {
            // This test is logically redundant, it's just for better performance.
            if (node.second.mark_ == DependencyNode::Mark::Unmarked) {
                toposort_visit_node(node.first, injector);
            }
        }

        return injector;
    }

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

    void toposort_visit_node(int node_id, Injector& injector)
    {
        DependencyNode& node = graph[node_id];

        if (node.mark_ == DependencyNode::Mark::Temp) {
            throw std::runtime_error(node.debug_type_name + " appears to be part of a cycle");
        } else if (node.mark_ == DependencyNode::Mark::Unmarked) {
            node.mark_ = DependencyNode::Mark::Temp;

            for (int dependent : node.dependencies) {
                toposort_visit_node(dependent, injector);
            }

            node.mark_ = DependencyNode::Mark::Marked;

            if (node.has_iniliatizer) {
                // if has_initializer_ is false, it means someone depends on this
                // node, but an instance factory for this node has not been provided.
                // This will result in an injection error later.
                node.initializer(injector);
            }
        }
    }

    std::map<int, DependencyNode> graph;
};

}}

#endif /* INJECTPP_DICONFIG_HPP_HEADER__ */
