#include "StringParser.h"

std::string stripSpaces(const std::string& inputString)
{
	std::string outputString;
	const size_t inputStringSize = inputString.size();
	size_t startIndex = 0;

	if (inputStringSize == 0)
		return "";

	size_t endIndex = inputStringSize - 1;

	while (startIndex < inputStringSize && inputString.at(startIndex) == ' ')
		startIndex++;

	if (startIndex == inputStringSize)
		outputString = "";

	while (endIndex > startIndex && inputString.at(endIndex) == ' ')
		endIndex--;

	for (size_t i = startIndex; i <= endIndex; i++)
		outputString.push_back(inputString.at(i));
	return outputString;
}
