#ifndef SFDC_CLIENT_H
#define SFDC_CLIENT_H

#include <iostream>
#include <string>
#include <curl/curl.h>

class sfdc_client
{
    public:
        sfdc_client(std::string endpoint, std::string token):
            endpoint_(std::move(endpoint)), 
            token_(std::move(token)), 
            curl_(curl_easy_init())
        {}

        sfdc_client(const sfdc_client& rhs) = delete;

        sfdc_client(sfdc_client&& rhs) noexcept : 
            endpoint_(std::move(rhs.endpoint_)), 
            token_(std::move(rhs.token_)), 
            curl_(rhs.curl_)
        {
            rhs.curl_ = nullptr;
        }
            
        sfdc_client& operator=(const sfdc_client& rhs) = delete;

        sfdc_client& operator=(sfdc_client&& rhs) noexcept                                                                                                                                  
        {
            endpoint_ = std::move(rhs.endpoint_);
            token_ = std::move(rhs.token_);
            curl_ = rhs.curl_;
            rhs.curl_ = nullptr;
            return *this;
        }

        ~sfdc_client();

        std::pair<int, std::string> create_class(const std::string body);

        std::pair<int, std::string> delete_class(const std::string& class_id);

    private:
        std::string endpoint_;
        std::string token_;
        CURL *curl_;

        static size_t write_fun(void *ptr, size_t size, size_t nmemb, std::string* data);

        std::string parse_error(const std::string& raw_response) const;

        std::string parse_ok(const std::string& raw_response) const;
};
#endif
