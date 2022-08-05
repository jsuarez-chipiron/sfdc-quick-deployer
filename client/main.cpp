#include <iostream>
#include "sfdc_client/sfdc_client.h"
#include "orquestrator/orquestrator.h"
#include "resource_repo/resource_repo.h"
#include "resource_reader/resource_reader.h"
#include "utils/utils.h"

int main()
{
    std::string endpoint = "https://jadm.my.salesforce.com/services/data/v55.0/tooling/sobjects/ApexClass";
    std::string token = "00D24000000KjJn!AQsAQHGpxLHHtAdDkOwqklIEehrTUV0DPe3Z87fqWNPSEOqNjw20LW.ioIylOlrtriLPfrtm_yT.3v0Sfg6mj9xseoIcdAGw";
    std::string filepath = "/home/javier/Projects/salesforce/quick-deployer/resources/dictionary.dat";

    sfdc_client client(endpoint, token);
    resource_repo rrepo(filepath);
    resource_reader rreader("../../resources/class.cls");
    utils utils;

    rrepo.print_repo();
    orquestrator orq(std::move(client), std::move(rrepo), std::move(rreader), std::move(utils));
    // orq.test();
    orq.upload_resource("../../resources/class.cls", "xxxxx");

    return 0;
}
