#include <iostream>
#include <fstream>
#include <unordered_map>
#include "resource_repo.h"
#include "../serdes/resource_serdes.h"

resource_repo::resource_repo(const std::string& filepath)
{
    std::ifstream infile(filepath);

    resource_serdes re_serdes;

    std::string line;
    while (std::getline(infile, line))
    {
        auto res = re_serdes.deserialize(line);
        repo_.insert({res.get_classname(), res});
    }
}

void resource_repo::print_repo() const
{
    for (const auto& item: repo_)
    {
        std::cout << item.first << ": " << item.second.get_classname() << item.second.get_classid() << item.second.get_orgid() << "\n";
    }
}


/* resource */
std::string resource::get_classname() const
{
    return classname_;
}

std::string resource::get_classid() const
{
    return classid_;
}

std::string resource::get_orgid() const
{
    return orgid_;
}

