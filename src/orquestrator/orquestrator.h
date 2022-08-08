#ifndef ORQUESTRATOR_H
#define ORQUESTRATOR_H

#include <iostream>
#include "../sfdc_client/sfdc_client.h"
#include "../resource_repo/resource_repo.h"
#include "../resource_reader/resource_reader.h"

class orquestrator
{
    public:
        orquestrator(sfdc_client&& sfdc_client, resource_repo&& resource_repo):
            sfdc_client_(std::move(sfdc_client)),
            resource_repo_(std::move(resource_repo))
        {}

        int execute(int argc, char** argv);

        void update_login(const std::string& login_url, const std::string& username, const std::string& password);

        void upload_resource(const std::string& resource_filepath);

    private:
        sfdc_client sfdc_client_;
        resource_repo resource_repo_;

        static std::tuple<bool, bool, std::string, std::string, std::string, std::string> parse_flags(int argc, char** argv);

        static std::string get_filename_from_filepath(const std::string& filepath);
};
#endif
