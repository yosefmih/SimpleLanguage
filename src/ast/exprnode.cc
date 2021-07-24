#include "exprnode.h"

#include <assert.h>

#include "../commons/common.h"

ExprNode::ExprNodeType ExprNode::getType() {
    return type;
}

void ExprNode::print(uint32_t depth, std::ostream& printTo) {
    printTo << TAB_SPACES;
}



