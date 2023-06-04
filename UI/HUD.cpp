#include "HUD.hpp"
#include <GL/glew.h>
#include "Shaders/shaders.hpp"
#include "Textures/bmp.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <string>

void HUDManager::Init(int* width, int* height) {
	this->width = width;
	this->height = height;
	drawDebug = true;

	GenerateTextData();

	GenerateBuffers();
	GenerateTextBuffers();
}

void HUDManager::CreateBuffers() {
	if (!buffersGenerated) {
		buffersGenerated = true;

		float chA = 10;
		float chB = 1;

		vertex_buffer_data = {
			-chA,  chB, 0.0f,
			 chA,  chB, 0.0f,
			-chA, -chB, 0.0f,
			 chA, -chB, 0.0f,

			 chB, -chA, 0.0f,
			 chB,  chA, 0.0f,
			-chB, -chA, 0.0f,
			-chB,  chA, 0.0f
		};

		colour_buffer_data = {
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,

			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f
		};

		index_buffer_data = {
			0, 2, 1,
			1, 2, 3,

			4, 5, 6,
			5, 7, 6
		};

		glGenVertexArrays(2, VertexArrayIDs);
		glGenBuffers(8, buffers);
		programID = LoadShaders("C:/Users/ben_l/source/repos/Blockmine/Shaders/hudVert.shader", "C:/Users/ben_l/source/repos/Blockmine/Shaders/hudFrag.shader");
		textProgramID = LoadShaders("C:/Users/ben_l/source/repos/Blockmine/Textures/TextVert.shader", "C:/Users/ben_l/source/repos/Blockmine/Textures/TextFrag.shader");
		textTexture = loadBMP_custom("C:/Users/ben_l/source/repos/Blockmine/Text/Font.bmp");
		text_buffer_data_length = 0;
	}
}

void HUDManager::GenerateBuffers() {
	CreateBuffers();

	glBindVertexArray(VertexArrayIDs[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_buffer_data.size(), vertex_buffer_data.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colour_buffer_data.size(), colour_buffer_data.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index_buffer_data.size(), index_buffer_data.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void HUDManager::GenerateTextBuffers() {
	CreateBuffers();

	glBindVertexArray(VertexArrayIDs[1]);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * text_position_buffer_data.size(), text_position_buffer_data.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * text_uv_buffer_data.size(), text_uv_buffer_data.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * text_foreground_colour_buffer_data.size(), text_foreground_colour_buffer_data.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * text_background_colour_buffer_data.size(), text_background_colour_buffer_data.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[7]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * text_index_buffer_data.size(), text_index_buffer_data.data(), GL_DYNAMIC_DRAW);

	

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void HUDManager::Draw() {
	DrawCrosshair();
	if (drawDebug) {
		DrawText();
	}
}

void HUDManager::DrawCrosshair() {
	glBindVertexArray(VertexArrayIDs[0]);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);

	glUseProgram(programID);
	glUniform1i(glGetUniformLocation(programID, "screenWidth"), *width);
	glUniform1i(glGetUniformLocation(programID, "screenHeight"), *height);
	glUniform1f(glGetUniformLocation(programID, "scale"), 1);

	glDrawElements(
		GL_TRIANGLES,
		index_buffer_data.size(),
		GL_UNSIGNED_INT,
		(void*)0
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void HUDManager::DrawText() {
	GenerateTextData();
	glBindTexture(GL_TEXTURE_2D, textTexture);
	glBindVertexArray(VertexArrayIDs[1]);

	// Vertex Attrib pointer positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	// Vertex Attrib pointer UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[5]);
	glVertexAttribPointer(
		2,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[6]);
	glVertexAttribPointer(
		3,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[7]);

	glUseProgram(textProgramID);
	//GLfloat backgroundColour[4] = { 0.0f, 0.0f, 0.0f, 0.3f };
	//GLfloat foregroundColour[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	//glUniform4fv(glGetUniformLocation(textProgramID, "backgroundColour"), 1, backgroundColour);
	//glUniform4fv(glGetUniformLocation(textProgramID, "foregroundColour"), 1, foregroundColour);

	glUniform1i(glGetUniformLocation(textProgramID, "screenWidth"), *width);
	glUniform1i(glGetUniformLocation(textProgramID, "screenHeight"), *height);
	glUniform1i(glGetUniformLocation(textProgramID, "scale"), 1);

	glDrawElements(
		GL_TRIANGLES,
		text_index_buffer_data.size(),
		GL_UNSIGNED_INT,
		(void*)0
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void getCharacterUV(char character, float& u1, float& v1, float& u2, float& v2) {
	int xPosition = character % 16;
	int yPosition = 16 - (character / 16) - 1;

	u1 = (float)xPosition / 16.0f;
	v1 = 1 - (float)yPosition / 16.0f;
	u2 = ((float)xPosition + 1) / 16.0f;
	v2 = 1 - ((float)yPosition + 1) / 16.0f;
}

//void HUDManager::AddTextToBuffer(char* text, int row, int column) {
//	AddTextToBuffer(text, row, column);
//}

int HUDManager::AddTextToBuffer(const char* text, int row, int column) {

	float characterHeight = (2.0f / *height) * 16;
	float characterWidth = (2.0f / *width) * 16;

	int textPosition = 0;
	while (text[textPosition] != '\0') {
		if (text[textPosition] == '\n') {
			row++;
			textPosition++;
			column = 0;
			continue;
		}

		char character = text[textPosition];
		float u1, v1, u2, v2;
		getCharacterUV(character, u1, v1, u2, v2);
		int indexOffset = text_position_buffer_data.size() / 2;

		text_position_buffer_data.push_back(column);
		text_position_buffer_data.push_back(row);

		text_position_buffer_data.push_back(column + 1);
		text_position_buffer_data.push_back(row);

		text_position_buffer_data.push_back(column + 1);
		text_position_buffer_data.push_back(row + 1);

		text_position_buffer_data.push_back(column);
		text_position_buffer_data.push_back(row + 1);

		text_uv_buffer_data.push_back(u1);
		text_uv_buffer_data.push_back(v2);

		text_uv_buffer_data.push_back(u2);
		text_uv_buffer_data.push_back(v2);

		text_uv_buffer_data.push_back(u2);
		text_uv_buffer_data.push_back(v1);

		text_uv_buffer_data.push_back(u1);
		text_uv_buffer_data.push_back(v1);

		text_index_buffer_data.push_back(indexOffset + 0);
		text_index_buffer_data.push_back(indexOffset + 2);
		text_index_buffer_data.push_back(indexOffset + 1);

		text_index_buffer_data.push_back(indexOffset + 0);
		text_index_buffer_data.push_back(indexOffset + 3);
		text_index_buffer_data.push_back(indexOffset + 2);

		text_foreground_colour_buffer_data.push_back(1.0f);
		text_foreground_colour_buffer_data.push_back(1.0f);
		text_foreground_colour_buffer_data.push_back(1.0f);
		text_foreground_colour_buffer_data.push_back(1.0f);

		text_foreground_colour_buffer_data.push_back(1.0f);
		text_foreground_colour_buffer_data.push_back(1.0f);
		text_foreground_colour_buffer_data.push_back(1.0f);
		text_foreground_colour_buffer_data.push_back(1.0f);

		text_foreground_colour_buffer_data.push_back(1.0f);
		text_foreground_colour_buffer_data.push_back(1.0f);
		text_foreground_colour_buffer_data.push_back(1.0f);
		text_foreground_colour_buffer_data.push_back(1.0f);

		text_foreground_colour_buffer_data.push_back(1.0f);
		text_foreground_colour_buffer_data.push_back(1.0f);
		text_foreground_colour_buffer_data.push_back(1.0f);
		text_foreground_colour_buffer_data.push_back(1.0f);


		text_background_colour_buffer_data.push_back(0.0f);
		text_background_colour_buffer_data.push_back(0.0f);
		text_background_colour_buffer_data.push_back(0.0f);
		text_background_colour_buffer_data.push_back(0.3f);

		text_background_colour_buffer_data.push_back(0.0f);
		text_background_colour_buffer_data.push_back(0.0f);
		text_background_colour_buffer_data.push_back(0.0f);
		text_background_colour_buffer_data.push_back(0.3f);

		text_background_colour_buffer_data.push_back(0.0f);
		text_background_colour_buffer_data.push_back(0.0f);
		text_background_colour_buffer_data.push_back(0.0f);
		text_background_colour_buffer_data.push_back(0.3f);

		text_background_colour_buffer_data.push_back(0.0f);
		text_background_colour_buffer_data.push_back(0.0f);
		text_background_colour_buffer_data.push_back(0.0f);
		text_background_colour_buffer_data.push_back(0.3f);

		textPosition++;
		column++;
	}
	return row;
}

void HUDManager::GenerateTextData() {
	text_position_buffer_data.clear();
	text_uv_buffer_data.clear();
	text_index_buffer_data.clear();
	text_foreground_colour_buffer_data.clear();
	text_background_colour_buffer_data.clear();

	//AddTextToBuffer("Hi\nA", 0, 0);
	int bufferLength = 50;
	char* buffer = new char[bufferLength];

	std::snprintf(buffer, bufferLength, "Delta Time: %.0fms\n", deltaTime);
	AddTextToBuffer(buffer, 0, 0);

	std::snprintf(buffer, bufferLength, "Frame Rate: %.0ffps\n", frameRate);
	AddTextToBuffer(buffer, 1, 0);
	
	std::snprintf(buffer, bufferLength, "Position: (%i,%i,%i)\n", posX, posY, posZ);
	int start = AddTextToBuffer(buffer, 2, 0);

	for (int i = 0; i < items.size(); i++) {
		std::snprintf(buffer, bufferLength, "%s: %5.0f%s\n", items[i].name, items[i].value, items[i].suffix);
		start = AddTextToBuffer(buffer, start, 0);
	}

	delete[] buffer;

	GenerateTextBuffers();
}

HUDManager::~HUDManager() {
	glDeleteVertexArrays(2, VertexArrayIDs);
	glDeleteBuffers(8, buffers);
	glDeleteShader(programID);
}

void HUDManager::SetDeltaTimeS(float time) {
	for (int i = 0; i < 9; i++) {
		frameRates[i] = frameRates[i + 1];
		deltaTimes[i] = deltaTimes[i + 1];
	}
	frameRates[9] = 1.0f / time;
	deltaTimes[9] = time * 1000;
	frameRate = frameRate - (frameRates[0] / 10) + (frameRates[9] / 10);
	deltaTime = deltaTime - (deltaTimes[0]) + (deltaTimes[9]);
	//GenerateTextData();
}

void HUDManager::SetPosition(int x, int y, int z) {
	posX = x;
	posY = y;
	posZ = z;
}

void HUDManager::EnableDebug() {
	drawDebug = true;
}

void HUDManager::DisableDebug() {
	drawDebug = false;
}

bool HUDManager::ToggleDebug() {
	drawDebug = !drawDebug;
	return drawDebug;
}

void HUDManager::SetAttribute(const char* name, float value, const char* suffix) {
	for (int i = 0; i < items.size(); i++) {
		if (*(items[i].name) == *(name)) {
			int j = 0;
			while (items[i].name[j] == name[j]) {
				if (name[j] == '\0') {
					items[i].value = value;
					items[i].suffix = suffix;
					return;
				}
				j++;
			}
		}
	}
	HUDItem newItem = { name, value, suffix };
	items.push_back(newItem);
}