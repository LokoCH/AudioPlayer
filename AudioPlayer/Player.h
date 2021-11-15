#ifndef PLAYER
#define PLAYER
#include <vector>
#include <memory>
#include "Song.h"

class Player
{
public:
	virtual void play() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;
	virtual void addSong(Song* song) = 0;
	virtual int getCurrentTime() = 0;
	virtual int getFullTime() = 0;
	virtual ~Player() {};

protected:
	Song* song;
};


#endif //!PLAYER