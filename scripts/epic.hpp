#pragma once

#include <string>

#include "defines.hpp"
#include "node.hpp"

constexpr const char* EPICS_PATH = "./epics/";

constexpr const char* START_EPIC = "testing";

constexpr const char* NODES_PATH = "/nodes/";

/*
	epic specification

	story_name/
	|-- metadata.json
	|-- nodes/

	* metadata.json contains info about the story's display name, the author(s),
	  a description, revision number (stdver), date published, date revised

*/

struct Epic
{
	Epic(fs::path epicPath);

	nodeDict nodes;
};
