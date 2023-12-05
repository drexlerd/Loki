
namespace loki {

template<typename... Ts>
template<typename T>
std::optional<ValueType<T>> Bindings<Ts...>::get(const std::string& key) const {
    const auto& t_bindings = std::get<MapType<T>>(bindings);
    auto it = t_bindings.find(key);
    if (it != t_bindings.end()) {
        return {it->second};
    }
    return std::optional<ValueType<T>>();
}


template<typename... Ts>
template<typename T>
std::optional<ValueType<T>> Bindings<Ts...>::getWithParent(const std::string& key, const Scope* parent_scope) const {
    auto result = get<T>(key);
    if (result.has_value() || !parent_scope) {
        return result;
    }
    return parent_scope->get<T>(key);
}

template<typename... Ts>
template<typename T>
const MapType<T>& Bindings<Ts...>::get() const {
    return std::get<MapType<T>>(bindings);
}

template<typename... Ts>
template<typename T>
MapType<T> Bindings<Ts...>::getWithParent(const Scope* parent_scope) const {
    MapType<T> result = get<T>();
    if (parent_scope) {
        auto parent_bindings = parent_scope->get<T>();
        result.insert(parent_bindings.begin(), parent_bindings.end());
    }
    return result;
}

template<typename... Ts>
template<typename T>
void Bindings<Ts...>::insert(const std::string& key, const BindingPtrType<T>& binding, const std::optional<PositionType>& position) {
    assert(binding);
    auto& t_bindings = std::get<MapType<T>>(bindings);
    assert(!t_bindings.count(key));
    t_bindings.emplace(key, ValueType<T>{binding, position});
}


template<typename T>
std::optional<ValueType<T>> Scope::get(const std::string& name) const {
    return bindings.getWithParent<T>(name, m_parent_scope.get());
}

template<typename T>
MapType<T> Scope::get() const {
    return bindings.getWithParent<T>(m_parent_scope.get());
}

template<typename T>
void Scope::insert(const std::string& name, const BindingPtrType<T>& binding, const std::optional<PositionType>& position) {
    assert(binding);
    assert(!this->get<T>(name));
    bindings.insert<T>(name, binding, position);
}

}