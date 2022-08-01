#include <iostream>
#include "sfdc_client.h"

int main()
{
    std::string endpoint = "https://xxxxxxxx.my.salesforce.com/services/data/v55.0/tooling/sobjects/ApexClass";
    std::string token = "xxxxxxxx";

    sfdc_client client(endpoint, token);

    std::string body = R"({ "Body": "public class MySuperCoolTest{\n\tpublic String sayHi() {\n\t\treturn 'hello';\n\t}\n}"})";
    const auto [httpcode, message] = client.create_class(body);
    std::cout << "httpcode: " << httpcode << "\n";
    std::cout << "message: " << message << "\n";

    client.delete_class(message);

    return 0;
}
