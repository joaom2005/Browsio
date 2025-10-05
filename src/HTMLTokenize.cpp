#include "include/HTMLTokenize.hpp"
#include "include/logger.hpp"

#include <stack>

enum class State
{
    TEXT = 0,
    TAG = 1,
    ATTRIBUTE_NAME = 2,
    ATTRIBUTE_VALUE = 3
};

HTMLTokenize::HTMLTokenize(std::string &htmlCode)
{
    LOG_INFO("Starting Tokenization of the Website's HTML code");
    // Initialize the DOM node
    auto DOMNode = std::make_shared<Node>();
    DOMNode->name = "DOM";
    DOMNode->childrenNodes = std::vector<std::shared_ptr<Node>>();

    // Create a stack to save the open nodes.
    std::stack<std::shared_ptr<Node>> openNodes;
    openNodes.push(DOMNode);

    // Loop through the code
    char currentCharacter;              // Save the current character. The variable is created outside so that it doesn't need to be created in every iteration
    State state = State::TEXT;          // Current read state
    std::string buffer = std::string(); // String to save the current read value
    for (size_t i = 0; i < htmlCode.size(); i++)
    {
        currentCharacter = htmlCode[i];

        switch (state)
        {
        case State::TEXT:
            // When reading normal text, if a '<' is encountered, switch to tag mode and save the current buffer
            if (currentCharacter == '<')
            {
                buffer.clear();
                state = State::TAG;
            }
            // Else just add the current character to the buffer
            else
            {
                buffer += currentCharacter;
            }
            break;
        case State::TAG:
            // When reading for a tag, if a '>' is encountered, switch to text mode and save the current buffer
            if (currentCharacter == '>')
            {
                auto newNode = std::make_shared<Node>();
                newNode->name = buffer;
                newNode->parentNode = openNodes.top().get();

                openNodes.push(newNode);

                buffer.clear();
                state = State::TEXT;
            }
            // Else just add the current character to the buffer
            else
            {
                buffer += currentCharacter;
            }
            break;
        default:
            break;
        }
    }
}

HTMLTokenize::~HTMLTokenize()
{
}
