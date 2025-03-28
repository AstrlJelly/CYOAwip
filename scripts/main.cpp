#include "main.hpp"

int main(int argc, char* argv[])
{
    std::string beginNode{};
    std::stringstream beginNodeStream{};
    std::ifstream saveFile(SAVE_PATH);
    beginNodeStream << saveFile.rdbuf();
    beginNode = beginNodeStream.str();

    // if save file didn't exist, check cli args
    // if not cli args, get default beginning node
    if (beginNode.length() <= 0) {
        beginNode = argc > 1 ? std::string(argv[1]) : BEGIN_NODE;
    }

    fs::path epicPath = fs::path(EPICS_PATH);
    epicPath += START_EPIC;
    epicPath += NODES_PATH;

    EpicContext* ctx = new EpicContext(epicPath, beginNode);

    mainLoop(ctx);

    // no need for it to be async... yet.
    //std::async(std::launch::async, mainLoop, nodes);

    //endwin();
    return 0;
}

void mainLoop(EpicContext* ctx)
{
    bool keepRunning = true;


    while (keepRunning)
    {
        ctx->executeCurrentNode();
    }
}