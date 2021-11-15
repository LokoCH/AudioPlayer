#ifndef SONG
#define SONG
#include <iostream>
#include <string>
#include <vector>

enum class Status
{
	playing,
	paused,
	stopped,
};

class Song
{
public:
	Song(std::string path) :name{ path }, path{ path } {}

	std::string getName() { return name; }
	std::string getPath() { return path; }
	Song* getSong() { return this; }

protected:
	std::string name;
	std::string path;
};

#endif //!SONG