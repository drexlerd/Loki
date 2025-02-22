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

#ifndef LOKI_INCLUDE_LOKI_PDDL_TRANSLATOR_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_TRANSLATOR_HPP_

#include "loki/details/pddl/declarations.hpp"

namespace loki
{

class DomainTranslationResult
{
private:
    Domain original_domain;  ///< use it to check that a problem can be translated with this result.

    Domain domain;

    // TODO: add more if needed

public:
    DomainTranslationResult(Domain original_domain, Domain domain);
};

extern DomainTranslationResult translate(const Domain& domain);

class ProblemTranslationResult
{
private:
    Problem problem;

public:
    explicit ProblemTranslationResult(Problem problem);
};

extern ProblemTranslationResult translate(const Problem& problem, const DomainTranslationResult& result);

}

#endif
