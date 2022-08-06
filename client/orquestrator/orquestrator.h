#ifndef ORQUESTRATOR_H
#define ORQUESTRATOR_H

#include <iostream>
#include "../sfdc_client/sfdc_client.h"
#include "../resource_repo/resource_repo.h"
#include "../resource_reader/resource_reader.h"
#include "../utils/utils.h"

class orquestrator
{
    public:
        orquestrator(sfdc_client&& sfdc_client, resource_repo&& resource_repo, 
                resource_reader&& resource_reader, utils&& utils):
            sfdc_client_(std::move(sfdc_client)),
            resource_repo_(std::move(resource_repo)),
            resource_reader_(std::move(resource_reader)),
            utils_(std::move(utils)) 
        {
            std::cout << "2 login details (endpoint): " << std::get<1>(resource_repo_.get_login_details()) << "\n";
            std::cout << "2 login details (token): " << std::get<0>(resource_repo_.get_login_details()) << "\n";
        }

        void test();

        void upload_resource(const std::string& resource_filepath);

    private:
        sfdc_client sfdc_client_;
        resource_repo resource_repo_;
        resource_reader resource_reader_;
        utils utils_;
};
#endif
