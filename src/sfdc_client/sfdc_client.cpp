#include <iostream>
#include <string>
#include <curl/curl.h>
#include "sfdc_client.h"

sfdc_client::sfdc_client(const std::tuple<std::string, std::string>& login_tuple):
    sfdc_client(std::get<1>(login_tuple), std::get<0>(login_tuple))
{}

sfdc_client::~sfdc_client()
{
    if (curl_ != nullptr)
    {
        curl_easy_cleanup(curl_);
    }
}

std::pair<int, std::string> sfdc_client::tooling_post(const std::string& path, const std::string& body)
{
    std::string response_string;
    if (curl_ != nullptr)
    {
        // curl_ = curl_easy_init(); // reinit(endpoint_+path).c_str() the curl object (fix bad_allow bug)

        curl_easy_setopt(curl_, CURLOPT_URL, (endpoint_+path).c_str()); //NOLINT
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "POST"); //NOLINT
        curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, body.c_str()); //NOLINT

        struct curl_slist *headers = nullptr;

        std::string auth_header = "Authorization: Bearer " + token_;

        headers = curl_slist_append(headers, auth_header.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers); //NOLINT

        curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, body.size()); //NOLINT
        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_fun); //NOLINT
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response_string); //NOLINT

        CURLcode res = curl_easy_perform(curl_);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << "\n";
            return {1000, "Generic Error"};
        }

        long http_code = 0;
        curl_easy_getinfo (curl_, CURLINFO_RESPONSE_CODE, &http_code); //NOLINT

        if ( http_code != 201 )
        {
            return {http_code, parse_error(response_string)};
        }
    }
    return {0, parse_ok(response_string)};
}

std::pair<int, std::string> sfdc_client::tooling_get(const std::string& path, const std::string& resource_id)
{
    std::string response_string;
    if (curl_ != nullptr)
    {
        // curl_ = curl_easy_init(); // reinit(endpoint_+path).c_str() the curl object (fix bad_allow bug)

        curl_easy_setopt(curl_, CURLOPT_URL, (endpoint_+path+"/"+resource_id).c_str()); //NOLINT
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "GET"); //NOLINT
        // curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, body.c_str()); //NOLINT

        struct curl_slist *headers = nullptr;

        std::string auth_header = "Authorization: Bearer " + token_;

        headers = curl_slist_append(headers, auth_header.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers); //NOLINT

        // curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, body.size()); //NOLINT
        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_fun); //NOLINT
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response_string); //NOLINT

        CURLcode res = curl_easy_perform(curl_);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << "\n";
            return {1000, "Generic Error"};
        }

        long http_code = 0;

        curl_easy_getinfo (curl_, CURLINFO_RESPONSE_CODE, &http_code); //NOLINT

        if ( http_code != 200 )
        {
            return {http_code, parse_error(response_string)};
        }
    }
    return {0, response_string};
}

std::pair<int, std::string> sfdc_client::delete_class(const std::string& class_id)
{
    if (curl_ != nullptr)
    {
        // curl_ = curl_easy_init(); // reinit the curl object (fix bad_allow bug)
        std::string endpoint = endpoint_ + "/" + class_id;

        curl_easy_setopt(curl_, CURLOPT_URL, endpoint.c_str()); //NOLINT
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "DELETE"); //NOLINT

        struct curl_slist *headers = nullptr;

        std::string auth_header = "Authorization: Bearer " + token_;

        headers = curl_slist_append(headers, auth_header.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers); //NOLINT

        CURLcode res = curl_easy_perform(curl_);

        long http_code = 0;
        curl_easy_getinfo (curl_, CURLINFO_RESPONSE_CODE, &http_code); //NOLINT

        if ( http_code != 204 )
        {
            return {http_code, "Generic Error"};
        }

        if(res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
            return {1000, "Generic Error"};
        }
    }
    return {0, ""};
}

std::tuple<int, std::string, std::string> sfdc_client::login(const std::string& login_url, const std::string& username, const std::string& password)
{
    std::string response_string;
    if (curl_ != nullptr)
    {
        // curl_ = curl_easy_init(); // reinit the curl object (fix bad_allow bug)

        std::string part1 = R"(<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:urn="urn:partner.soap.sforce.com"><soapenv:Body><urn:login><urn:username>)";
        std::string part2 = R"(</urn:username><urn:password>)";
        std::string part3 = R"(</urn:password></urn:login></soapenv:Body></soapenv:Envelope>)";

        std::string body = part1 + username + part2 + password + part3;

        curl_easy_setopt(curl_, CURLOPT_URL, login_url.c_str());   // NOLINT
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "POST");    // NOLINT
        curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, body.c_str()); // NOLINT

        struct curl_slist *headers = nullptr;

        headers = curl_slist_append(headers, "SOAPAction: ''");
        headers = curl_slist_append(headers, "Content-Type: text/xml;charset=UTF-8");

        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers); // NOLINT

        curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, body.size());  // NOLINT
        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_fun);    // NOLINT
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response_string); // NOLINT

        CURLcode res = curl_easy_perform(curl_);

        long http_code = 0;
        curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &http_code); // NOLINT

        if ( http_code != 200 )
        {
            return {http_code, "", ""};
        }

        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
            return {1000, "", ""};
        }
    }
    return parse_login(response_string);
}

size_t sfdc_client::write_fun(void *ptr, size_t size, size_t nmemb, std::string* data)
{
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

std::string sfdc_client::parse_error(const std::string& raw_response)
{
    // TODO: implement this method using std::string_view
    size_t found = raw_response.find("message");
    if ( found != std::string::npos )
    {
        std::string partial = raw_response.substr(found+10);
        found = partial.find('\"');
        if ( found != std::string::npos )
        {
            return partial.substr(0, found);
        }
        return partial;
    }
    return "Generic Error";
}

std::string sfdc_client::parse_ok(const std::string& raw_response)
{
    // TODO: implement this method with a regex
    return raw_response.substr(7,18);
}

std::tuple<int, std::string, std::string> sfdc_client::parse_login(const std::string& raw_response)
{
    std::string url;
    std::string token;

    auto begin = raw_response.find("<serverUrl>");
    auto end = raw_response.find("</serverUrl>");
    if ( begin != std::string::npos )
    {
        url = raw_response.substr(begin + 11, (end-begin-11));
    }

    begin = raw_response.find("<sessionId>");
    end = raw_response.find("</sessionId>");
    if ( begin != std::string::npos )
    {
        token = raw_response.substr(begin + 11, (end-begin-11));
    }
    return {0, url, token};
}
