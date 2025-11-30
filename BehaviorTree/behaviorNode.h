#pragma once

//ビヘイビアツリーの基底ノード

enum class NodeStatus
{
    Success,    //成功
	Failure,    //失敗
    Running     //処理中
};

class BehaviorNode
{
public:
    virtual ~BehaviorNode() = default;
    virtual NodeStatus Tick() = 0;
};