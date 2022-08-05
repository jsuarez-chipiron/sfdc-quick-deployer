#include <iostream>

class resource_reader
{
    public:
        explicit resource_reader(std::string filepath): filepath_(std::move(filepath)) {}

        resource_reader(const resource_reader& rhs) = delete;

        resource_reader(resource_reader&& rhs) = default;

        resource_reader& operator=(const resource_reader& rhs) = delete;

        resource_reader& operator=(resource_reader&& rhs) = default;

        ~resource_reader() = default;

        void print_repo() const;

        std::string get_filepath() const;

        std::string create_body() const;

    private:
        std::string filepath_;

};

