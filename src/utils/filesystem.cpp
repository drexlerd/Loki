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

#include "loki/details/utils/filesystem.hpp"

#include "loki/details/exceptions.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

namespace loki
{

std::string read_file(const fs::path& file_path)
{
    std::ifstream file(file_path.c_str());
    if (!file.is_open())
    {
        throw FileNotExistsError(std::string(file_path.c_str()));
    }

    std::stringstream buffer;
    std::string line;

    while (std::getline(file, line))
    {
        // Strip comments
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos)
        {
            line = line.substr(0, commentPos);
        }

        // Replace tabs with four spaces
        size_t tabPos = 0;
        while ((tabPos = line.find('\t', tabPos)) != std::string::npos)
        {
            line.replace(tabPos, 1, "    ");
            tabPos += 4;  // Move past the new spaces
        }

        // Convert line to lowercase
        std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c) { return std::tolower(c); });

        buffer << line << '\n';
    }
    return buffer.str();
}

}