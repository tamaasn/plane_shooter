//Sorry if the source code is kinda messy :/
//Source code made by tamaaasn
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "include/font.hpp"
#include "include/objects.hpp"
#include "include/keys.hpp"
#include "include/audio.hpp"
#include <string>
#include <iostream>
#include <cstdlib>
#define width 640
#define height 480
#define enemy_max 5
#define bullet_max 10
#define ship_max 6
#define speed 5

//Initializing window , and render
SDL_Window *window;
Uint32 render_flag;
SDL_Renderer *rend;

// Object class
Player player;
Enemies enemies[enemy_max];
Bullets bullets[bullet_max];
Ships ships[ship_max];
Enemy_bullets enemy_bullets[bullet_max];

// Classes
Audio audio;
Keys key;
Font font;


//Destroy window
void destroy(){
	font.destroy();
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

// Game over screen

void gameover(){
	bool run=true;
	SDL_Rect gameover_rect;
	gameover_rect.w=400;
	gameover_rect.h=60;
	gameover_rect.x = (width/2)-gameover_rect.w/2;
	gameover_rect.y = (height/2);
	
	SDL_Rect text_rect;
	text_rect.w=400;
	text_rect.h=60;
	text_rect.x = gameover_rect.x;
	text_rect.y = gameover_rect.y+text_rect.h;
	
	SDL_Texture *gameover_tex;
	SDL_Texture *text_tex;
	
	font.create_text(&gameover_tex , rend , "GAMEOVER");
	font.create_text(&text_tex , rend , "Press ENTER to exit");
	
	SDL_Event event;
	while(run){
		SDL_RenderClear(rend);
		SDL_RenderCopy(rend , gameover_tex , NULL , &gameover_rect);
		SDL_RenderCopy(rend , text_tex , NULL , &text_rect);
		while(SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_KEYDOWN:
					switch(event.key.keysym.scancode){
						case SDL_SCANCODE_RETURN:run=false; destroy();
					}
					break;
				case SDL_QUIT: run=false;destroy();break;
			}
		}
		SDL_Delay(1000/60);
		SDL_RenderPresent(rend);
	}
	
}

void spawn_bullet(){

	//Make bullet alive

	for(int i=0;i<bullet_max;i++){
		if (!bullets[i].alive){
			bullets[i].rect.y = player.rect.y;
			bullets[i].rect.x = player.rect.x;
			bullets[i].rect.w = 16;
			bullets[i].rect.h = 32;
			bullets[i].alive=true;
			break;
		}
	}
}


void spawn_enemy(){

	// Make enemy alive

	if (ENEMY_TIMER-- < 0){
		for (int i=0;i<enemy_max;i++){
			if (!enemies[i].alive && !enemies[i].explode){
				enemies[i].rect.x = rand()%width-64;
				enemies[i].rect.w = 64;
				enemies[i].rect.h = 72;
				enemies[i].rect.y = -72;
				enemies[i].alive=true;
				enemies[i].src_rect={0,0,32,40};
				ENEMY_TIMER = 20;
				break;
			}
		}
	}
}


void spawn_ship(){ // Spawning ship
	if (SHIP_TIMER-- < 0){
		for (int i=0;i<ship_max;i++){
			if (!ships[i].exist){
				ships[i].exist=true;
				ships[i].rect.x = rand()%width-32;
				ships[i].rect.w = 32;
				ships[i].rect.h = 64;
				ships[i].rect.y = -64;
				ships[i].src_rect = {0,0,32,64};
				SHIP_TIMER=30;
				break;
			}
		}
	}
}

void move_ship(){ // Moving ship
	for(int i=0;i<ship_max;i++){

		if (ships[i].exist){
			SDL_RenderCopy(rend , SHIP_TEXTURE , &ships[i].src_rect , &ships[i].rect);

			ships[i].rect.y += speed/2;
			
			//Ship animation
			if (ships[i].anim_timer-- < 0){
				if (ships[i].anim_index++ >= 1){
					ships[i].anim_index=0;
				}
				ships[i].src_rect= {32*ships[i].anim_index , 0 , 32 , 64};
				ships[i].anim_timer=5;
			}
			if (ships[i].rect.y > height){
				ships[i].exist=false;
				ships[i].rect = {0,0,0,0};
			}
		}
	}
}



void enemy_shoot(SDL_Rect enemyrect){
	for(int i=0;i<bullet_max;i++){
		if (enemy_bullets[i].alive==false){
			enemy_bullets[i].rect.x = enemyrect.x;
			enemy_bullets[i].rect.y = enemyrect.y;
			enemy_bullets[i].rect.w = 16;
			enemy_bullets[i].rect.h = 32;
			enemy_bullets[i].alive=true;
			break;
		}
	}
}
			


//This is gameplay
void game(){

	std::string score_text = "x0";

	//Loading object and textures
	SDL_Texture* score_tex;

	//Configuring rect
	player.rect.w = 64;
	player.rect.h = 64;
	player.rect.x = width/2;
	player.rect.y = height/2;
	player.src_rect = {0 , 0 , 32 , 40};

	SDL_Rect heart_rect;
	heart_rect.w = 24;
	heart_rect.h = 24;
	heart_rect.x = 1;
	heart_rect.y = 0;
	
	SDL_Rect score_rect;
	score_rect.w = 32;
	score_rect.h = 32;
	score_rect.x = 32;
	score_rect.y = heart_rect.y + 32;

	SDL_Rect score_logo;
	score_logo.w = 32;
	score_logo.h = 32;
	score_logo.x = 0;
	score_logo.y = heart_rect.y + 32;
	
	SDL_Rect score_src_rect = {0,0,32,40};
	font.create_text(&score_tex , rend , score_text.c_str());

	//Play audio
	audio.load("audio/bg.wav");
	audio.play();

	uint64_t score = 0;
	SDL_Event event;
	bool run=true;
	int health=5;
	while (run){
		if (health == 0){
			run=false;
			audio.destroy();
			gameover();
		}
		
		//Play audio if its over
		Uint32 audiostate = audio.get_state();
		if (audiostate == 0){
			audio.play();
		}
		SDL_RenderClear(rend);
		SDL_RenderCopy(rend , BACKGROUND_TEXTURE  , NULL , NULL);
		
		
		score_text = "x"+std::to_string(score);

		spawn_enemy();
		spawn_ship();
		move_ship();

		for(int i=0;i<bullet_max;i++){ // Show bullets
			if (bullets[i].alive){
				SDL_RenderCopy(rend , BULLET_TEXTURE, NULL , &bullets[i].rect);
				bullets[i].rect.y -= speed+5;
				if (bullets[i].rect.y < 0){
					bullets[i].alive=false;
					bullets[i].rect = {0,0,0,0};
				}
			}
		}
		
		for (int i=0;i<bullet_max;i++){ // Show enemy bullets
			if (enemy_bullets[i].alive){
				SDL_RenderCopy(rend , ENEMY_BULLET_TEXTURE , NULL , &enemy_bullets[i].rect);
				enemy_bullets[i].rect.y += speed+3;

				// Bullet collision
				for(int j=0;j<bullet_max;j++){
					if(SDL_HasIntersection(&enemy_bullets[i].rect , &bullets[j].rect)){
						enemy_bullets[i].alive=false;
						enemy_bullets[i].rect={0,0,0,0};
						bullets[j].alive=false;
						bullets[j].rect={0,0,0,0};
						break;
					}
				}

				if (SDL_HasIntersection(&enemy_bullets[i].rect , &player.rect)){
					health--;
					enemy_bullets[i].alive=false;
					enemy_bullets[i].rect = {0,0,0,0};
				}

				
				if (enemy_bullets[i].rect.y > height){
					enemy_bullets[i].alive=false;
					enemy_bullets[i].rect = {0,0,0,0};
				}

			}
			
		}
		
		for(int i=0;i<enemy_max;i++){ // Show enemy
			if (enemies[i].alive){
				SDL_RenderCopy(rend , ENEMY_TEXTURE , &enemies[i].src_rect , &enemies[i].rect);
				enemies[i].rect.y += speed;

				//Enemy shoot
				enemies[i].shoot_timer--;
				if (enemies[i].shoot_timer < 0){
					enemies[i].shoot_timer = 100;
					enemy_shoot(enemies[i].rect);
				}
				


				for (int j=0;j<bullet_max;j++){
					if (SDL_HasIntersection(&bullets[j].rect , &enemies[i].rect)){
						enemies[i].alive=false;
						enemies[i].explode=true;
						bullets[j].alive=false;
						bullets[j].rect={0,0,0,0};
						score++;
						break;
					}
				}

				if (SDL_HasIntersection(&enemies[i].rect , &player.rect)){
					health--;
					enemies[i].alive=false;
					enemies[i].explode=true;
				}

				if (enemies[i].rect.y > height){
					enemies[i].alive=false;
					enemies[i].rect = {0,0,0,0};
				}
			}

			//Enemy explode animation

			if (enemies[i].explode){
				SDL_RenderCopy(rend , ENEMY_TEXTURE , &enemies[i].src_rect , &enemies[i].rect);
				if (enemies[i].anim_timer-- < 0){
					if (enemies[i].anim_index++ >= 2){
						enemies[i].explode=false;
						enemies[i].rect = {0,0,0,0};
						enemies[i].src_rect = {0,0,32,40};
						enemies[i].anim_index=0;
					}
					enemies[i].src_rect = {32*enemies[i].anim_index , 0 , 32 ,40};
					enemies[i].anim_timer=4;

				}
			}
		}
		
		//Player animation

		if (player.anim_timer-- < 0){
			if (player.anim_index++ >= 1){
				player.anim_index=0;
			}
			player.src_rect = {32*player.anim_index , 0  , 32 , 40};
			player.anim_timer = 4;
		}

		//Show health indication

		for (int i=0;i<health;i++){
			heart_rect.x = heart_rect.w * i;
			SDL_RenderCopy(rend , HEART_TEXTURE , NULL , &heart_rect);
			heart_rect.x = 0;
		}



		SDL_RenderCopy(rend , PLAYER_TEXTURE , &player.src_rect , &player.rect);
		SDL_RenderCopy(rend , ENEMY_TEXTURE , &score_src_rect , &score_logo);
		font.render(rend , score_text.c_str() , &score_rect);

		while (SDL_PollEvent(&event)){ // Handle event
			switch (event.type){
				case SDL_QUIT:
					run=false;
					OBJECT_DESTROY_TEXTURES();
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode){
						case SDL_SCANCODE_A: key.left=true;break;
						case SDL_SCANCODE_S: key.bottom=true;break;
						case SDL_SCANCODE_D: key.right=true;break;
						case SDL_SCANCODE_W: key.top=true;break;
						case SDL_SCANCODE_SPACE: spawn_bullet();break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.scancode){
						case SDL_SCANCODE_A: key.left=false;break;
						case SDL_SCANCODE_S: key.bottom=false;break;
						case SDL_SCANCODE_D: key.right=false;break;
						case SDL_SCANCODE_W: key.top=false;break;
					}
					break;

			}

		}

		if (key.top){
			player.rect.y -= speed;
		}
		if (key.left){
			player.rect.x -= speed;
		}
		if (key.right){
			player.rect.x += speed;
		}
		if (key.bottom){
			player.rect.y += speed;
		}

		if (player.rect.y < 0){
			player.rect.y = 0;
		}
		if (player.rect.x < 0){
			player.rect.x = 0;
		}
		if (player.rect.y > height-player.rect.h){
			player.rect.y = height-player.rect.h;
		}
		if (player.rect.x > width-player.rect.w){
			player.rect.x = width-player.rect.w;
		}


		SDL_RenderPresent(rend);
		SDL_Delay(1000/60);
	}
	OBJECT_DESTROY_TEXTURES();
}


//Main menu
int main(){
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		printf("Error");
		exit(0);
	}
	window = SDL_CreateWindow("Plane shooter" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , width , height , 0);
	render_flag = SDL_RENDERER_ACCELERATED;
	rend = SDL_CreateRenderer(window , -1 , render_flag);

	//Inializing font
	font.init();

	//Initializing font texture
	SDL_Texture *play;
	SDL_Texture *quit;
	SDL_Texture *cursor;

	// Loads object textures
	OBJECT_LOAD_TEXTURES(rend);

	
	//User input choice
	int choice_max=1;
	int choice=0;
	

	//Object rect
	SDL_Rect rect;
	rect.w = 60;
	rect.h = 50;
	rect.x = width/2;
	rect.y = height/2;
	SDL_Rect rect2;
	rect2.w = 60;
	rect2.h = 50;
	rect2.x = width/2;
	rect2.y = rect.y+50;
	SDL_Rect cursor_rect;
	cursor_rect.w = 50;
	cursor_rect.h = 50;
	cursor_rect.x = rect.x-50;
	cursor_rect.y= rect.y;
	
	//Running indication
	bool run=true;

	//Event
	SDL_Event event;
	
	//Creating text
	font.create_text(&play,rend , "Play");
	font.create_text(&quit , rend , "Quit");
	font.create_text(&cursor , rend , ">");

	while (run){
		SDL_RenderClear(rend);

		//Insert textures to render
		SDL_RenderCopy(rend , BACKGROUND_TEXTURE , NULL , NULL);

		// Ship animation on menu screen
		spawn_ship();
		move_ship();

		SDL_RenderCopy(rend , play , NULL , &rect);
		SDL_RenderCopy(rend , quit , NULL , &rect2);
		SDL_RenderCopy(rend , cursor , NULL , &cursor_rect);
		//Event handling
		while(SDL_PollEvent(&event) != 0){
			if (event.type == SDL_QUIT){
				run=false;
				break;
			}

			if (event.type == SDL_KEYDOWN){
				switch(event.key.keysym.scancode){
					case SDL_SCANCODE_A: run=false;SDL_DestroyTexture(play);SDL_DestroyTexture(quit);SDL_DestroyTexture(cursor);game();break;
					case SDL_SCANCODE_UP:
							     if (choice++ >=choice_max){
								     choice=0;
							     }
							     cursor_rect.y = rect.y + choice*50;

							     break;
					case SDL_SCANCODE_DOWN:
							     if (choice-- <= 0){
								     choice=1;
							     }
							     cursor_rect.y = rect.y + choice*50;

							     break;
					case SDL_SCANCODE_RETURN:
							     switch(choice){
								     case 0:run=false;SDL_DestroyTexture(play);SDL_DestroyTexture(quit);SDL_DestroyTexture(cursor);game();break;
								     case 1: run=false;destroy();break;
							     }
							     break;
				}				
			}

		}

		SDL_RenderPresent(rend);
		SDL_Delay(1000/60);
	}
		
}
