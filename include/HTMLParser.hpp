#ifndef DOM_PARSER_TOKENIZER_H
#define DOM_PARSER_TOKENIZER_H

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

// Token representation
struct Token
{
    // Class to hold the type of token
    enum class Type
    {
        TAG_OPEN,
        TAG_CLOSE,
        TEXT,
        ATTRIBUTE,
        SELF_CLOSE
    };
    // Token data
    Type type;
    std::string name;
    std::unordered_map<std::string, std::string> attributes;

    // Constructors
    Token() = default;
    inline Token(Type _type, std::string &_name, std::unordered_map<std::string, std::string> _attributes) : type(_type), name(_name), attributes(_attributes) {};
    inline Token(Type _type, std::string &_name) : type(_type), name(_name) {};
};

// Class to use to create the DOM
class HTMLTokenize
{
public:
    /// @brief After creating the object, use Tokenize() to get the tokens and ParseTokens() to create the DOM
    /// @param htmlCode The HTML source code to parse
    HTMLTokenize(std::string &htmlCode);

    /// @brief Tokenize the HTML code into a vector of tokens
    void Tokenize();
    /// @brief Parse the tokens into a DOM structure
    void ParseTokens();

private:
    // The HTML code to parse
    std::string htmlCode;
    // The list of tokens generated
    std::vector<Token> tokens;
};

#endif // DOM_PARSER_TOKENIZER_H