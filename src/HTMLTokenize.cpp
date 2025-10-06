#include "include/HTMLTokenize.hpp"
#include "include/logger.hpp"

#include <stack>
#include <cctype>

enum class State
{
    TEXT = 0,
    TAG,
    ATTRIBUTE_NAME,
    ATTRIBUTE_VALUE
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

    // Temporary values
    std::string buffer = std::string();       // String to save the current read value
    std::string atributeName = std::string(); // String to save the current atribute name
    Node tempNode = Node();                   // Temporary node

    // Loop through the code - this is a state machine, where depenending on the curent state, the text will be read differently
    bool readingText = false;  // Store whether we are currently inside a string (eg. ""; '')
    char currentCharacter;     // Save the current character. The variable is created outside so that it doesn't need to be created in every iteration
    State state = State::TEXT; // Current read state
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
                    tempNode.name = buffer;
                    buffer = "";

                    state = State::ATTRIBUTE_NAME;
                }
                else
                {
                    state = State::ATTRIBUTE_NAME;
                }
            }
            // TODO Handle what happens if we reach the end here
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
            // TODO Handle end of tag
            else
            {
                buffer += currentCharacter;
            }
            break;
        case State::ATTRIBUTE_VALUE:
            // If we're reading text, we should remember that incase there's a space
            if ((currentCharacter == '\'' || currentCharacter == '\"') && !buffer.empty())
            {
                readingText = !readingText;
            }
            // If we encounter a space and we're not reading text (""/''), then the value ended
            else if ((std::isspace(currentCharacter) && !readingText))
            {
                tempNode.attributes[atributeName] = buffer;
                atributeName = "";
                buffer = "";
                state = State::TAG;
            }
            else
            {
                buffer += currentCharacter;
            }
        default:
            break;
        }
    }
}

HTMLTokenize::~HTMLTokenize()
{
}
