#pragma once

//ğŒƒm[ƒh

#include "BehaviorTree/behaviorNode.h"
#include <functional>

class ConditionNode : public BehaviorNode
{
public:
	using Func = std::function<bool()>;

	ConditionNode(Func fn) : fn(fn) {}

	virtual NodeStatus Tick() override
	{
		return fn() ? NodeStatus::Success : NodeStatus::Failure;
	}

private:
	Func fn;
};