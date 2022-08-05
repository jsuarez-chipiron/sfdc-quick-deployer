#include <iostream>
#include <fstream>
// #include <sstream>
#include "resource_reader.h"

std::string resource_reader::get_filepath() const
{
    return filepath_;
}

std::string resource_reader::create_body() const
{
    std::string tmpl;
    std::ifstream infile(filepath_);

    std::string line;
    while (std::getline(infile, line))
    {
        tmpl += line + "\\n";
    }
    std::string ret = R"({"Body": ")";
    ret += tmpl + "\"}";

    return ret;
}
