#include <iostream>
#include "orquestrator.h"

void orquestrator::test()
{
    const auto [httpcode, message] = sfdc_client_.create_class(resource_reader_.create_body());

    std::cout << "httpcode: " << httpcode << "\n";
    std::cout << "message: " << message << "\n";

    sfdc_client_.delete_class(message);

    resource_repo_.print_repo();
}

void orquestrator::upload_resource(const std::string& resource_filepath, const std::string& orgid)
{
    std::string filename = utils_.get_filename_from_filepath(resource_filepath);
    std::cout << "upload_resource: " << filename << "\n";

    std::string identifier = filename + orgid;
    std::cout << "upload_resource2: " << identifier << "\n";

    if ( resource_repo_.get_repo().contains(identifier) )
    {
        //update
        std::cout << "IMPLEMENT ME!!!\n";
    }
    else
    {
        // create new
        const auto [code, message] = sfdc_client_.create_class(resource_reader_.create_body());

        std::cout << "httpcode: " << code << "\n";
        std::cout << "message: " << message << "\n";

        if ( code != 0 )
        {
            std::cerr << "ERROR: " << message << "\n";
            return;
        }

        resource new_resource(filename, message, orgid);
        if ( !resource_repo_.insert(identifier, new_resource) )
        {
            std::cerr << "ERROR: inserting in the repo\n";
            return;
        }

        sfdc_client_.delete_class(message);

        resource_repo_.print_repo();
    }

}


