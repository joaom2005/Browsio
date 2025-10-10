#include "include/HTMLTokenize.hpp"
#include "include/logger.hpp"

#include <stack>
#include <cctype>
#include <algorithm>

enum class State
{
    TEXT,
    TAG,
    ATTRIBUTE_NAME,
    ATTRIBUTE_VALUE
};

// Check if a string is empty
bool isNullOrWhitespace(const std::string &str)
{
    return str.empty() || std::all_of(str.begin(), str.end(), [](char c)
                                      { return std::isspace(static_cast<unsigned char>(c)); });
}

// Class initializer
HTMLTokenize::HTMLTokenize(std::string &htmlCode)
{
    this->htmlCode = htmlCode;
}

// Tokenize the HTML code
std::vector<Token> HTMLTokenize::Tokenize()
{
    // Vector with the tokens to return
    std::vector<Token> tokenList = std::vector<Token>();

    // Temp variables
    std::string buffer = std::string();       // String to save the current read value
    std::string atributeName = std::string(); // String to save the current atribute name
    Token tempToken;                          // Temporary token to save some info

    // Variables to loop through the html code
    char currentCharacter = '\0';  // Variable to store the current read character.
    char quoteChar = '\0';         // Variable used to check if we are inside a string
    State state = State::TEXT;     // Current read state
    bool isSelfClosingTag = false; // Whether the current tag is self-closing
    bool isClosingTag = false;     // Whether the current tag is of type closing

    // Helper lambda to create an tag
    auto emitTag = [&](Token::Type type)
    {
        tempToken.type = type;
        tempToken.name = buffer;
        tokenList.push_back(tempToken);
        tempToken = Token(); // reset
        buffer.clear();
        state = State::TEXT;
        isSelfClosingTag = false;
    };

    for (size_t i = 0; i < htmlCode.size(); i++)
    {
        // Save the current value to not have to read from the string more than once per iteration
        currentCharacter = htmlCode[i];

        // Handle how we read differently by the current state
        switch (state)
        {
        case State::TEXT:
            // When reading normal text, if a '<' is encountered, switch to tag mode and save the current buffer
            if (currentCharacter == '<')
            {
                if (i + 1 < htmlCode.size() && htmlCode[i + 1] == '/')
                {
                    isClosingTag = true;
                    i++;
                }
                else
                {
                    isClosingTag = false;
                }
                // If the buffer isn't empty, save that as text
                if (!isNullOrWhitespace(buffer))
                {
                    Token textToken = Token(Token::Type::TEXT, buffer);
                    tokenList.push_back(textToken);
                }

                buffer = "";
                state = State::TAG;
            }
            // Else just add the current character to the buffer
            else
            {
                buffer += currentCharacter;
            }
            break;
        case State::TAG:
            // If the character is an empty character, it means that the name ended now it's the atributes
            if (std::isspace(currentCharacter))
            {
                // If the string isn't empty save that
                if (!buffer.empty())
                {
                    // Save the node's name and empty the buffer
                    tempToken.name = buffer;
                    buffer = "";

                    state = State::ATTRIBUTE_NAME;
                }
                else
                {
                    state = State::ATTRIBUTE_NAME;
                }
            }
            // If the current character is /, enable self-closing
            else if (currentCharacter == '/')
            {
                isSelfClosingTag = true;
            }
            // Handle closing of the tag
            else if (currentCharacter == '>')
            {
                // Handle in case it's a closing tag
                if (isClosingTag)
                {
                    emitTag(Token::Type::TAG_CLOSE);
                }
                // Handle if a tag that ends with '/>' like <br/>
                else if (isSelfClosingTag)
                {
                    emitTag(Token::Type::SELF_CLOSE);
                }
                else
                // Handle if a tag that ends with '>' like <br>
                {
                    emitTag(Token::Type::TAG_OPEN);
                }
                buffer = "";
            }
            // Else just add the current character to the buffer
            else
            {
                buffer += currentCharacter;
            }
            break;
        case State::ATTRIBUTE_NAME:
            if (currentCharacter == '=')
            {
                atributeName = buffer;
                buffer = "";
                state = State::ATTRIBUTE_VALUE;
            }
            // If the current character is /, enable self-closing
            else if (currentCharacter == '/')
            {
                isSelfClosingTag = true;
            }
            // Handle closing of the tag
            else if (currentCharacter == '>')
            {
                tempToken.attributes[buffer] = "";
                // Handle if a tag that ends with '/>' like <br/>
                if (isSelfClosingTag)
                {
                    emitTag(Token::Type::SELF_CLOSE);
                }
                else
                // Handle if a tag that ends with '>' like <br>
                {
                    emitTag(Token::Type::TAG_OPEN);
                }

                buffer = "";
            }
            else
            {
                buffer += currentCharacter;
            }
            break;
        case State::ATTRIBUTE_VALUE:
            // If we're reading text, we should remember that incase there's a space
            if (quoteChar == '\0' && (currentCharacter == '\'' || currentCharacter == '\"'))
                quoteChar = currentCharacter;
            else if (currentCharacter == quoteChar)
                quoteChar = '\0';
            // If we encounter a space and we're not reading text (""/''), then the value ended
            else if ((std::isspace(currentCharacter) && quoteChar == '\0'))
            {
                tempToken.attributes[atributeName] = buffer;
                atributeName = "";
                buffer = "";
                state = State::TAG;
            }
            // If the current character is /, enable self-closing
            else if (currentCharacter == '/')
            {
                isSelfClosingTag = true;
            }
            // Handle closing of the tag
            else if (currentCharacter == '>')
            {
                tempToken.attributes[atributeName] = buffer;
                // Handle if a tag that ends with '/>' like <br/>
                if (isSelfClosingTag)
                {
                    emitTag(Token::Type::SELF_CLOSE);
                }
                else
                // Handle if a tag that ends with '>' like <br>
                {
                    emitTag(Token::Type::TAG_OPEN);
                }
                buffer = "";
            }
            else
            {
                buffer += currentCharacter;
            }
            break;
        default:
            break;
        }
    }

    // Check to see if there's anything left
    if (!isNullOrWhitespace(buffer))
        tokenList.push_back(Token(Token::Type::TEXT, buffer));

    return tokenList;
}