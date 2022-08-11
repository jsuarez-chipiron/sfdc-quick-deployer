#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include "req_res_utils.h"

std::string req_res_utils::get_filepath() const
{
    return filepath_;
}

std::string req_res_utils::parse_body() const
{
    std::string ret;
    std::ifstream infile(filepath_);

    std::string line;
    while (std::getline(infile, line))
    {
        ret += line + "\\n";
    }

    return ret;
}

std::string req_res_utils::insert_body(const std::string& body)
{
    std::stringstream ss;
    ss << R"({"Body": ")" << body << "\"}";
    return ss.str();
}

std::string req_res_utils::metadata_container_body()
{
    auto millisec_since_epoch = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::stringstream ss;
    ss << R"({"Name": ")" << millisec_since_epoch << "\"}";
    return ss.str();
}

std::string req_res_utils::apexmember_body(const std::string& content_entity_id, 
        const std::string& meta_container_id, const std::string& body)
{
    std::stringstream sst;
    sst << R"({"ContentEntityId": ")" << content_entity_id 
        << R"(", "MetadataContainerId": ")" << meta_container_id
        << R"(", "Body": ")" << body << R"("})";

    return sst.str();
}

std::string req_res_utils::async_request_body(const std::string& meta_container_id)
{
    std::stringstream sst;
    sst << R"({"IsCheckOnly": false, "MetadataContainerId": ")" << meta_container_id
        << R"("})";

    return sst.str();
}
