#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#include "action.hpp"

/*
.node format basic specifications

; gives a command, # is for comments

to do a command, do `;{command} {params}`
to comment, you can do `# comment` or `# comment #`
    to put a comment in the middle of a line

commands:
;text - displays text in the console at a pre-specified speed
;prompt - when empty, it will prompt for an enter(?) press.
        - when parameters are inputted, it will give the user an option,
          and do things depending on the choice
;go - moves to another node, the parameter must be the file name
    - i.e battle1.node would have to be `;go battle1`

when writing plain text, it will be interpreted as a `;text` command with a line break at the end
*/

// bitmasks for the parsing state machine
constexpr int LINE_ACTION_STATE  = 1 << 0;
constexpr int OTHER_ACTION_STATE = 1 << 1;
constexpr int COMMENT_STATE      = 1 << 2;
constexpr int UNUSED_STATE       = 1 << 3;

class Node
{
private:
    std::vector<Action*> actions;

public:
    Node();
    Node(std::initializer_list<Action*> actions);
    Node(std::string filePath);

    ~Node();

    static Node* fromFile(std::string filePath);

    void addAction(Action* action);
    std::optional<std::string> doActions();
};