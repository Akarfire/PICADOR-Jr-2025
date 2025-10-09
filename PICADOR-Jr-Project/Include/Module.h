#pragma once

class Module
{
	class PicadorJrCore* core;

public:
	Module(PicadorJrCore* core_)) : core(core_) {}

	// Main function of the module
	virtual short runModule();
};