#include <iostream>
#include <fstream>
#include <unordered_map>
#include "resource_repo.h"
#include "../serdes/resource_serdes.h"

resource_repo::resource_repo(const std::string& filepath): filepath_(filepath)
{
    std::ifstream infile(filepath);

    resource_serdes re_serdes;

    std::string line;
    while (std::getline(infile, line))
    {
        auto res = re_serdes.deserialize(line);
        repo_.insert({res.get_classname()+res.get_orgid(), res});
    }

    //TODO: dejar de usor el resource y crear directamente la tupla de login details
    auto login_details = read_login_details();
    if ( std::get<0>(login_details).empty() )
    {
        std::cerr << "ERROR: invalid login config\n";
        exit(-1);
    }
    login_details_ = login_details;
}

std::tuple<std::string, std::string> resource_repo::get_login_details() const
{
    return login_details_;
}

std::unordered_map<std::string, resource> resource_repo::get_repo() const
{
    return repo_;
}

std::tuple<std::string, std::string> resource_repo::read_login_details() const
{
    for (const auto& item: repo_)
    {
        if ( item.second.get_classname() == "*" )
        {
            auto ret = item.second;
            return std::make_tuple(ret.get_classid(), ret.get_orgid());
        }
    }
    return std::make_tuple("","");
}

void resource_repo::print_repo() const
{
    for (const auto& item: repo_)
    {
        std::cout << item.first << ": " << item.second.get_classname() << item.second.get_classid() << item.second.get_orgid() << "\n";
    }
}

bool resource_repo::write_to_file() const
{
    std::cout << "write_to_file: " << filepath_ << "\n";
    std::ofstream out(filepath_);
    resource_serdes re_serdes;
    for (const auto& item: repo_)
    {
        out << re_serdes.serialize(item.second) << "\n";
    }
    out.close();
    return true;
}

bool resource_repo::delete_from_repo(const std::string& identifier)
{
    repo_.erase(identifier);
    return write_to_file();
}

bool resource_repo::insert(const std::string& identifier, const resource& res)
{
    auto insert_result = repo_.insert({identifier, res});
    return insert_result.second && write_to_file();
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

