#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#include "action.hpp"

constexpr auto NODES_PATH = "./nodes/";

/*
    .node format basic specifications

    ; gives a command, # is for comments

    to do a command, do `;{command} {params}`

    to comment, you can do `# comment` for a single line
    or `# comment #` to put a comment in the middle of a line

    IMPORTANT NOTE:
    do not name nodes `null.node`! `null` is used to refer to a nonexistant node

    all commands:
    ;text {text} - displays text in the console at a pre-specified speed
          - when writing plain text, it will be interpreted 
            as a `;text` command with a line break at the end

    ;pause {milliseconds} - pauses for a set number of milliseconds

    ;set - set custom variables
         - default node variables: textspeed

    wip ;playsfx - plays a sound effect, the parameter must be the sfx path

    ;prompt - when empty, it will prompt for an enter(?) press.
            - when parameters are inputted, it will give the user an option,
              and do things depending on the choice

    ;go - moves to another node, the parameter must be the file name
        - i.e battle1.node would have to be `;go battle1`

*/

// TODO: classes deriving from Node so that you can have more specialized behaviour?
//       (i.e BattleNode or StealthNode)

#define SET_DEFAULT_ROOT_IF_NEEDED if (rootPath.length() <= 0) rootPath = defaultRootPath

class Node
{
private:
    std::vector<Action*> actions;

    void addAction(Action* action);
public:
    Node();
    Node(std::initializer_list<Action*> actions);
    Node(std::string filePath);

    ~Node();

    static Node* fromFile(std::string filePath);

    static const char* defaultRootPath;
    static std::string nameFromFilePath(std::filesystem::path filePath, std::string rootPath = "");
    static std::filesystem::path filePathFromName(std::string name, std::string rootPath = "");

    Action* getActionAtIndex(size_t index);
    size_t getActionsSize();
};

//template <typename T>
//class NodeVariable
//{
//private:
//    T value;
//
//public:
//    NodeVariable<T>(T value);
//
//    NodeVariable<T> fromString(std::string valueStr);
//
//    T getValue();
//};

typedef std::unordered_map<std::string, Node*> nodeDict;
