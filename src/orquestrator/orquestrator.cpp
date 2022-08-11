#include <unistd.h>
#include <iostream>
#include <string>
#include "orquestrator.h"
#include "../req_res_utils/req_res_utils.h"

int orquestrator::execute(int argc, char** argv)
{
    const auto[ok, is_login, is_get_all, username, password, login_url, resource_path] = parse_flags(argc, argv);

    if ( !ok ) 
    {
        return 1;
    }

    if ( is_login )
    {
        return this->update_login(login_url, username, password);
    }

    if ( is_get_all )
    {
        return this->get_all_resources(std::get<1>(resource_repo_.get_login_details()));
    }

    return this->upload_resource(resource_path);

    // const auto [code, body] = sfdc_client_.tooling_get("tooling/sobjects/ApexClass", "01p0800000EccHAAAZ");

    // if ( code != 0 )
    // {
    //     std::cerr << "ERROR: polling ContainerAsyncRequest " << body << "\n";
    //     return 1;
    // }

    // std::cout << "async_body: " << body << "\n";

    // return 0;
}

int orquestrator::update_login(const std::string& login_url, const std::string& username, const std::string& password)
{
    std::string full_login_url = login_url+"/services/Soap/c/55.0";

    const auto [code, url, token] = sfdc_client_.login(full_login_url, username, password);
    if ( code == 0 )
    {
        if ( resource_repo_.update_login_details(token, url) )
        {
            std::cout << "Login details updated correctly\n";
            return 1;
        }
    }
    std::cerr << "ERROR: updating login details\n";
    return 0;
}

int orquestrator::upload_resource(const std::string& resource_filepath)
{
    std::string orgid = std::get<1>(resource_repo_.get_login_details());
    std::string filename = get_filename_from_filepath(resource_filepath);
    std::string identifier = filename + orgid;

    req_res_utils r_reader(resource_filepath);
    std::string body = r_reader.parse_body();

    if ( resource_repo_.get_repo().contains(identifier) )
    {
        // update the resource
        const auto [metacontainer_code, metacontainer_id] = sfdc_client_.tooling_post("tooling/sobjects/MetadataContainer", 
                req_res_utils::metadata_container_body());

        if ( metacontainer_code != 0 )
        {
            std::cerr << "ERROR creating MetadataContainer: " << metacontainer_id << "\n";
            return 1;
        }

        auto updatable_resource = resource_repo_.get_repo().at(identifier);

        const auto [apex_code, apex_id] = sfdc_client_.tooling_post("tooling/sobjects/ApexClassMember", 
                req_res_utils::apexmember_body(updatable_resource.get_classid(), metacontainer_id, body));

        const auto [async_code, async_id] = sfdc_client_.tooling_post("tooling/sobjects/ContainerAsyncRequest", 
                req_res_utils::async_request_body(metacontainer_id));

        if ( async_code != 0 )
        {
            std::cerr << "ERROR: creating ContainerAsyncRequest " << async_id << "\n";
            return 1;
        }
        
        std::string response_body;
        std::string last_state;

        for (size_t i=0; i<20; ++i)
        {
            const auto [async_poll_code, async_body] = sfdc_client_.tooling_get("tooling/sobjects/ContainerAsyncRequest", async_id);

            if ( async_poll_code != 0 )
            {
                std::cerr << "ERROR: polling ContainerAsyncRequest " << async_body << "\n";
                return 1;
            }

            auto state = get_state_async_request(async_body);

            if ( state == "Completed" || state == "Failed" )
            {
                last_state = state;
                response_body = async_body;
                break;
            }
        }

        if ( last_state == "Failed" )
        {
            std::cerr << "ERROR: Updating resource ==> [errmsg: " << get_problem_async_request(response_body) << "]\n";
            return 1;
        }
        std::cout << "Resource update correctly ==> [async_id: " << async_id << "]  --  ";
        return 0;
    }
    else
    {
        const auto [code, message] = sfdc_client_.tooling_post("tooling/sobjects/ApexClass",
                 req_res_utils::insert_body(body));

        if ( code != 0 )
        {
            std::cerr << "ERROR: " << message << "\n";
            return 1;
        }

        resource new_resource(filename, message, orgid);
        if ( !resource_repo_.insert_and_write_to_file(identifier, new_resource) )
        {
            std::cerr << "ERROR: inserting in the repo\n";
            return 1;
        }

        std::cout << "Resource created correctly ==> [resource_id: " << message << "]  --  ";
        return 0;
    }
}

int orquestrator::get_all_resources(const std::string& orgid)
{
    const auto[code, body] = sfdc_client_.tooling_get("tooling/apexManifest", "");

    auto all_resources = get_all_resources_parser(body, orgid);

    for (const auto& res: all_resources)
    {
        std::string identifier = res.get_classname()+res.get_orgid();
        // if ( resource_repo_.get_repo().contains(identifier) )
        // {
        //     if ( res.get_classid() != resource_repo_.get_repo()[identifier].get_classid() )
        //     {
        //         std::cout << res.get_classname() << " overwrited\n";
        //     }
        // }
        resource_repo_.get_repo()[identifier] = res;
    }

    if ( resource_repo_.write_to_file() ) { 
        std::cout << resource_repo_.get_repo().size() << " entries has been added to the repository  --  ";
        return 0; 
    }

    std::cerr << "ERROR: writing repo to disk\n";
    return 1;
}

std::string orquestrator::get_filename_from_filepath(const std::string& filepath)
{
    auto found_slash = filepath.find_last_of('/');
    size_t begin = 0;

    if ( found_slash != std::string::npos )
    {
        begin = found_slash+1;
    }

    auto found_dot = filepath.find_last_of('.');

    if ( found_dot == std::string::npos ) { return ""; }

    return filepath.substr(begin, found_dot-begin);
}

std::string orquestrator::get_state_async_request(const std::string& body)
{
    auto found = body.find("State");
    if ( found == std::string::npos )
    {
        return "";
    }

    auto partial = body.substr(found+7);

    auto found_end = partial.find(',');
    if ( found_end == std::string::npos )
    {
        return "";
    }

    return partial.substr(1, found_end-2);
}

std::vector<resource> orquestrator::get_all_resources_parser(std::string_view body, const std::string& orgid)
{
    std::vector<resource> ret;
    const auto *id_begin = R"("id":)";
    const auto *name_begin = R"("name":)";
    auto partial = body;
    auto found_id = partial.find(id_begin);
    found_id++;

    int i = 0;
    while ( found_id != std::string_view::npos )
    {
        partial = partial.substr(found_id);
        auto found_end = partial.find(',');

        auto id_sv = partial.substr(0, found_end);
        std::string id = std::string(id_sv.substr(5, id_sv.length()-6));

        partial = partial.substr(found_end);

        found_id = partial.find(id_begin);

        auto found_name = partial.find(name_begin);
        partial = partial.substr(found_name);
        found_end = partial.find(',');
        auto name_sv = partial.substr(0, found_end);
        std::string name = std::string(name_sv.substr(8, name_sv.length()-9));

        ret.emplace_back(std::move(name), std::move(id), orgid);

        i++;
        // if ( i == 10 ) { break; }
    }

    return ret;
}

std::string orquestrator::get_problem_async_request(const std::string& body)
{
    auto found = body.find(R"(,"problem":)");
    if ( found == std::string::npos )
    {
        return "";
    }

    auto partial = body.substr(found+11);

    auto found_end = partial.find(',');
    if ( found_end == std::string::npos )
    {
        return "";
    }

    return partial.substr(1, found_end-2);
}

std::tuple<bool, bool, bool, std::string, std::string, std::string, std::string> orquestrator::parse_flags(int argc, char** argv)
{
    int l_flag = 0;
    int g_flag = 0;
    char* u_value = nullptr;
    char* p_value = nullptr;
    char* r_value = nullptr;
    char* f_value = nullptr;
    int c = 0;

    opterr = 0;

    while ((c = getopt (argc, argv, "glu:p:r:f:")) != -1) //NOLINT
    {
        switch (c)
        {
            case 'l':
                l_flag = 1;
                break;
            case 'g':
                g_flag = 1;
                break;
            case 'u':
                u_value = optarg;
                break;
            case 'p':
                p_value = optarg;
                break;
            case 'r':
                r_value = optarg;
                break;
            case 'f':
                f_value = optarg;
                break;
            case '?':
                if (optopt == 'u') 
                {
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt); //NOLINT
                }
                else { 
                    if (optopt == 'p') {
                        fprintf (stderr, "Option -%c requires an argument.\n", optopt); //NOLINT
                    }
                    else{ 
                        if (optopt == 'r')
                        {
                            fprintf (stderr, "Option -%c requires an argument.\n", optopt); //NOLINT
                        }
                        else
                        {
                            if (optopt == 'f')
                            {
                                fprintf (stderr, "Option -%c requires an argument.\n", optopt); //NOLINT
                            }
                            else
                            {
                                if (isprint (optopt) != 0)
                                {
                                    fprintf (stderr, "Unknown option `-%c'.\n", optopt); //NOLINT
                                }
                                else
                                {
                                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt); //NOLINT
                                }
                            }
                        }
                    }
                }
                return {false, false, false, "", "", "", ""};
            default:
                abort ();
        }
    }

    return {true, (l_flag==1), (g_flag==1),
        (u_value==nullptr)?"":std::string(u_value),
        (p_value==nullptr)?"":std::string(p_value),
        (r_value==nullptr)?"":std::string(r_value),
        (f_value==nullptr)?"":std::string(f_value)};
}

