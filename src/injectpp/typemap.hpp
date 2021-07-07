#ifndef INJECTPP_TYPEMAP_HPP_HEADER__
#define INJECTPP_TYPEMAP_HPP_HEADER__


namespace nt { namespace ioc {
// Maps types to objects of ValueType
template<class ValueType>
class TypeMap
{
    using Container = std::unordered_map<int, ValueType>;

public:
    using iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;

    iterator begin()
    {
        return container_.begin();
    }

    iterator end()
    {
        return container_.end();
    }

    const_iterator begin() const
    {
        return container_.begin();
    }

    const_iterator end() const
    {
        return container_.end();
    }

    const_iterator cbegin() const
    {
        return container_.cbegin();
    }

    const_iterator cend() const
    {
        return container_.cend();
    }

    template<class Key>
    iterator find()
    {
        return container_.find(type_id<Key>());
    }

    template<class Key>
    const_iterator find() const
    {
        return container_.find(type_id<Key>());
    }

    template<class Key>
    void put(ValueType&& value)
    {
        container_[type_id<Key>()] = std::forward<ValueType>(value);
    }

private:
    Container container_;
};

}}

#endif /* INJECTPP_TYPEMAP_HPP_HEADER__ */
