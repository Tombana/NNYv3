#pragma once

//======================================================
//TODO: Write the complete CLog class. :D
//
//First start making:
//Have a std::vector<ConsoleLine> that holds all console lines.
//ConsoleLine is a std::string with a colour value and timestamp.
//Then have a (static) function like AddLog(std::string text, bool errorlog = false)
//
//When that works:
//
//Goal:
//std::cout should be printed in ingame-console
//std::cerr should be printed as red in ingame-console and possibly be logged to a file
//Maybe a timestamp before each line. Problem: since std::cout and std::cin are STREAMS
// you can not determine seperate debugprints. One debugprint can (and will!) have multiple calls
// to the std::cout functions and may have multiple lines per debugprint.
//
//
//Possible solutions:
//
//- std::cout and std::cerr are redirected to two instances of std::stringstream
//  The main thread could check the stringstreams every 50 ms or so. (it's not like it's doing important stuff anyways)
//  When it detects a newline in one of the streams it adds it to the vector with a timestamp.
//  Problems: - timestamp will be 50 ms late
//            - when the main thread checks std::cout's stream first then std::cout output
//              might go before std::cerr even though std::cerr could have been called first.
//      !!->  - mutex needed ? is std::stringstream thread-safe: is the read pointer
//              seperate from the write pointer?
//
//- Subclass std::stringstream or std::ostream to my own class and overload the flush() / overflow() functions
//  or whatever the required functions are (google the ones that need to be overloaded).
//  Those overloaded functions should be called for every character that needs to be added/outputted.
//  This function could then somehow pass it to the vector.
//  Problems: - mutexes required for passing it to the vector
//            - i dunno lol
//
//======================================================

class CLog
{
public:
	CLog(void);
	~CLog(void);

	//For capturing console output.
	//std::stringstream		mConsoleOutput;
	//std::stringstream		mErrorOutput;
	//std::streambuf		*mOldConsoleOutput;
	//std::streambuf		*mOldErrorOutput;
	//Code for redirect console output:
	//mOldConsoleOutput = std::cout.rdbuf(mConsoleOutput.rdbuf());
	//mOldErrorOutput = std::cerr.rdbuf(mErrorOutput.rdbuf());
};
