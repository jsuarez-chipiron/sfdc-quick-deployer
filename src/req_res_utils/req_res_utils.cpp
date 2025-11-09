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

std::string fix_double_quote(std::string body) //NOLINT
{
    char sustitutee = '"';
 
    size_t pos = -1;
    size_t offset = 0;
    while ((pos = body.substr(offset).find(sustitutee)) != std::string::npos) {
        offset += pos+3;
        body.replace(offset-3, 1, R"(\")");
    }
    return body;
}

std::string fix_tab(std::string body) //NOLINT
{
    char sustitutee = '\t';
 
    size_t pos = -1;
    size_t offset = 0;
    while ((pos = body.substr(offset).find(sustitutee)) != std::string::npos) {
        offset += pos+1;
        body.replace(offset-1, 1, "    ");
        std::cout << "WARNING: tab replaced at position " << offset-1 << '\n';

    }
    return body;
}

std::string req_res_utils::fix_request_body(std::string body)
{
    body = fix_double_quote(body);
    body = fix_tab(body);
    return body;
}

std::string req_res_utils::fix_response_body(const std::string& body)
{
    std::string new_body;
    new_body.reserve(body.length());
    size_t pos = 0;

    for (pos=0; pos<body.length(); ++pos)
    {
        char c = body[pos];
        if ( c != '\\' ) { new_body += c; }
        else
        {
            size_t next_char_index = pos+1;
            if ( next_char_index < body.length() && body[next_char_index] == 't' )
            {
                std::cout << "WARNING: tab replaced for 4 spaces at position: " << pos << '\n';
                new_body += "    ";
                pos++;
            }
            else { new_body += c; }
        }
    }
    return new_body;
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

std::string req_res_utils::parse_class_response(std::string_view body)
{
    auto partial = body;
    auto found_begin = body.find(R"("Body":)");
    if ( found_begin != std::string_view::npos )
    {
        partial = partial.substr(found_begin+8);
    }
    auto found_end = partial.find(R"("LengthWithoutComments":)");
    if ( found_end != std::string_view::npos )
    {
        partial = partial.substr(0, found_end-2);
        return std::string(partial);
    }

    return "";
}
