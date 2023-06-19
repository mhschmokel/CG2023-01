#pragma once
#include "SceneManager.h"

class IniFileParser
{
public:
	IniFileParser() {}
	~IniFileParser() {}
	sceneStruct parse(string iniFilePath);
};

