#include "resource_serdes.h"

resource resource_serdes::deserialize(const std::string& line)
{
    int nfield = 0;
    std::string buff;

    std::string classname;
    std::string classid;
    std::string orgid;

    for (const auto& c: line)
    {
        if ( c != ',' )
        {
            buff += c;
        }
        else
        {
            ++nfield;
            switch (nfield)
            {
                case 1:
                    classname = buff;
                    break;
                case 2:
                    classid = buff;
                    break;
                case 3:
                    orgid = buff;
                    break;
                default:
                    return resource("error_deserializing", "", "");
            }
            buff = "";
        }
    }

    if ( nfield != 3 )
    {
        return resource("error_deserializing", "", "");
    }

    return resource(classname, classid, orgid);
}

std::string resource_serdes::serialize(const resource& resource)
{
    std::string ret = resource.get_classname()+","+resource.get_classid()+","+resource.get_orgid()+",";
    return ret;
}
