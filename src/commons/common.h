#ifndef COMMON_H
#define COMMON_H

#include <vector>

#include "defs.h"

void toLower(string str);

// indents each line of str by depth times of 4 spaces (defined in defs.h)
string addIndent(string str, uint32_t depth);

string indentationAtDepth(uint32_t depth);


// template <typename T, typename A>
// void deleteVectorMembers(std::vector<ASTNode*>& vec); 

uint32_t stringToInt(string str); 

#endif