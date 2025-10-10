#ifndef DOM_PARSER_TOKENIZER_H
#define DOM_PARSER_TOKENIZER_H

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

// Token representation
struct Token
{
    enum class Type
    {
        TAG_OPEN,
        TAG_CLOSE,
        TEXT,
        ATTRIBUTE,
        SELF_CLOSE
    };
    Type type;
    std::string name;
    std::unordered_map<std::string, std::string> attributes;

    inline Token(Type _type, std::string &_name, std::unordered_map<std::string, std::string> _attributes) : type(_type), name(_name), attributes(_attributes) {};
    inline Token(Type _type, std::string &_name) : type(_type), name(_name) {};
    Token() = default;
};

// Class to use to create the DOM
class HTMLTokenize
{
public:
    HTMLTokenize(std::string &htmlCode);

    std::vector<Token> Tokenize();

private:
    std::string htmlCode;
};

#endif // DOM_PARSER_TOKENIZER_H