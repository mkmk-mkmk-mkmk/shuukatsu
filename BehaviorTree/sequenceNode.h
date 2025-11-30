#pragma once

//‡ŽŸŽÀsƒm[ƒh

#include "compositeNode.h"

class SequenceNode : public CompositeNode
{
public:
    virtual NodeStatus Tick() override
    {
        for (BehaviorNode* child : children)
        {
            NodeStatus status = child->Tick();
            if (status == NodeStatus::Failure)
            {
                return NodeStatus::Failure;
            }            
            else if (status == NodeStatus::Running)
            {
                return NodeStatus::Running;
			}
        }
		return NodeStatus::Success;
    }
};