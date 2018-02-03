#ifndef OPGS16_SYSTEM_DEBUGS_HIERARCHY_TREE_H
#define OPGS16_SYSTEM_DEBUGS_HIERARCHY_TREE_H

#include <vector>
#include <string>

struct ObjectTree {
	std::string name{};
	std::vector<ObjectTree> children;
};

#endif /** OPGS16_SYSTEM_DEBUGS_HIERARCHY_TREE_H */