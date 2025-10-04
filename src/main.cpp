#include "../include/connection.hpp"
#include "../include/logger.hpp"

int main() {
    // Initialize Logger
    LOG_INIT("", Logger::Level::Info);
    LOG_INFO("App started!");

    LOG_INFO("This won't appear");

    std::string webSiteLink = std::string("https://motherfuckingwebsite.com");
    Connection::GetWebsite(webSiteLink);
}
