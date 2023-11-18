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

#include "../../include/loki/common/filesystem.hpp"

#include <string>
#include <fstream>
#include <sstream>


namespace loki {

std::string read_file(const fs::path& file_path) {
    std::ifstream file(file_path.c_str());
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        // TODO remove comments, change tabs to 4 spaces
        return buffer.str();
    }
    throw std::runtime_error("file does not exist at " + std::string(file_path.c_str()));
}

}