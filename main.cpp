#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <cctype>

/**
 * @name processLogicalLine
 * @def Function to process the input lines according to the given rules
 * @author Theint Nwe Nyein
 * @return String
 */
 //process the line
std::string processLogicalLine(const std::string& line) {
	//there is a bug. Read log for more info
	std::string processed;
	bool inSingleQuotes = false;
	bool inDoubleQuotes = false;
	bool isComment = false;

	for (size_t i = 0; i < line.length(); ++i) {
		char currentChar = line[i];

		// Handle single and double quotes
		if (currentChar == '\'' && (i == 0 || line[i - 1] != '\\')) {
			inSingleQuotes = !inSingleQuotes;
		}
		else if (currentChar == '\"' && (i == 0 || line[i - 1] != '\\')) {
			inDoubleQuotes = !inDoubleQuotes;
		}

		// Check for comment
		if (currentChar == '#' && !inSingleQuotes && !inDoubleQuotes) {
			isComment = true;
			break; // Stop processing as it's a comment
		}
		if (!line.empty() && line.back() == '\\') {
			// Remove the backslash indicating continuation
			return ""; // Return an empty string or a special marker
		}
		// Add character if it's not a space or the first space after non-space characters
		if (!std::isspace(currentChar) || (processed.empty() || !std::isspace(processed.back()))) {
			processed += currentChar;
		}
	}

	// Trim trailing spaces
	processed.erase(std::find_if(processed.rbegin(), processed.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), processed.end());

	// Check for line continuation if there's no comment
	if (!isComment && !processed.empty() && processed.back() == '\\') {
		processed.pop_back(); // Remove continuation character
		return processed + " "; // Add space for next line concatenation
	}


	return processed;

}// end processLogicalLine
std::string classifyWord(const std::string& word, const std::map<std::string, std::string>& table) {
	// Implement logic to classify the word
	auto it = table.find(word);
	if (it != table.end()) {
		return it->second; // Found in table
	}
	else {
		return "external"; // Not found in table
	}
}


/**
 * @name main
 * @def Main function of the program
 * @author Josh Moore
 * @authors Maria Colon, Leo Goldstein, Theint Nwe Nyein
 * @return 0
*/
int main() {

	std::string inputText;     //Store the input line
	std::ostringstream logicalLines; //Store all the logical lines after being processed
	std::string continuationLine;  // Store the continuation line.
	bool singleComment;  // Track if it's a single quoted comment or not
	bool doubleComment;  // Track if it's a double quoted comment or not

	std::map<std::string, std::string> dictionary = {

		// Short words
		{"CAT","short"},
		{"DOD","short"},
		{"ICE","short"},
		{"SUN","short"},
		{"THE","short"},
		{"IT","short"},
		{"FOR","short"},
		{"CAR","short"},
		{"BUS","short"},
		{"PEN","short"},

		//Medium words
		{"APPLE","medium"},
		{"TIME","medium"},
		{"THERE","medium"},
		{"MOON","medium"},
		{"FOOD","medium"},
		{"WINTER","medium"},
		{"SUMMER","medium"},
		{"SMALL","medium"},
		{"PEOPLE","medium"},
		{"LEMON","medium"},

		//Long words
		{"ELEPHANT","long"},
		{"WATERMELON","long"},
		{"STRAWBERRY","long"},
		{"PINEAPPLE","long"},
		{"CHEETAH","long"},
		{"STUDENTS","long"},
		{"PROFESSORS","long"},
		{"UNIVERSITY","long"},
		{"COMPUTER","long"},
		{"SCIENCE","long"}

	};

	std::string inputLine;
	std::string currentLine;
	std::string processedLine;
	std::string firstWord;
	std::string classification;
	bool continuation = false;

	while (true) {
		// Print prompt
		std::cout << (continuation ? ">>> " : "ALPHA $ ");

		// Get input
		std::getline(std::cin, inputLine);

		// Process the line
		processedLine = processLogicalLine(inputLine);

		// Check for continuation
		if (!processedLine.empty() && inputLine.substr(inputLine.length() - 2, inputLine.length()) == "\\n" && inputLine.find("#") == std::string::npos) {
			continuation = true;
			currentLine += " " + inputLine;
			continue;
		}

		currentLine += " " + inputLine;
		processedLine = processLogicalLine(currentLine);

		// Extract the first word
		std::istringstream iss(processedLine);
		iss >> firstWord;

		// Check for exit condition
		if (firstWord == "exit") {
			break;
		}

		if(continuation && firstWord.substr(firstWord.length() - 2, firstWord.length()) == "\\n")
			firstWord = firstWord.substr(0, firstWord.length() - 2);

		std::string tmpCompareValue = firstWord;

		for(char& c : tmpCompareValue) {
			c = std::toupper(c);
		}

		// Classify the word
		classification = classifyWord(tmpCompareValue, dictionary);

		// Output the result
		std::cout << firstWord << ": " << classification << std::endl;

		// Reset used variables for next NEW input of values.
		currentLine = "";
		continuation = false;
	}

	return 0;
}//end main

// External Methods from Pre-Defined Functions

// +======== NOTES =========+
/**
 * This section is used for any changes done to the program. This can also contain information
 * about certain properties of the program that can/will affect other portions of the program
 * that are necessary (like user input, output etc.). To have consistency please use this format:
 *
 * NAME: name
 * DATE: 01 January 2023
 * LOG: INFORMATION
 *
 *
 * Name:Theint Nwe Nyein
 * DATE: 14 January 2023
 * LoG: inputText is used to read the user's input line, where processedLine is the final processed result of each line.
 * 	processedLine should be used to get the first character for Section C.The program asks for the input in a while loop
 * 	so there is no limit for the number of lines the user can enter. The user must use Ctrl+D to exit the loop and see the result.
 *
 *
 * Name: Josh Moore
 * Date: 15 January 2024
 * Log: Removed an unncessary include statement (including external .h files that are not needed for this project); moved the information
 * for the main function back to the main function. Also added some documentation to the processLogicalLine function. Removed unused placeholder
 * sections for program attributes (Global Variables, Function Definiions, etc.) Removed "include_files" folder with .h file, unneeded for project.
 *
 *
 * Name: Leo Goldstein
 * Date: 16 January 2024
 * Log: Reworked line processing. As it was, there was no way to exit the loop as Ctrl+D did nothing. Currently there is a bug with line continuation
 * The bug is that when you input a continuation(for example, Science \) it does not wait for next user input and instead immediately says what it thinks the first
 * word is, which is the previous word(so if your most recent input is The, it will think The is the word and return The: short). Not sure how to fix yet
 * 
 * Name: Josh Moore
 * Date: 21 January 2023
 * Log: Fixed the Continuation error; Added handler for Diferrent input casing to run against the dictionary. Removed commented out code that was not necessary.
 * Added handler for comment and New-Line Function.
*/

//
