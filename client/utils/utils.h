#ifndef UTILS_H
#define UTILS_H

#include <iostream>

class utils
{
    public:
        utils() = default;

        utils(const utils& rhs) = delete;

        utils(utils&& rhs) = default;

        utils& operator=(const utils& rhs) = delete;

        utils& operator=(utils&& rhs) = default;

        ~utils() = default;

        static std::string get_filename_from_filepath(const std::string& filepath);
};

#endif
