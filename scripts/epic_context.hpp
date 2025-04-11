#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include "defines.hpp"

//typedef std::string epstr;
//typedef bool epbool;
//typedef long int epint;
//typedef double epfloat;
typedef boost::variant<long,bool,std::string> epicVariable;
typedef std::unordered_map<std::string,epicVariable> epicVariableMap;

struct EpicContext
{
	EpicContext(fs::path epicPath, std::string startNode);
	~EpicContext();

	void saveCurrentNode();
	static std::string loadSavedNode();

	void moveNode(std::string newNodeName);

	void setGlobalVariable(std::string varName,epicVariable value);
	void setNodeVariable(std::string varName,epicVariable value);

	std::string currentNodeName;

	#define ADD_VAR(varName, value) {varName,epicVariable(value)}

	const epicVariableMap defaultGlobalVariables = {
	};
	const epicVariableMap defaultNodeVariables = {
		ADD_VAR("textdelay",defaultTextDelay)
	};

	#undef ADD_VAR

	epicVariableMap globalVariables;
	epicVariableMap nodeVariables;
};