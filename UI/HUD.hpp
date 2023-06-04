#pragma once
#include <vector>

struct HUDItem {
	const char* name;
	double value;
	const char* suffix;
};

class HUDManager {
	std::vector<float> vertex_buffer_data;
	std::vector<float> colour_buffer_data;
	std::vector<unsigned int> index_buffer_data;

	// 0 - crosshair vertices, 1 - text vertices
	unsigned int VertexArrayIDs[2];
	// 0-2 - crosshair buffers
	// 3-7 - text buffers
	unsigned int buffers[8];

	unsigned int programID;
	unsigned int textProgramID;
	unsigned int textTexture;
	bool buffersGenerated;

	size_t text_buffer_data_length;
	std::vector<float> text_position_buffer_data;
	std::vector<float> text_uv_buffer_data;
	std::vector<float> text_foreground_colour_buffer_data;
	std::vector<float> text_background_colour_buffer_data;
	std::vector<unsigned int> text_index_buffer_data;

	bool drawDebug;

	// 5 frame rolling average
	float frameRate;
	float deltaTime;

	// 5 frame history
	float frameRates[10];
	float deltaTimes[10];

	int posX, posY, posZ;

	std::vector<HUDItem> items;

	int* width;
	int* height;

	void CreateBuffers();
	void GenerateBuffers();
	void GenerateTextBuffers();

	void DrawCrosshair();
	void DrawText();

	int AddTextToBuffer(const char* text, int row, int column);
	//void AddTextToBuffer(char* text, int row, int column);
	void GenerateTextData();

public:
	void SetDeltaTimeS(float time);
	void SetPosition(int x, int y, int z);

	void SetAttribute(const char* name, float value, const char* suffix);

	void EnableDebug();
	void DisableDebug();
	bool ToggleDebug();

	void Init(int* width, int* height);

	void Draw();

	~HUDManager();
};