#ifndef BROWSIO_CONNECTION_H
#define BROWSIO_CONNECTION_H

#include <string>

class Connection{
    public:
    static std::string GetWebsite(const std::string& link);
};

#endif // BROWSIO_CONNECTION_H