#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include "sfdc_client/sfdc_client.h"
#include "orquestrator/orquestrator.h"
#include "resource_repo/resource_repo.h"
#include "resource_reader/resource_reader.h"

// void call_parse_flags(std::vector<std::string>& args)
// {
//     std::vector<const char *> argv(args.size());
//     std::transform(args.begin(), args.end(), argv.begin(), [](std::string& str){
//         return str.c_str();});
//     orquestrator::parse_flags(argv.size(), const_cast<char**>(argv.data()));
// }

int main(int argc, char *argv[])
{
    std::vector<std::string> arguments = std::vector<std::string>(argv, argv + argc);
    std::vector<const char *> converted_argv(arguments.size());
    std::transform(arguments.begin(), arguments.end(), converted_argv.begin(), [](std::string& str){
        return str.c_str();
    });
    // call_parse_flags(arguments);
    // std::string endpoint = "https://xxxxxxxx.my.salesforce.com/services/data/v55.0/tooling/sobjects/ApexClass";
    // std::string token = "xxxxxxxx";
    std::string filepath = "../resources/dictionary.dat";
    std::string classpath = "../resources/class.cls";

    resource_repo rrepo(filepath);
    sfdc_client client(rrepo.get_login_details());
    resource_reader rreader(classpath);

    orquestrator orq(std::move(client), std::move(rrepo), std::move(rreader));

    return orq.execute(converted_argv.size(), const_cast<char**>(converted_argv.data())); //NOLINT
    // orq.test();

    // orq.upload_resource(classpath);


    // return 0;
}
