#include <iostream>
#include <cstdio>
#include <utility>
#include <curl/curl.h>

class sfdc_client
{
    private:
        std::string endpoint_;
        std::string token_;
        std::string body_;
        CURL *curl_;                                                      

        static size_t write_fun(void *ptr, size_t size, size_t nmemb, std::string* data)
        {
            std::cout << "*****\t\tnew chunk\n";                                
            data->append((char*) ptr, size * nmemb);                                
            return size * nmemb;                                
        }
        
    public:
        sfdc_client(std::string endpoint, std::string token, std::string body):
            endpoint_(std::move(endpoint)), token_(std::move(token)), body_(std::move(body)), curl_(curl_easy_init())
        {}

        ~sfdc_client()
        {
            curl_easy_cleanup(curl_);                                                             
        }

        void create_class()
        {
            if (curl_)
            {
                curl_easy_setopt(curl_, CURLOPT_URL, endpoint_.c_str());                                
                curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "POST");
                curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, body_.c_str());

                struct curl_slist *headers = nullptr;

                std::string auth_header = "Authorization: Bearer " + token_;
                std::cout << auth_header << std::endl;

                headers = curl_slist_append(headers, auth_header.c_str());
                headers = curl_slist_append(headers, "Content-Type: application/json");

                curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

                std::string response_string;

                curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, body_.size());                                
                curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_fun);
                curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response_string);

                CURLcode res = curl_easy_perform(curl_);

                if(res != CURLE_OK)
                {
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                }
                else
                {
                    std::cout << "response: " << response_string << "\n";
                }
            }
        }
};

int main()
{                                
    std::string endpoint = "https://xxxxxxxx.my.salesforce.com/services/data/v55.0/tooling/sobjects/ApexClass";
    std::string token = "xxxxxxxx";
    std::string body = R"({ "Body": "public class MySuperCoolTest{\n\tpublic String sayHi() {\n\t\treturn 'hello';\n\t}\n}"})";

    sfdc_client client(endpoint, token, body);
    client.create_class();

    return 0;                                                          
}
