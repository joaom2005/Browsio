#include "include/connection.hpp"
#include "include/logger.hpp"
#include "include/HTMLParser.hpp"

int main() {
    // Initialize Logger
    LOG_INIT("", Logger::Level::Warn);
    LOG_INFO("App started!");

    LOG_INFO("This won't appear");

    std::string webSiteLink = std::string("https://motherfuckingwebsite.com");
    std::string htmlCode = Connection::GetWebsite(webSiteLink);

    HTMLTokenize Tokens = HTMLTokenize(htmlCode);
    Tokens.Tokenize();
}
