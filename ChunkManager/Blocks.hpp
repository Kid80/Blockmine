#pragma once

// Top - Y = 1
// Bottom - Y = 0
// Front - X = 1
// Back - X = 0
// Left - Z = 0
// Right - Z = 1
enum Faces { Top = 0, Bottom, Front, Back, Left, Right };
static int facesOffsetX[6]{ 0, 0, 1, -1, 0, 0 };
static int facesOffsetY[6]{ 1, -1, 0, 0, 0, 0 };
static int facesOffsetZ[6]{ 0, 0, 0, 0, -1, 1 };

enum Blocks {
	air = 0,
	grass = 1,
	dirt = 2
};

struct blockData {
	bool visible;
	int topTexIndex;
	int bottomTexIndex;
	// Front, right, left, back
	int sideTexIndices[4];

};

static blockData data[3]{
	{false, 0, 0, 0, 0, 0, 0 },
	{true, 0, 2, 1, 1, 1, 1},
	{true, 2, 2, 2, 2, 2, 2}
};