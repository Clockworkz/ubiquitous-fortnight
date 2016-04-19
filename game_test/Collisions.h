#pragma once

class Collisions {
public:
	Collisions(bool topCol, bool botCol, bool leftCol, bool rightCol) : top(topCol), bottom(botCol), left(leftCol), right(rightCol) {};

	bool left;
	bool right;
	bool top;
	bool bottom;
};