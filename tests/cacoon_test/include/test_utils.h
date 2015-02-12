
#ifndef TESTS_UTILS_H
#define TESTS_UTILS_H

#include <string>
#include <fstream>

struct test_utils {

    static void empty_file(const std::string& loc) {
        write_to_file(loc, "");
    }

    static void write_to_file(const std::string& loc, const std::string& to_write, const bool truncate = true) {
        std::ios_base::openmode mode = std::ios_base::out;
        if (truncate) {
            mode |= std::ios_base::trunc;
        } else {
            mode |= std::ios_base::app;
        }
        std::ofstream os(loc, mode);
        os << to_write;
        os.flush();
    }

};

#endif
