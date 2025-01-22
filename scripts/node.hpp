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

IMPORTANT NOTE:
do not name nodes `null.node`! `null` is used to refer to a nonexistant node

all commands:
;text - displays text in the console at a pre-specified speed
      - when writing plain text, it will be interpreted 
        as a `;text` command with a line break at the end

;prompt - when empty, it will prompt for an enter(?) press.
        - when parameters are inputted, it will give the user an option,
          and do things depending on the choice

wip ;playsfx - plays a sound effect, the parameter must be the sfx path

;go - moves to another node, the parameter must be the file name
    - i.e battle1.node would have to be `;go battle1`

*/


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