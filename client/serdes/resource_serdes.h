#ifndef RESOURCE_SERDES_REPO_H
#define RESOURCE_SERDES_REPO_H

#include "../resource_repo/resource_repo.h"

class resource_serdes
{
    public:
        static resource deserialize(const std::string& line);

        static std::string serialize(const resource& resource);
};
#endif
