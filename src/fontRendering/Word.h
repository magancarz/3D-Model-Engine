#pragma once

#include "../Headers.h"

#include "Character.h"

/**
 * During the loading of a text this represents one word in the text.
 */
class Word {
public:
	Word(double fontSize);
	void addCharacter(Character character);
	std::vector<Character> getCharacters();
	double getWordWidth();

private:
	std::vector<Character> characters;
	double width = 0;
	double fontSize;	
};