#ifndef ORQUESTRATOR_H
#define ORQUESTRATOR_H

#include <iostream>
#include "../sfdc_client/sfdc_client.h"

class orquestrator
{
    public:
        explicit orquestrator(sfdc_client sfdc_client): sfdc_client_(std::move(sfdc_client)) {}

        void test();

    private:
        sfdc_client sfdc_client_;
};
#endif
