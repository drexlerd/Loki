#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONTEXT_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONTEXT_HPP_

#include "action.hpp"
#include "atom.hpp"
#include "conditions.hpp"
#include "declarations.hpp"
#include "derived_predicate.hpp"
#include "domain.hpp"
#include "effects.hpp"
#include "function.hpp"
#include "function_skeleton.hpp"
#include "literal.hpp"
#include "object.hpp"
#include "parameter.hpp"
#include "predicate.hpp"
#include "requirements.hpp"
#include "term.hpp"
#include "type.hpp"
#include "variable.hpp"

#include "../../common/factory.hpp"

#include <cassert>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <memory>
#include <tuple>


namespace loki::domain {
    template<typename... Ts>
    class Bindings {
        private:
            std::tuple<std::unordered_map<std::string, std::shared_ptr<const Ts>>...> bindings;

        public:
            /// @brief Gets a binding of type T. Returns nullptr if it does not exist.
            template<typename T> 
            std::shared_ptr<const T> get(const std::string& key) const {
                const auto& t_bindings = std::get<T>(bindings);
                auto it = t_bindings.find(key);
                if (it != t_bindings.end()) {
                    return it->second;
                }
                return nullptr;
            }

            /// @brief Inserts a binding of type T
            template<typename T>
            void insert(const std::string& key, std::shared_ptr<const T> binding) {
                auto& t_bindings = std::get<T>(bindings);
                assert(!t_bindings.count(key));
                t_bindings.emplace(key, binding);
            }
    };

    /// @brief Contains scoped bindings
    class Scope {
        private:
            std::shared_ptr<const Scope> m_parent_scope;

            Bindings<pddl::TypeImpl
                , pddl::ObjectImpl
                , pddl::PredicateImpl 
                , pddl::FunctionSkeletonImpl
                , pddl::VariableImpl> bindings;

        public:
            explicit Scope(std::shared_ptr<const Scope> parent_scope = nullptr) 
                : m_parent_scope(parent_scope) { }

            /// @brief Resolves the name. Returns nullptr if the name could not be resolved.
            template<typename T>
            std::shared_ptr<const T> get(const std::string& name) const {
                auto result = bindings.get<T>(name);
                if (result) return result;
                if (m_parent_scope) return this->get<T>(name);
                return nullptr;
            }

            /// @brief Insert binding of type T.
            template<typename T>
            void insert(const std::string& name, std::shared_ptr<const T> binding) {
                assert(!this->get<T>(name));
                bindings.insert<T>(name, binding);
            }
    };


    struct Context {
        std::unique_ptr<std::ostringstream> error_stream;

        ReferenceCountedObjectFactory<pddl::RequirementsImpl
            , pddl::TypeImpl
            , pddl::VariableImpl
            , pddl::TermConstantImpl
            , pddl::TermVariableImpl
            , pddl::ObjectImpl
            , pddl::AtomImpl
            , pddl::LiteralImpl
            , pddl::ParameterImpl
            , pddl::PredicateImpl
            , pddl::FunctionImpl
            , pddl::FunctionSkeletonImpl
            , pddl::ConditionLiteralImpl
            , pddl::ConditionAndImpl
            , pddl::ConditionOrImpl
            , pddl::ConditionNotImpl
            , pddl::EffectLiteralImpl
            , pddl::EffectAndImpl
            , pddl::ActionImpl
            , pddl::DerivedPredicateImpl
            , pddl::DomainImpl> cache;

        std::deque<std::shared_ptr<Scope>> scopes;   

        // Singletons for convenience.
        pddl::Requirements requirements;
        pddl::Type base_type_object;
        pddl::Type base_type_number;

        Context(std::unique_ptr<std::ostringstream>&& error_stream_)
            : error_stream(std::move(error_stream_)) {
            // Initialize the global scope
            auto global_scope = std::make_shared<Scope>(nullptr);
            scopes.push_back(global_scope);

            // create base types.
            base_type_object = cache.get_or_create<pddl::TypeImpl>("object").object;
            base_type_number = cache.get_or_create<pddl::TypeImpl>("number").object;
            global_scope->insert("object", base_type_object);
            global_scope->insert("number", base_type_number);
        }
    };
}


#endif
