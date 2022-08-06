#ifndef ORQUESTRATOR_H
#define ORQUESTRATOR_H

#include <iostream>
#include "../sfdc_client/sfdc_client.h"
#include "../resource_repo/resource_repo.h"
#include "../resource_reader/resource_reader.h"

class orquestrator
{
    public:
        orquestrator(sfdc_client&& sfdc_client, resource_repo&& resource_repo, 
                resource_reader&& resource_reader):
            sfdc_client_(std::move(sfdc_client)),
            resource_repo_(std::move(resource_repo)),
            resource_reader_(std::move(resource_reader))
        {}

        void test();

        void upload_resource(const std::string& resource_filepath);

    private:
        sfdc_client sfdc_client_;
        resource_repo resource_repo_;
        resource_reader resource_reader_;

        static std::string get_filename_from_filepath(const std::string& filepath);
};
#endif
