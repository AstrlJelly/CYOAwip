#pragma once

#include <string>

#include "defines.hpp"
#include "node.hpp"

constexpr const char* EPICS_PATH = "./epics/";

// will eventually be chosen in a menu
constexpr const char* START_EPIC = "testing";

constexpr const char* NODES_PATH = "/nodes/";

/*
	epic specification

	story_name\
	|-- metadata.json
	|-- nodes\
	|---- *.node\nde

	* metadata.json contains info about the story's display name, the author(s),
	  a description, revision number (stdver), date published, date revised
	* nodes may be in subfolders, and will internally be stored as
	  filename, subfolder1/filename, or subfolder1/subfolder2/filename

*/

struct Epic
{
	Epic(fs::path epicPath, std::string startNode);
	~Epic();

	Node* getCurrentNode();
	Node* getCurrentNode(EpicContext* ctx);
	bool executeCurrentNode();
	bool executeCurrentNode(EpicContext* ctx);

	nodeDict nodes;
	EpicContext* ctx;
};
