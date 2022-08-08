#ifndef SFDC_CLIENT_H
#define SFDC_CLIENT_H

#include <iostream>
#include <string>
#include <tuple>
#include <curl/curl.h>

class sfdc_client
{
    public:
        // No se puede pasar por valor y mover porque se pierde el valor para su uso más adelante
        sfdc_client(const std::string& endpoint, const std::string& token): // NOLINT
            endpoint_(endpoint),
            token_(token),
            curl_(curl_easy_init())
        {}

        explicit sfdc_client(const std::tuple<std::string, std::string>& login_tuple);

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

        std::pair<int, std::string> tooling_post(const std::string& path, const std::string& body);

        std::pair<int, std::string> delete_class(const std::string& class_id);

        std::tuple<int, std::string, std::string> login(const std::string& login_url, const std::string& username, const std::string& password);

    private:
        std::string endpoint_;
        std::string token_;
        CURL *curl_;

        static size_t write_fun(void *ptr, size_t size, size_t nmemb, std::string* data);

        static std::string parse_error(const std::string& raw_response);

        static std::string parse_ok(const std::string& raw_response);

        static std::tuple<int, std::string, std::string> parse_login(const std::string& raw_response);
};
#endif
