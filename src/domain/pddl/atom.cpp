/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
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

#include "../../../include/loki/domain/pddl/atom.hpp"

#include "../../../include/loki/domain/pddl/predicate.hpp"
#include "../../../include/loki/domain/pddl/term.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
AtomImpl::AtomImpl(int identifier, const Predicate& predicate, const TermList& terms)
    : Base(identifier)
    , m_predicate(predicate)
    , m_terms(terms)
{
}

bool AtomImpl::are_equal_impl(const AtomImpl& other) const {
    return (m_predicate == other.m_predicate)
        && (m_terms == other.m_terms);
}

size_t AtomImpl::hash_impl() const {
    return hash_combine(m_predicate, hash_vector(m_terms));
}

class StrTermVisitor : public TermVisitor {
public: 
    std::stringstream& out;

    explicit StrTermVisitor(std::stringstream& out_) : out(out_) { }

    void visit(const std::shared_ptr<const TermConstantImpl>& term) {
        out << *term;
    }

    void visit(const std::shared_ptr<const TermVariableImpl>& term) {
        out << *term;
    }
};

void AtomImpl::str_impl(std::stringstream& out, const FormattingOptions& options) const {
    out << "(" << m_predicate->get_name() << " ";
    StrTermVisitor visitor(out);
    for (size_t i = 0; i < m_terms.size(); ++i) {
        if (i != 0) out << " ";
        m_terms[i]->accept(visitor);
    }
    out << ")";
}

const Predicate& AtomImpl::get_predicate() const {
    return m_predicate;
}

const TermList& AtomImpl::get_terms() const {
    return m_terms;
}

}


namespace std {
    bool less<loki::pddl::Atom>::operator()(
        const loki::pddl::Atom& left_atom,
        const loki::pddl::Atom& right_atom) const {
        return *left_atom < *right_atom;
    }

    std::size_t hash<loki::pddl::AtomImpl>::operator()(const loki::pddl::AtomImpl& atom) const {
        return atom.hash_impl();
    }
}
