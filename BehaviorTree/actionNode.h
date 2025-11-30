#pragma once

//アクションノード

#include "BehaviorTree/behaviorNode.h"
#include <functional>

class ActionNode : public BehaviorNode
{
public:
    using Func = std::function<NodeStatus()>;

	ActionNode(Func fn) : fn(fn) {}

    virtual NodeStatus Tick() override
    {
		return fn();
    }

private:
    Func fn;
};