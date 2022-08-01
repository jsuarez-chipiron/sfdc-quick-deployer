#include <iostream>
#include "sfdc_client/sfdc_client.h"
#include "orquestrator/orquestrator.h"

int main()
{
    std::string endpoint = "https://jadm.my.salesforce.com/services/data/v55.0/tooling/sobjects/ApexClass";
    std::string token = "00D24000000KjJn!AQsAQJXIHg3w9wvZeSZHfwSl0i8ePbo4bVqONkrhIJmiomVPHNxLLK6rkQtY_ZYndJBlLdoOnZZu58RLN4_exkJAWMsg751m";

    sfdc_client client(endpoint, token);

    std::string body = R"({ "Body": "public class MySuperCoolTest{\n\tpublic String sayHi() {\n\t\treturn 'hello';\n\t}\n}"})";
    const auto [httpcode, message] = client.create_class(body);
    std::cout << "httpcode: " << httpcode << "\n";
    std::cout << "message: " << message << "\n";

    client.delete_class(message);

    orquestrator orq(std::move(client));
    orq.test();

    return 0;
}
