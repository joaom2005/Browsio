#ifndef DOM_PARSER_TOKENIZER_H
#define DOM_PARSER_TOKENIZER_H

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

// The node/token representing an element
struct Node
{
    std::string name;                                        // Token name
    std::unordered_map<std::string, std::string> attributes; // Attributes like class or id
    std::vector<std::shared_ptr<Node>> childrenNodes;        // The children of this token
    Node *parentNode;                                        // The parent token that this one belongs to

    Node() = default;
};

// Class to use to create the DOM
class HTMLTokenize
{
public:
    HTMLTokenize(std::string &htmlCode);
    ~HTMLTokenize();
};

#endif // DOM_PARSER_TOKENIZER_H