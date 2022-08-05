#ifndef RESOURCE_REPO_H
#define RESOURCE_REPO_H

#include <iostream>
#include <unordered_map>
#include <tuple>

class resource
{
    public:
        resource() = default;

        explicit resource(std::string  classname, std::string classid, std::string orgid): 
            classname_(std::move(classname)),
            classid_(std::move(classid)),
            orgid_(std::move(orgid)) 
        {}

        // explicit resource(const std::string& classname, const std::string& classid, const std::string& orgid): 
        //     classname_(classname),
        //     classid_(classid),
        //     orgid_(orgid) 
        // {}

        resource(const resource& rhs) = default;

        resource(resource&& rhs) noexcept = default;

        resource& operator=(const resource& rhs) = default;

        resource& operator=(resource&& rhs) noexcept                                                                                                                                  
        {
            return *this;
        }

        ~resource() = default;

        std::string get_classname() const;

        std::string get_classid() const;

        std::string get_orgid() const;

    private:
        std::string classname_;
        std::string classid_;
        std::string orgid_;
};

class resource_repo
{
    public:
        explicit resource_repo(const std::string& filepath);

        resource_repo(const resource_repo& rhs) = delete;

        resource_repo(resource_repo&& rhs) noexcept: 
            repo_(std::move(rhs.repo_)),
            filepath_(std::move(rhs.filepath_))
        {}

        resource_repo& operator=(const resource_repo& rhs) = delete;

        resource_repo& operator=(resource_repo&& rhs) noexcept                                                                                                                                  
        {
            repo_ = std::move(rhs.repo_);
            filepath_ = std::move(rhs.filepath_);
            return *this;
        }

        ~resource_repo() = default;

        std::unordered_map<std::string, resource> get_repo() const;

        bool write_to_file() const;

        bool insert(const std::string& identifier, const resource& res);

        std::tuple<std::string, std::string> read_login_details() const;

        std::tuple<std::string, std::string> get_login_details() const;

        void print_repo() const;

    private:
        std::unordered_map<std::string, resource> repo_;
        std::string filepath_;
        std::tuple<std::string, std::string> login_details_;
};

#endif
