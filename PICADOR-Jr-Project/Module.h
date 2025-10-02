#pragma once

class Module
{
	class PicadorJrCore* core;

public:
	Module(PicadorJrCore* inCore) : core(inCore) {}

	// Main function of the module
	virtual int runModule();
};