#pragma once

enum class ModuleExecutionStatus {Success, Error};

class Module
{
	class PicadorJrCore* core;

public:
	Module(PicadorJrCore* core_) : core(core_) {}
	virtual ~Module() {}

	// Main function of the module
	virtual ModuleExecutionStatus runModule() = 0;
};