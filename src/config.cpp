#include "injectpp.hpp"


namespace nt { namespace ioc {

void
Config::toposort_visit_node(int node_id,
                            Injector& injector)
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

// Create Instances
Injector
Config::build_injector()
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

}}

