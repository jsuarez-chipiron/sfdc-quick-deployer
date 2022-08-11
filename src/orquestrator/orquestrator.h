#ifndef ORQUESTRATOR_H
#define ORQUESTRATOR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>
#include "../sfdc_client/sfdc_client.h"
#include "../resource_repo/resource_repo.h"
#include "../req_res_utils/req_res_utils.h"

class orquestrator
{
    public:
        orquestrator(sfdc_client&& sfdc_client, resource_repo&& resource_repo):
            sfdc_client_(std::move(sfdc_client)),
            resource_repo_(std::move(resource_repo))
        {}

        int execute(int argc, char** argv);

        int update_login(const std::string& login_url, const std::string& username, const std::string& password);

        int upload_resource(const std::string& resource_filepath);

        int download_resource(const std::string& resource_filepath);

        int get_all_resources(const std::string& orgid);

    private:
        sfdc_client sfdc_client_;
        resource_repo resource_repo_;

        static std::tuple<bool, bool, bool, bool, std::string, std::string, std::string, std::string> parse_flags(int argc, char** argv);

        static std::string get_filename_from_filepath(const std::string& filepath);

        static std::string get_state_async_request(const std::string& body);

        static std::string get_problem_async_request(const std::string& body);

        static std::vector<resource> get_all_resources_parser(std::string_view body, const std::string& orgid);

        static bool write_class_to_file(const std::string& filename, std::string file_content);
};
#endif
