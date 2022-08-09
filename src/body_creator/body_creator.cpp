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

std::string body_creator::parse_body() const
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

std::string body_creator::insert_body(const std::string& body)
{
    std::stringstream ss;
    ss << R"({"Body": ")" << body << "\"}";
    return ss.str();
}

std::string body_creator::metadata_container_body()
{
    auto millisec_since_epoch = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::stringstream ss;
    ss << R"({"Name": ")" << millisec_since_epoch << "\"}";
    return ss.str();
}

std::string body_creator::apexmember_body(const std::string& content_entity_id, 
        const std::string& meta_container_id, const std::string& body)
{
    std::stringstream sst;
    sst << R"({"ContentEntityId": ")" << content_entity_id 
        << R"(", "MetadataContainerId": ")" << meta_container_id
        << R"(", "Body": ")" << body << R"("})";

    return sst.str();
}

std::string body_creator::async_request_body(const std::string& meta_container_id)
{
    std::stringstream sst;
    sst << R"({"IsCheckOnly": false, "MetadataContainerId": ")" << meta_container_id
        << R"("})";

    return sst.str();
}
