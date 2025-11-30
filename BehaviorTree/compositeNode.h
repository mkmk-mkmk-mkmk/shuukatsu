#pragma once

//コンポジットノードの基底クラス

#include "BehaviorTree/behaviorNode.h"
#include <vector>

class CompositeNode : public BehaviorNode
{
public:
    void AddChild(BehaviorNode* child)
    {
        children.push_back(child);
	}

protected:
    std::vector<BehaviorNode*> children;
};