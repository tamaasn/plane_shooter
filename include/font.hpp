#include "include_sdl.cpp"
class Font{
	public:
		void init();
		void create_text(SDL_Texture **texture ,SDL_Renderer* render ,const char *text);
		void destroy();
		void render(SDL_Renderer *render , const char *text , SDL_Rect *rect);
	private:
		SDL_Color color = {255 , 255 , 255};
		TTF_Font *font;
};

void Font::init(){
	TTF_Init();
	font = TTF_OpenFont("font.ttf" , 24);
}

void Font::create_text(SDL_Texture **texture, SDL_Renderer* render , const char *text){
	SDL_Surface *surface;
	surface = TTF_RenderText_Solid(font , text , color);
	*texture = SDL_CreateTextureFromSurface(render , surface);
	SDL_FreeSurface(surface);
}

void Font::destroy(){
	TTF_CloseFont(font);
	font=NULL;
}

void Font::render(SDL_Renderer* render , const char*text , SDL_Rect *rect){ // Update font while render it
	SDL_Surface *surface=TTF_RenderText_Solid(font , text , color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(render , surface);
	SDL_RenderCopy(render , texture , NULL , rect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

