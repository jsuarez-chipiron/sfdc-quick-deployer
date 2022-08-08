#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include "sfdc_client/sfdc_client.h"
#include "orquestrator/orquestrator.h"
#include "resource_repo/resource_repo.h"
#include "resource_reader/resource_reader.h"

int main(int argc, char *argv[])
{
    std::vector<std::string> arguments = std::vector<std::string>(argv, argv + argc);
    std::vector<const char *> converted_argv(arguments.size());
    std::transform(arguments.begin(), arguments.end(), converted_argv.begin(), [](std::string& str){
        return str.c_str();
    });

    std::string filepath = "../resources/dictionary.dat";
    std::string classpath = "../resources/class.cls";

    resource_repo r_repo(filepath);
    sfdc_client client(r_repo.get_login_details());
    resource_reader rreader(classpath);

    orquestrator orq(std::move(client), std::move(r_repo), std::move(rreader));

    return orq.execute(converted_argv.size(), const_cast<char**>(converted_argv.data())); //NOLINT
}
