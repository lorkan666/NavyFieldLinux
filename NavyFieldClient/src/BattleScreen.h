#pragma once
#include "GameScreen.h"

class BattleScreen :
	public GameScreen

{
public:
	BattleScreen(string name);
	void enter();
	~BattleScreen(void);
};

