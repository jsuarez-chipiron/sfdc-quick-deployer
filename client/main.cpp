#include <iostream>
#include "sfdc_client/sfdc_client.h"
#include "orquestrator/orquestrator.h"
#include "resource_repo/resource_repo.h"

int main()
{
    std::string endpoint = "https://jadm.my.salesforce.com/services/data/v55.0/tooling/sobjects/ApexClass";
    std::string token = "00D24000000KjJn!AQsAQJXIHg3w9wvZeSZHfwSl0i8ePbo4bVqONkrhIJmiomVPHNxLLK6rkQtY_ZYndJBlLdoOnZZu58RLN4_exkJAWMsg751m";
    std::string config_filename = "/home/javier/Projects/salesforce/quick-deployer/resources/dictionary.dat";

    sfdc_client client(endpoint, token);

    std::string body = R"({ "Body": "public class MySuperCoolTest{\n\tpublic String sayHi() {\n\t\treturn 'hello';\n\t}\n}"})";
    const auto [httpcode, message] = client.create_class(body);
    std::cout << "httpcode: " << httpcode << "\n";
    std::cout << "message: " << message << "\n";

    client.delete_class(message);

    resource_repo rr(config_filename);
    rr.print_repo();
    orquestrator orq(std::move(client), std::move(rr));
    orq.test();

    return 0;
}
