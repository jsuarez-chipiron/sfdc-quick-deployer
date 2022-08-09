#ifndef BODY_CREATOR_H
#define BODY_CREATOR_H

#include <iostream>

class body_creator
{
    public:
        explicit body_creator(std::string filepath): filepath_(std::move(filepath)) {}

        body_creator(const body_creator& rhs) = delete;

        body_creator(body_creator&& rhs) = default;

        body_creator& operator=(const body_creator& rhs) = delete;

        body_creator& operator=(body_creator&& rhs) = default;

        ~body_creator() = default;

        void print_repo() const;

        std::string get_filepath() const;

        std::string parse_body() const;

        static std::string insert_body(const std::string& body);

        static std::string metadata_container_body();

        static std::string apexmember_body(const std::string& content_entity_id, 
                const std::string& meta_container_id, const std::string& body);

        static std::string async_request_body(const std::string& meta_container_id);

    private:
        std::string filepath_;

};

#endif
