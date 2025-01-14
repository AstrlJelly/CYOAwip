#pragma once

#include <vector>
#include "action.hpp"

class Node
{
private:
    std::vector<Action*> actions;

public:
    Node();
    Node(std::initializer_list<Action*> actions);

    void addAction(Action* action);
    std::optional<std::string> doActions();
};