#ifndef BASS_PLAYER
#define BASS_PLAYER
#include <conio.h>
#include "Player.h"
#include "bass.h"
#include "Button.h"

class BASSPlayer :public Player
{
public:
	void play() override;
	void pause() override;
	void stop() override;
	void addSong(Song* song) override;
	int getCurrentTime() override;
	int getFullTime() override;

private:
	HSTREAM sound;
};

void BASSPlayer::play() {
	if (!song)
		return;
	BASS_ChannelStop(sound);
	BASS_StreamFree(sound);

	sound = BASS_StreamCreateFile(false, song->getPath().c_str(), 0, 0, BASS_STREAM_AUTOFREE);
	std::cout << song->getName();

	BASS_ChannelPlay(sound, false);
}

void BASSPlayer::pause() {
	if (BASS_ChannelIsActive(sound) == BASS_ACTIVE_PAUSED)
		BASS_ChannelPlay(sound, false);
	else
		BASS_ChannelPause(sound);
}

void BASSPlayer::stop() {
	BASS_ChannelStop(sound);
	std::cout << "ST\n";
}

void BASSPlayer::addSong(Song* song) {
	this->song = song;
}

int BASSPlayer::getCurrentTime() {
	QWORD len = BASS_ChannelGetPosition(sound, BASS_POS_BYTE);
	int currentTime = BASS_ChannelBytes2Seconds(sound, len);
	return currentTime;
}

int BASSPlayer::getFullTime() {
	DWORD len = BASS_ChannelGetLength(sound, BASS_POS_BYTE);
	int fullTime = BASS_ChannelBytes2Seconds(sound, len);
	return fullTime;
}

#endif //!BASS_PLAYER