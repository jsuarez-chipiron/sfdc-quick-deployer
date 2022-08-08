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

        resource& operator=(resource&& rhs) noexcept = default;

        ~resource() = default;

        std::string get_classname() const;

        std::string get_classid() const;

        std::string get_orgid() const;

        void set_classid(const std::string& classid);

        void set_orgid(const std::string& orgid);

    private:
        std::string classname_;
        std::string classid_;
        std::string orgid_;
};

class resource_repo
{
    public:
        explicit resource_repo(const std::string& filepath);

        resource_repo(const resource_repo& rhs) = default;

        resource_repo(resource_repo&& rhs) noexcept = default;
        // resource_repo(resource_repo&& rhs) noexcept:
        //     repo_(std::move(repo_)),
        //     filepath_(std::move(filepath_)),
        //     login_details_(std::move(login_details_))
        // {}

        resource_repo& operator=(const resource_repo& rhs) = delete;

        resource_repo& operator=(resource_repo&& rhs) noexcept = default;

        ~resource_repo() = default;

        std::unordered_map<std::string, resource> get_repo() const;

        bool update_login_details(const std::string& token, const std::string& url);

        bool delete_from_repo(const std::string& identifier);

        bool insert(const std::string& identifier, const resource& res);

        std::tuple<std::string, std::string> get_login_details() const;

        void print_repo() const;

    private:
        std::unordered_map<std::string, resource> repo_;
        std::string filepath_;
        std::tuple<std::string, std::string> login_details_;

        std::tuple<std::string, std::string> read_login_details() const;

        bool write_to_file() const;

        static std::string convert_to_tooling_url(const std::string& url);
};

#endif
