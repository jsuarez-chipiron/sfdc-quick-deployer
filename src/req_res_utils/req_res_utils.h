#ifndef REQ_RES_UTILS_H
#define REQ_RES_UTILS_H

#include <iostream>
#include <string_view>

class req_res_utils
{
    public:
        explicit req_res_utils(std::string filepath): filepath_(std::move(filepath)) {}

        req_res_utils(const req_res_utils& rhs) = delete;

        req_res_utils(req_res_utils&& rhs) = default;

        req_res_utils& operator=(const req_res_utils& rhs) = delete;

        req_res_utils& operator=(req_res_utils&& rhs) = default;

        ~req_res_utils() = default;

        void print_repo() const;

        std::string get_filepath() const;

        std::string parse_body() const;

        static std::string insert_body(const std::string& body);

        static std::string metadata_container_body();

        static std::string apexmember_body(const std::string& content_entity_id, 
                const std::string& meta_container_id, const std::string& body);

        static std::string async_request_body(const std::string& meta_container_id);

        static std::string parse_class_response(std::string_view body);

    private:
        std::string filepath_;

};

#endif
