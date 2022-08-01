#ifndef ORQUESTRATOR_H
#define ORQUESTRATOR_H

#include <iostream>
#include "../sfdc_client/sfdc_client.h"
#include "../resource_repo/resource_repo.h"

class orquestrator
{
    public:
        explicit orquestrator(sfdc_client&& sfdc_client, resource_repo&& resource_repo): 
            sfdc_client_(std::move(sfdc_client)),
            resource_repo_(std::move(resource_repo)) 
    {}

        void test();

    private:
        sfdc_client sfdc_client_;
        resource_repo resource_repo_;
};
#endif
