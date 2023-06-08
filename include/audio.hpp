#include "include_sdl.cpp"

class Audio{
	public:
		void load(const char* filename);
		void play();
		Uint32 get_state();
		void destroy();
	private:
		Uint32 wavlength;
		Uint8 *wavbuffer;
		SDL_AudioSpec wavspec;
		SDL_AudioDeviceID deviceid;
		bool ready=false;
};

void Audio::load(const char* filename){
	SDL_FreeWAV(wavbuffer);
	SDL_LoadWAV(filename , &wavspec , &wavbuffer , &wavlength);
	deviceid = SDL_OpenAudioDevice(NULL , 0 , &wavspec , NULL , 0);
	ready=true;
}

void Audio::play(){
	if (ready){
		SDL_QueueAudio(deviceid , wavbuffer , wavlength);
		SDL_PauseAudioDevice(deviceid , 0);
	}
}

Uint32 Audio::get_state(){
	return SDL_GetQueuedAudioSize(deviceid);
}

void Audio::destroy(){
	SDL_FreeWAV(wavbuffer);
	SDL_CloseAudioDevice(deviceid);
}
