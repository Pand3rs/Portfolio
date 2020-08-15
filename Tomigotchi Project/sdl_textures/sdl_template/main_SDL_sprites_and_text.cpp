#include <iostream>
#include <assert.h>
#include <time.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.9\include\SDL.h"
#include "SDL2-2.0.9\include\SDL_image.h"
#include <iostream>
#include <assert.h>
#include <time.h>
using namespace std;

//download SDL and SDL image

//include SDL header
#include "SDL2-2.0.9\include\SDL.h"
//copy the SDL_image.h from the SDL image folder to your SDL include folder
#include "SDL2-2.0.9\include\SDL_image.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2main.lib")
//copy the SDL2_image.lib from the SDL image folder to your SDL lib folder
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2_image.lib")

#pragma comment(linker,"/subsystem:console")


namespace Game
{
	SDL_Renderer *renderer = NULL;
	int screen_width = 128;
	int screen_height = 128;
}

int main(int argc, char **argv)
{
	//place the png file next to your main.cpp
	char *image_filename = "digi_spritesheet.png";

	SDL_Init(SDL_INIT_VIDEO);
	srand(time(0));

	SDL_Window *window = SDL_CreateWindow("TomIgotchu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Game::screen_width, Game::screen_height, SDL_WINDOW_SHOWN);

	Game::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//load image into a surface.
	SDL_Surface *sprite_surface = IMG_Load(image_filename);
	assert(sprite_surface);
	//create a texture from the surface. The texture is an GPU-efficient/compatible version of the surface
	//we draw textures, not surfaces.
	SDL_Texture *sprite_texture = SDL_CreateTextureFromSurface(Game::renderer, sprite_surface);
	//free the surface as it is not needed anymore
	SDL_FreeSurface(sprite_surface);

	unsigned char prev_key_state[256];
	unsigned char *keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	unsigned int last_sprite_time = SDL_GetTicks();;
	int image_src_x = 0;
	int image_src_y = 0;
	float sprite_x = Game::screen_width / 2;;

	int menu_active = 0;
	int current_menu_item = 0;
	int num_menu_items = 8;
	bool done = false;
	while (!done)
	{
		unsigned int current_time = SDL_GetTicks();

		memcpy(prev_key_state, keys, 256);

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}

		//GAME CODE

		//input
		{
			//directional
			if (keys[SDL_SCANCODE_W] && !prev_key_state[SDL_SCANCODE_W])
			{
				image_src_y = 96;
			}
			if (keys[SDL_SCANCODE_D] && !prev_key_state[SDL_SCANCODE_D])
			{
				image_src_y = 64;
			}
			if (keys[SDL_SCANCODE_S] && !prev_key_state[SDL_SCANCODE_S])
			{
				image_src_y = 0;
			}
			if (keys[SDL_SCANCODE_A] && !prev_key_state[SDL_SCANCODE_A])
			{
				image_src_y = 32;
			}

			//select, enter, and quit buttons
			if (keys[SDL_SCANCODE_Y] && !prev_key_state[SDL_SCANCODE_Y])
			{
				menu_active = 1;
				current_menu_item += 1;
				if (current_menu_item > num_menu_items)
				{
					current_menu_item = 1;
				}
			}
			if (keys[SDL_SCANCODE_H] && !prev_key_state[SDL_SCANCODE_H])
			{
				//select function
			}
			if (keys[SDL_SCANCODE_N] && !prev_key_state[SDL_SCANCODE_N])
			{
				current_menu_item = 0;
				menu_active = 0;
			}
		}

		//animate sprite
		if (current_time - last_sprite_time > 100)
		{
			last_sprite_time = current_time;
			image_src_x += 32;
			if (image_src_x >= 96)//max frames, wrapping 
			{
				image_src_x = 0;
			}

			if (image_src_y == 32)
			{
				sprite_x -= 5;
			}
			if (image_src_y == 64)
			{
				sprite_x += 5;
			}
		}

		//menu
		int menu_item_x = 0;
		int menu_item_y = 0;
		int menu_src_x = 0;
		int menu_src_y = 0;
		if (menu_active)
		{	
			//set x pos
			if (current_menu_item < 5)
			{
				menu_item_x = (current_menu_item - 1) * Game::screen_width / 4;
			}
			else
			{
				menu_item_x = (current_menu_item - 5) * Game::screen_width / 4;
			}

			//set y pos
			if (current_menu_item > 4)
			{
				menu_item_y = 3 * Game::screen_height / 4;
			}
		}

		//RENDER
		//set color to black
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
		//clear screen with black
		SDL_RenderClear(Game::renderer);

		//render menu
		if (menu_active)
		{
			SDL_Rect menu_src;
			menu_src.x = menu_src_x;
			menu_src.y = menu_src_y;
			menu_src.w = 32;
			menu_src.h = 32;

			//define the rectangle you are copying TO on the SCREENw
			//you can set width and height to anything you want, the GPU will properly scale your tile
			SDL_Rect menu_dest;
			menu_dest.x = menu_item_x;
			menu_dest.y = menu_item_y;
			menu_dest.w = 32;
			menu_dest.h = 32;

			//set transparency of the texture.
			//0 to 255, 255 is opaque
			SDL_SetTextureAlphaMod(sprite_texture, 255);

			//copy from source texture to destination screen.
			//you can change the rotation angle and provide a pivot point for rotations.
			//SDL_FLIP_XXX enumeration allows you to mirror the image
			SDL_RenderCopyEx(Game::renderer, sprite_texture, &menu_src, &menu_dest, 0, NULL, SDL_FLIP_NONE);
		}

		//sprite
		{
			//define the rectangle you are copying FROM the texture
			//in this case, it is a 108 by 140 pixel box in the top left corner of the image
			SDL_Rect src;
			src.x = image_src_x;
			src.y = image_src_y;
			src.w = 32;//was 108 x 140
			src.h = 32;

			//define the rectangle you are copying TO on the SCREENw
			//you can set width and height to anything you want, the GPU will properly scale your tile
			SDL_Rect dest;
			dest.x = sprite_x;
			dest.y = Game::screen_height / 2 -16;
			dest.w = 32;
			dest.h = 32;

			//set transparency of the texture.
			//0 to 255, 255 is opaque
			SDL_SetTextureAlphaMod(sprite_texture, 255);

			//copy from source texture to destination screen.
			//you can change the rotation angle and provide a pivot point for rotations.
			//SDL_FLIP_XXX enumeration allows you to mirror the image
			SDL_RenderCopyEx(Game::renderer, sprite_texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);
		}

		//flip buffers
		//getchar()
		SDL_RenderPresent(Game::renderer);
	}
	return 0;
}