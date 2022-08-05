#include <iostream>
#include "sfdc_client/sfdc_client.h"
#include "orquestrator/orquestrator.h"
#include "resource_repo/resource_repo.h"
#include "resource_reader/resource_reader.h"
#include "utils/utils.h"

int main()
{
    std::string endpoint = "https://xxxxxxxx.my.salesforce.com/services/data/v55.0/tooling/sobjects/ApexClass";
    std::string token = "xxxxxxxx";
    std::string filepath = "/home/javier/Projects/salesforce/quick-deployer/resources/dictionary.dat";
    std::string classpath = "../../resources/class.cls";

    sfdc_client client(endpoint, token);
    resource_repo rrepo(filepath);
    resource_reader rreader(classpath);
    utils utils;

    // rrepo.print_repo();

    orquestrator orq(std::move(client), std::move(rrepo), std::move(rreader), std::move(utils));
    orq.upload_resource(classpath, "xxxxx");
    // orq.test();

    return 0;
}
