/*
 * Copyright (C) 2023 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */



namespace loki {

template<typename... Ts>
template<typename T>
bool BindingReferences<Ts...>::exists(const T* reference) const {
    const auto& t_references = std::get<ReferenceSetType<T>>(references);
    return t_references.count(reference);
}


template<typename... Ts>
template<typename T>
void BindingReferences<Ts...>::track(const T* reference) {
    auto& t_references = std::get<ReferenceSetType<T>>(references);
    t_references.insert(reference);
}


template<typename... Ts>
template<typename T>
void BindingReferences<Ts...>::untrack(const T* reference) {
    auto& t_references = std::get<ReferenceSetType<T>>(references);
    t_references.erase(reference);
}

template<typename... Ts>
template<typename T>
bool EnumReferences<Ts...>::exists(T reference) const {
    const auto& t_references = std::get<ValueReferenceSetType<T>>(references);
    return t_references.count(reference);
}


template<typename... Ts>
template<typename T>
void EnumReferences<Ts...>::track(T reference) {
    auto& t_references = std::get<ValueReferenceSetType<T>>(references);
    t_references.insert(reference);
}


template<typename... Ts>
template<typename T>
void EnumReferences<Ts...>::untrack(T reference) {
    auto& t_references = std::get<ValueReferenceSetType<T>>(references);
    t_references.erase(reference);
}

}
