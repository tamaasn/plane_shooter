#include "include_sdl.cpp"

struct Player{
	SDL_Rect rect;
	SDL_Rect src_rect;
	int anim_timer = 4;
	int anim_index=0;
};

struct Enemies{
	bool explode=false;
	bool alive=false;
	int anim_timer=4;
	int anim_index=0;
	int shoot_timer=100;
	SDL_Rect rect;
	SDL_Rect src_rect;
};

struct Bullets{
	bool alive=false;
	SDL_Rect rect;
};

struct Ships{
	bool exist=false;
	SDL_Rect rect;
	SDL_Rect src_rect;
	int anim_timer=5;
	int anim_index=0;
};

struct Enemy_bullets{
	bool alive=false;
	SDL_Rect rect;
};

SDL_Surface *HEART_SURFACE = IMG_Load("images/heart.png");
SDL_Surface *ENEMY_SURFACE = IMG_Load("images/enemy.png");
SDL_Surface *SHIP_SURFACE = IMG_Load("images/ship.png");
SDL_Surface *PLAYER_SURFACE = IMG_Load("images/player.png");
SDL_Surface *BACKGROUND_SURFACE = IMG_Load("images/bg.png");
SDL_Surface *BULLET_SURFACE = IMG_Load("images/bullet.png");
SDL_Surface *ENEMY_BULLET_SURFACE =IMG_Load("images/enemy_bullet.png");

SDL_Texture *PLAYER_TEXTURE;
SDL_Texture *ENEMY_TEXTURE;
SDL_Texture *SHIP_TEXTURE;
SDL_Texture *BACKGROUND_TEXTURE;
SDL_Texture *BULLET_TEXTURE;
SDL_Texture *HEART_TEXTURE;
SDL_Texture *ENEMY_BULLET_TEXTURE;

int ENEMY_TIMER = 20;
int SHIP_TIMER = 40;

void OBJECT_LOAD_TEXTURES(SDL_Renderer *render){
	PLAYER_TEXTURE = SDL_CreateTextureFromSurface(render , PLAYER_SURFACE);
	ENEMY_TEXTURE = SDL_CreateTextureFromSurface(render , ENEMY_SURFACE);
	SHIP_TEXTURE = SDL_CreateTextureFromSurface(render , SHIP_SURFACE);
	BACKGROUND_TEXTURE = SDL_CreateTextureFromSurface(render , BACKGROUND_SURFACE);
	BULLET_TEXTURE = SDL_CreateTextureFromSurface(render , BULLET_SURFACE);
	HEART_TEXTURE = SDL_CreateTextureFromSurface(render , HEART_SURFACE);
	ENEMY_BULLET_TEXTURE = SDL_CreateTextureFromSurface(render , ENEMY_BULLET_SURFACE);
	SDL_FreeSurface(HEART_SURFACE);
	SDL_FreeSurface(PLAYER_SURFACE);
	SDL_FreeSurface(ENEMY_SURFACE);
	SDL_FreeSurface(SHIP_SURFACE);
	SDL_FreeSurface(BACKGROUND_SURFACE);
	SDL_FreeSurface(BULLET_SURFACE);
	SDL_FreeSurface(ENEMY_BULLET_SURFACE);
}

void OBJECT_DESTROY_TEXTURES(){
	SDL_DestroyTexture(HEART_TEXTURE);
	SDL_DestroyTexture(PLAYER_TEXTURE);
	SDL_DestroyTexture(ENEMY_TEXTURE);
	SDL_DestroyTexture(SHIP_TEXTURE);
	SDL_DestroyTexture(BACKGROUND_TEXTURE);
	SDL_DestroyTexture(BULLET_TEXTURE);
	SDL_DestroyTexture(ENEMY_BULLET_TEXTURE);

}

