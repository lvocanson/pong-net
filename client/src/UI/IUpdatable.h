#pragma once

class IUpdatable
{
public:

	virtual void OnUpdate(float dt) = 0;
};