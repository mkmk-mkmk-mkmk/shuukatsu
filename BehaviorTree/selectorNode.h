#pragma once

//—Dæ“x‘I‘ðƒm[ƒh

#include "compositeNode.h"

class SelectorNode : public CompositeNode
{
public:
	virtual NodeStatus Tick() override
	{
		for (BehaviorNode* child : children)
		{
			NodeStatus status = child->Tick();
			if (status == NodeStatus::Success)
			{
				return NodeStatus::Success;
			}
			else if (status == NodeStatus::Running)
			{
				return NodeStatus::Running;
			}
		}
		return NodeStatus::Failure;
	}
};