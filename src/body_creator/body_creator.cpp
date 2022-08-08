#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include "body_creator.h"

std::string body_creator::get_filepath() const
{
    return filepath_;
}

std::string body_creator::create_body() const
{
    std::string tmpl;
    std::ifstream infile(filepath_);

    std::string line;
    while (std::getline(infile, line))
    {
        tmpl += line + "\\n";
    }
    std::string ret = R"({"Body": ")";
    ret += tmpl + "\"}";

    return ret;
}

std::string body_creator::metadata_container_body()
{
    auto millisec_since_epoch = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::stringstream ss;
    ss << R"({"Name": ")" << millisec_since_epoch << "\"}";
    return ss.str();
}

std::string body_creator::apexmember_body()
{

// {
//     "ContentEntityId": "01p0800000EccFYAAZ",
//     "MetadataContainerId": "1dc08000001Dav0AAC",
//     "Body": "public class MySuperCoolTest {\n\tpublic String sayHello2222() {\n\t\treturn 'hello';\n\t}\n}"
// }
    auto millisec_since_epoch = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::stringstream ss;
    ss << R"({"Name": ")" << millisec_since_epoch << "\"}";
    return ss.str();
}

