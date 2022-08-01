// #include <string>
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
                    std::cout << "what the fuck\n";
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
    return "implement me!!";
}
