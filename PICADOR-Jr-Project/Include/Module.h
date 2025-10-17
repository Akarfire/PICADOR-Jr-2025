#pragma once

enum class ModuleExecutionStatus {Success, Error};

class Module
{
protected:
	class PicadorJrCore* core;

public:
	Module(PicadorJrCore* core_) : core(core_) {}
	virtual ~Module() {}

	// Module functionality

	// Called by the core before entering the simulation loop
	virtual ModuleExecutionStatus onBegin() = 0;

	// Called on every iteration of the simulation loop
	virtual ModuleExecutionStatus onUpdate() = 0;

	// Called after the core has exited the simulation loop
	virtual ModuleExecutionStatus onEnd() = 0;
};