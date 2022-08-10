#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include <timer.h>
#include "sfdc_client/sfdc_client.h"
#include "orquestrator/orquestrator.h"
#include "resource_repo/resource_repo.h"

int main(int argc, char *argv[])
{
    timer t;

    std::vector<std::string> arguments = std::vector<std::string>(argv, argv + argc);
    std::vector<const char *> converted_argv(arguments.size());
    std::transform(arguments.begin(), arguments.end(), converted_argv.begin(), [](std::string& str){
        return str.c_str();
    });

    std::string directory = "/home/javier/Tech/c++/sfdc-quick-deployer/resources/dictionary.dat"; //TODO: put the dictionary in a config dir

    resource_repo r_repo(directory);
    sfdc_client client(r_repo.get_login_details());

    orquestrator orq(std::move(client), std::move(r_repo));

    auto retcode = orq.execute(converted_argv.size(), const_cast<char**>(converted_argv.data())); //NOLINT
    std::cout << t.elapsed() << "ms\n";

    return retcode;
}
