#pragma once

class Player
{
public:
	uint64						playerId = 0;
	string						name;
	Protocol::LevelInfo			levelinfo;
	GameSessionRef				ownerSession; // Cycle
};

