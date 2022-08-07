#include <iostream>
#include "sfdc_client/sfdc_client.h"
#include "orquestrator/orquestrator.h"
#include "resource_repo/resource_repo.h"
#include "resource_reader/resource_reader.h"

int main()
{
    // std::string endpoint = "https://xxxxxxxx.my.salesforce.com/services/data/v55.0/tooling/sobjects/ApexClass";
    // std::string token = "xxxxxxxx";
    std::string filepath = "../resources/dictionary.dat";
    std::string classpath = "../resources/class.cls";

    resource_repo rrepo(filepath);
    sfdc_client client(rrepo.get_login_details());
    resource_reader rreader(classpath);

    // rrepo.print_repo();

    orquestrator orq(std::move(client), std::move(rrepo), std::move(rreader));
    orq.upload_resource(classpath);
    // orq.test();

    return 0;
}
