#include <iostream>
#include "sfdc_client/sfdc_client.h"
#include "orquestrator/orquestrator.h"
#include "resource_repo/resource_repo.h"

#include "resource_reader/resource_reader.h"

int main()
{
    std::string endpoint = "https://xxxxxxxx.my.salesforce.com/services/data/v55.0/tooling/sobjects/ApexClass";
    std::string token = "xxxxxxxx";
    std::string filepath = "/home/javier/Projects/salesforce/quick-deployer/resources/dictionary.dat";

    sfdc_client client(endpoint, token);

    resource_reader rreader("../../resources/class.cls");

    const auto [httpcode, message] = client.create_class(rreader.create_body());

    std::cout << "httpcode: " << httpcode << "\n";
    std::cout << "message: " << message << "\n";

    client.delete_class(message);

    resource_repo rr(filepath);
    rr.print_repo();
    orquestrator orq(std::move(client), std::move(rr));
    orq.test();


    return 0;
}
