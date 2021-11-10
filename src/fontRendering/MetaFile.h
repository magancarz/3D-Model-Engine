#pragma once

#include "../Headers.h"

#include "Character.h"

/**
 * Provides functionality for getting the values from a font file.
 */
class MetaFile {
public:
	MetaFile(std::string file);
	double getSpaceWidth();
	Character* getCharacter(unsigned ascii);

private:
	bool processNextLine();
	int getValueOfVariable(std::string variable);
	std::vector<int> getValuesOfVariable(std::string variable);
	void close();
	bool openFile(std::string file);
	void loadPaddingData();
	void loadLineSizes();
	void loadCharacterData(int imageWidth, int desiredPadding);
	Character* loadCharacter(int imageSize, int desiredPadding);

	static constexpr int PAD_TOP = 0;
	static constexpr int PAD_LEFT = 1;
	static constexpr int PAD_BOTTOM = 2;
	static constexpr int PAD_RIGHT = 3;

	//static constexpr int DESIRED_PADDING = 8; // was 3

	std::string SPLITTER = " ";
	std::string NUMBER_SEPARATOR = ",";

	double aspectRatio;
	double verticalPerPixelSize;
	double horizontalPerPixelSize;
	double spaceWidth;
	std::vector<int> padding;
	int paddingWidth;
	int paddingHeight;
	std::map<int, Character*> metaData;
	std::map<std::string, std::string> values;
	std::ifstream ifs; // input file
};