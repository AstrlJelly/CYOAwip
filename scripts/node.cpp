#include "node.hpp"

Node::Node()
{
	this->actions = {};
}

Node::Node(std::initializer_list<Action*> actions)
{
	this->actions = actions;
}

void Node::addAction(Action* action)
{
	this->actions.push_back(action);
}

// returns name of next node
std::optional<std::string> Node::doActions()
{
	for (unsigned int i = 0; i < actions.size(); i++) {
		std::cout << "i : " << i << '\n';
		Action* currentAction = actions[i];
		std::optional<std::string> nextNodeName = currentAction->execute();
		if (nextNodeName.has_value()) {
			return nextNodeName.value();
		}
		// else just continue
	}
	return std::nullopt;
}