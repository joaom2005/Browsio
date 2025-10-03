#include "../include/connection.hpp"
#include "../include/logger.hpp"

int main() {
    Logger::Init(); // initialize logger

    Logger::Info("App started!");
    Logger::Warn("This is a warning, value = {}", 42);
    Logger::Error("An error occurred!");

    Logger::SetLevel(Logger::Level::Warn); // only Warn and Error will log now
    Logger::Info("This won't appear");

    std::string webSiteLink = std::string("https://google.com");
    Connection::GetWebsite(webSiteLink.c_str());
}
