#include "../include/connection.hpp"
#include "../include/logger.hpp"

int main() {
    // Initialize Logger
    LOG_INIT("", Logger::Level::Info);

    LOG_INFO("App started!");
    LOG_WARN("This is a warning, value = {}", 42);
    LOG_ERROR("An error occurred!");

    SET_LOG_LEVEL(Logger::Level::Warn); // only Warn and Error will log now
    LOG_INFO("This won't appear");

    std::string webSiteLink = std::string("https://google.com");
    Connection::GetWebsite(webSiteLink.c_str());
}
