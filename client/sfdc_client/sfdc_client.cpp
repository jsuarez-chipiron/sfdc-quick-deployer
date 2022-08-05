#include <iostream>
#include <string>
#include <curl/curl.h>
#include "sfdc_client.h"

sfdc_client::~sfdc_client()
{
    curl_easy_cleanup(curl_);
    std::cout << "ciao sfdc_client\n";
}

std::pair<int, std::string> sfdc_client::create_class(const std::string body)
{
    std::string response_string;
    if (curl_)
    {
        curl_easy_setopt(curl_, CURLOPT_URL, endpoint_.c_str());
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, body.c_str());

        struct curl_slist *headers = nullptr;

        std::string auth_header = "Authorization: Bearer " + token_;
        std::cout << auth_header << "\n";

        headers = curl_slist_append(headers, auth_header.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, body.size());
        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_fun);
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response_string);

        CURLcode res = curl_easy_perform(curl_);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << "\n";
            exit(-1);
        }
        else
        {
            std::cout << "response: " << response_string << "\n";
        }

        long http_code = 0;
        curl_easy_getinfo (curl_, CURLINFO_RESPONSE_CODE, &http_code);

        if ( http_code != 201 )
        {
            return std::pair<int, std::string>(http_code, parse_error(response_string));
        }
    }
    return std::pair<int, std::string>(0, parse_ok(response_string));
}

std::pair<int, std::string> sfdc_client::delete_class(const std::string& class_id)
{
    if (curl_)
    {
        std::string endpoint = endpoint_ + "/" + class_id;

        curl_easy_setopt(curl_, CURLOPT_URL, endpoint.c_str());
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "DELETE");

        struct curl_slist *headers = nullptr;

        std::string auth_header = "Authorization: Bearer " + token_;
        std::cout << "endpoint: " << endpoint << "\n";
        std::cout << "auth_header: " << auth_header << "\n";

        headers = curl_slist_append(headers, auth_header.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

        CURLcode res = curl_easy_perform(curl_);

        long http_code = 0;
        curl_easy_getinfo (curl_, CURLINFO_RESPONSE_CODE, &http_code);
        std::cout << "status code: " << http_code << "\n";

        if ( http_code != 204 )
        {
            return std::pair<int, std::string>(http_code, "Generic Error");
        }

        if(res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
            exit(-1);
        }
        else
        {
            std::cout << http_code << "\n";
        }
    }
    return std::pair<int, std::string>(0, "");
}

size_t sfdc_client::write_fun(void *ptr, size_t size, size_t nmemb, std::string* data)
{
    std::cout << "*****\t\tnew chunk\n";
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

std::string sfdc_client::parse_error(const std::string& raw_response) const
{
    // TODO: implement this method using std::string_view
    size_t found = raw_response.find("message");
    if ( found != std::string::npos )
    {
        std::string partial = raw_response.substr(found+10);
        found = partial.find("\"");
        if ( found != std::string::npos )
        {
            return partial.substr(0, found);
        }
        return partial;
    }
    return "Generic Error";
}

std::string sfdc_client::parse_ok(const std::string& raw_response) const
{
    // TODO: implement this method with a regex
    return raw_response.substr(7,18);
}
