#include <unistd.h>
#include <iostream>
#include <string>
#include "orquestrator.h"
#include "../body_creator/body_creator.h"

int orquestrator::execute(int argc, char** argv)
{
    const auto[ok, is_login, username, password, login_url, resource_path] = parse_flags(argc, argv);

    if ( !ok ) 
    {
        return 1;
    }

    if ( is_login )
    {
        this->update_login(login_url, username, password);
    }
    else
    {
        this->upload_resource(resource_path);
    }

    return 0;
}

void orquestrator::update_login(const std::string& login_url, const std::string& username, const std::string& password)
{
    std::string full_login_url = login_url+"/services/Soap/c/55.0";

    const auto [code, url, token] = sfdc_client_.login(full_login_url, username, password);
    if ( code == 0 )
    {
        if ( resource_repo_.update_login_details(token, url) )
        {
            std::cout << "Login details updated correctly\n";
            return;
        }
    }
    std::cerr << "ERROR: updating login details\n";
}

void orquestrator::upload_resource(const std::string& resource_filepath)
{
    std::string orgid = std::get<1>(resource_repo_.get_login_details());
    std::string filename = get_filename_from_filepath(resource_filepath);
    std::string identifier = filename + orgid;

    if ( resource_repo_.get_repo().contains(identifier) )
    {
        // update the resource
        const auto [metacontainer_code, metacontainer_id] = sfdc_client_.tooling_post("tooling/sobjects/MetadataContainer", 
                body_creator::metadata_container_body());

        if ( metacontainer_code != 0 )
        {
            std::cerr << "ERROR: " << metacontainer_id << "\n";
            return;
        }

        auto updatable_resource = resource_repo_.get_repo().at(identifier);

        std::cout << "message: " << metacontainer_id << "\n";

        const auto [apex_code, apex_id] = sfdc_client_.tooling_post("tooling/sobjects/ApexClassMember", 
                body_creator::apexmember_body());

        // resource_repo_.delete_from_repo(identifier);
    }
    else
    {
        std::cout << "javier => " << "kaka" << "\n";
    }



    // body_creator r_reader(resource_filepath);
    // const auto [code, message] = sfdc_client_.create_class(r_reader.create_body());

    // if ( code != 0 )
    // {
    //     std::cerr << "ERROR: " << message << "\n";
    //     return;
    // }

    // resource new_resource(filename, message, orgid);
    // if ( !resource_repo_.insert(identifier, new_resource) )
    // {
    //     std::cerr << "ERROR: inserting in the repo\n";
    //     return;
    // }

    // std::cout << "Resource uploaded correctly: [" << message << "]\n";
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

std::tuple<bool, bool, std::string, std::string, std::string, std::string> orquestrator::parse_flags(int argc, char** argv)
{
    int l_flag = 0;
    char* u_value = nullptr;
    char* p_value = nullptr;
    char* r_value = nullptr;
    char* f_value = nullptr;
    int c = 0;

    opterr = 0;

    while ((c = getopt (argc, argv, "lu:p:r:f:")) != -1) //NOLINT
    {
        switch (c)
        {
            case 'l':
                l_flag = 1;
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
                return {false, false, "", "", "", ""};
            default:
                abort ();
        }
    }

    return {true, (l_flag==1), 
        (u_value==nullptr)?"":std::string(u_value),
        (p_value==nullptr)?"":std::string(p_value),
        (r_value==nullptr)?"":std::string(r_value),
        (f_value==nullptr)?"":std::string(f_value)};
}
