#pragma once
#include <string>
#include <vector>
using namespace std;
#include "sigslot_liangshi.h"

#if defined(WEBRTC_WIN) || defined(Q_OS_WIN32) || _WIN32
#define LIANGSHIEXOPRT _declspec(dllexport)
#else
#define LIANGSHIEXOPRT __attribute__ ((visibility("default")))
#endif

class HadesRecorder;
class HadesPlayer;
class AudioGod;
class HadesP2P;

struct StunServer
{
	char* uri;
	char* username;
	char* password;
};

namespace webrtc
{
	class AudioDeviceModule;
}

class LIANGSHIEXOPRT Hades
{
public:
	Hades();
	Hades(const Hades&);
	Hades& operator=(const Hades&);
	virtual ~Hades();

public:
	void Release();

	//Recorder相关
	void InitRecorder(char* filename, int seek, int time);
	bool HasMicrophone();
	void StartRecording();
	void ContinueRecording();
	void PauseRecording();
	void StopRecording();
	void FlushFile();

	//P2P相关
	void InitP2P(StunServer stuns[], int numberOfStuns, char* identifier, bool createOffer, bool hasLocalVideo, bool hasRemoteVideo);
	void OnRemoteSdp(char* type, char* sdp);
	void OnRemoteCandidate(char* sdp_mid, int sdp_mlineindex, char* sdp);
	bool HasStream();

	//play file related
	void InitPlayer(char* filename);
	void StartPlaying();
	void ContinuePlaying();
	void PausePlaying();
	void StopPlaying();
	void GotoTime(int time);
	int GetPlayFilePosition(int time);
	int GetPlayFileDuration();

	sigslot_liangshi::signal1<int> SignalRecordData;
	sigslot_liangshi::signal1<int> RecordTimeUpdate;
	sigslot_liangshi::signal3<const char*, const char*, const char*> SignalSdp;
	sigslot_liangshi::signal4<const char*, const char*, int, const char*> SignalCandidateSdp;
	sigslot_liangshi::signal3<int, int, unsigned char*> SignalLocalVideoImage;
	sigslot_liangshi::signal3<int, int, unsigned char*> SignalRemoteVideoImage;

private:
	void InitAudioGod();

private:
	AudioGod* _audioGod;
	webrtc::AudioDeviceModule* _audioDevice;
	HadesRecorder* _recorder;
	HadesPlayer* _player;
	HadesP2P* _p2p;
	int _playPosition;
};
;