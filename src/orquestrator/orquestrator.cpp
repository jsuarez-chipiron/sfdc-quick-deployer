#include <iostream>
#include <string>
#include "orquestrator.h"

void orquestrator::test()
{
    const auto [code, url, token] = sfdc_client_.login("https://login.salesforce.com/services/Soap/c/55.0/xxxxxxx", "username@force.com", "xxxxxxx");
    std::cout << "resp_code: " << code << "\n";
    std::cout << "url: " << url << "\n";
    std::cout << "token: " << token << "\n";
}

void orquestrator::upload_resource(const std::string& resource_filepath)
{
    std::string orgid = std::get<1>(resource_repo_.get_login_details());
    std::string filename = get_filename_from_filepath(resource_filepath);
    std::string identifier = filename + orgid;

    if ( resource_repo_.get_repo().contains(identifier) )
    {
        // si existe lo borramos antes
        auto updatable_resource = resource_repo_.get_repo().at(identifier);

        const auto [code, message] = sfdc_client_.delete_class(updatable_resource.get_classid());

        if ( code != 0 )
        {
            std::cerr << "ERROR: " << message << "\n";
            exit(-1); //NOLINT
        }

        resource_repo_.delete_from_repo(identifier);
    }

    const auto [code, message] = sfdc_client_.create_class(resource_reader_.create_body());

    if ( code != 0 )
    {
        std::cerr << "ERROR: " << message << "\n";
        exit(-1); //NOLINT
    }

    resource new_resource(filename, message, orgid);
    if ( !resource_repo_.insert(identifier, new_resource) )
    {
        std::cerr << "ERROR: inserting in the repo\n";
        exit(-1); //NOLINT
    }

    std::cout << "Resource uploaded correctly: [" << message << "]\n";
}

std::string orquestrator::get_filename_from_filepath(const std::string& filepath)
{
    auto found_slash = filepath.find_last_of('/');
    size_t begin = 0;

    if ( found_slash != std::string::npos )
    {
        begin = found_slash+1;
    }

    auto found_dot = filepath.find_last_of('.');

    if ( found_dot == std::string::npos ) { return ""; }

    return filepath.substr(begin, found_dot-begin);
}
