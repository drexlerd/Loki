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

#include "loki/details/utils/memory.hpp"

#if defined(__linux__)

#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <unistd.h>

namespace loki
{

std::tuple<double, double> process_mem_usage()
{
    using std::ifstream;
    using std::ios_base;
    using std::string;

    // 'file' stat seems to give the most reliable results
    //
    ifstream stat_stream("/proc/self/stat", ios_base::in);

    // dummy vars for leading entries in stat that we don't care about
    //
    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;

    // the two fields we want
    //
    unsigned long vsize;
    long rss;

    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime
        >> cutime >> cstime >> priority >> nice >> O >> itrealvalue >> starttime >> vsize >> rss;  // don't care about the rest

    stat_stream.close();

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;  // in case x86-64 is configured to use 2MB pages
    double vm_usage = vsize / 1024.0;
    double resident_set = rss * page_size_kb;

    return std::make_tuple(vm_usage, resident_set);
}

}

#endif  // __linux__

#if defined(__APPLE__)

#include "loki/details/utils/memory.hpp"

#include <mach/mach.h>

namespace loki
{

std::tuple<double, double> process_mem_usage()
{
    // macOS-specific implementation
    mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;

    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t) &info, &infoCount) != KERN_SUCCESS)
    {
        return std::make_tuple(0.0, 0.0);
    }

    double vm_usage = info.virtual_size / 1024.0 / 1024.0;       // convert from bytes to MB
    double resident_set = info.resident_size / 1024.0 / 1024.0;  // convert from bytes to MB

    return std::make_tuple(vm_usage, resident_set);
}

}  // namespace utils

#endif  // __APPLE__
