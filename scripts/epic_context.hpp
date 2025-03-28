#pragma once

#include <string>
#include <unordered_map>

#include "defines.hpp"
//#include "node.hpp"
class Epic;
class Node;

typedef std::unordered_map<std::string,Node*> nodeDict;

struct EpicContext
{
	EpicContext(fs::path epicPath, std::string startNode);
	~EpicContext();

	//operator ActionExecuteData();

	bool executeCurrentNode();
	void saveSpot();
	void moveNode(std::string newNodeName);

	Node* getCurrentNode();

	void setGlobalVariable(std::string varName,nodeVariable value);
	void setNodeVariable(std::string varName,nodeVariable value);

	std::string currentNodeName;

	#define ADD_VAR(varName, value) {varName,nodeVariable(value)}

	const nodeVariableMap defaultGlobalVariables = {
	};
	const nodeVariableMap defaultNodeVariables = {
		ADD_VAR("textdelay",defaultTextDelay)
	};
	nodeVariableMap globalVariables;
	nodeVariableMap nodeVariables;

	Epic* epic;
};