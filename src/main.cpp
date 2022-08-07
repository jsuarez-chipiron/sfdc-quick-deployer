#include <iostream>
#include "sfdc_client/sfdc_client.h"
#include "orquestrator/orquestrator.h"
#include "resource_repo/resource_repo.h"
#include "resource_reader/resource_reader.h"

int main()
{
    // std::string endpoint = "https://jadm.my.salesforce.com/services/data/v55.0/tooling/sobjects/ApexClass";
    // std::string token = "00D24000000KjJn!AQsAQHGpxLHHtAdDkOwqklIEehrTUV0DPe3Z87fqWNPSEOqNjw20LW.ioIylOlrtriLPfrtm_yT.3v0Sfg6mj9xseoIcdAGw";
    std::string filepath = "../resources/dictionary.dat";
    std::string classpath = "../resources/class.cls";

    resource_repo rrepo(filepath);
    sfdc_client client(rrepo.get_login_details());
    resource_reader rreader(classpath);

    orquestrator orq(std::move(client), std::move(rrepo), std::move(rreader));

    orq.test();

    orq.upload_resource(classpath);

    return 0;
}
