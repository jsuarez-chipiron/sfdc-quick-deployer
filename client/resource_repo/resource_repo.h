#ifndef RESOURCE_REPO_H
#define RESOURCE_REPO_H

#include <iostream>
#include <unordered_map>

class resource;

class resource_repo
{
    public:
        explicit resource_repo(const std::string& config_filename);

        resource_repo(const resource_repo& rhs) = delete;

        resource_repo(resource_repo&& rhs) noexcept: 
            repo_(std::move(rhs.repo_)),
            config_filename_(std::move(rhs.config_filename_))
        {}

        resource_repo& operator=(const resource_repo& rhs) = delete;

        resource_repo& operator=(resource_repo&& rhs) noexcept                                                                                                                                  
        {
            repo_ = std::move(rhs.repo_);
            config_filename_ = std::move(rhs.config_filename_);
            return *this;
        }

        ~resource_repo() {}

    private:
        std::unordered_map<std::string, resource> repo_;
        std::string config_filename_;

};

// classname, classid, orgid
class resource
{
    public:
        resource() {}

        resource(const resource& rhs) = delete;

        resource(resource&& rhs) noexcept
        {}

        resource& operator=(const resource& rhs) = delete;

        resource& operator=(resource&& rhs) noexcept                                                                                                                                  
        {
            return *this;
        }

        ~resource() {}

};
#endif
