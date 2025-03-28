#include "epic.hpp"

Epic::Epic(fs::path epicPath)
{
    // recursively go through every file in the nodes file
    // assigns nodes like `forest/journey1` instead of just `journey1`

    if(!fs::exists(epicPath))
    {
        throwlog("Path \"" + epicPath.string() + "\" did not exist.");
    }

    this->nodes = {};

    // TODO: load nodes based on when other nodes need them, not just every one in the folder
    //       how will i do this in a clean fashion? no clue. lmao
    for(const fs::directory_entry& entry : fs::recursive_directory_iterator(epicPath))
    {
        fs::path filePath = entry.path();

        // little file type check ^_^ yay
        std::string extension = filePath.extension().string();
        if(extension != ".node" && extension != ".nod")
        {
            continue;
        }

        std::string nodeName = Node::nameFromFilePath(filePath,epicPath.string());

        Node* node = new Node(filePath.string());

        this->nodes.insert({nodeName,node});
    }

    // should never happen in a finished product
    if(nodes.empty())
    {
        throwlog("nodes was empty! oopsies ^.^");
    }
}
