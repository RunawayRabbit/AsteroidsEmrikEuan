
#pragma once

class IState
{
public:
	IState() {};
	virtual ~IState() {};

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};