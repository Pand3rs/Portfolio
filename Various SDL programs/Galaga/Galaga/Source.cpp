#include "SDL2-2.0.8\include\SDL.h"
#pragma comment (lib, "SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment (lib, "SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment (linker, "/subsystem:console")
#include <time.h>
#include <iostream>

using namespace std;

SDL_Renderer *renderer = NULL;

class Game_Object
{
protected:
		float x, y, w, h;
		float sx, sy;
		int r, g, b, upgrade;			 
		bool is_active;

public:

	Game_Object()
	{
		is_active = false;
	}

	Game_Object(int _r, int _g, int _b)
	{
		r = _r;
		g = _g;
		b = _b;
		is_active = false;
	}
	void set_R(float _r) { r = _r; }
	void set_G(float _g) { g = _g; }
	void set_B(float _b) { b = _b; }
	void set_X(float _x) { x = _x; }
	void set_Y(float _y) { x = _y; }
	float get_X() { return x; }
	float get_Y() { return y; }
	float get_W() { return w; }
	float get_H() { return h; }

	void set_Size(float _w, float _h)
	{
		w = _w;
		h = _h;
	}

	void set_upgrade(int upg)		
	{
		upgrade = upg;
	}

	void set_Position(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	void set_Speed(float _sx, float _sy)
	{
		sx = _sx;
		sy = _sy;
	}

	void activate()
	{
		is_active = true;
	}

	void deactivate()
	{
		is_active = false;
	}

	bool is_Active()
	{
		return is_active;
	}

	void update()
	{
		if (!is_active) return;
		x += sx;
		y += sy;

		if (x + w > 800) x = 800 - w;
		if (x < 0) x = 0;
	}

	void draw()
	{
		if (!is_active) return;
		SDL_Rect rect = { x, y, w, h };
		SDL_SetRenderDrawColor (renderer, r, g, b, 255);
		SDL_RenderFillRect (renderer, &rect);
	}

	bool is_Colliding(Game_Object *a)
	{
		if (!is_active || !a->is_active) return false;
		if (x + w < a->x) return false;					
		if (x > a->x +a->w) return false;	
		if (y + h < a->y) return false;	
		if (y > a->y + a->h) return false;
		return true;
	}
};

class Player : public Game_Object
{
public:
	Player()
	{
		r = 0;
		g = 255;
		b = 255;
		w = 50;
		h = 50;
		x = 800 / 2;
		y = 600 - h;
		is_active = true;
	}
};

class Bullet : public Game_Object
{
public:
	Bullet()
	{
		r = 255;
		g = 0;
		b = 255;
		w = 4;
		h = 8;
		sx = 0;
		sy = -0.5;
	}

	void update()
	{
		Game_Object::update();
		
		if (x + w >= 800 || x <= 0)
		{
			deactivate();
		}

		if (y + h >= 600 || y <= 0)
		{
			deactivate();
		}
	}
};

class Bullet2 : public Game_Object
{
public:
	Bullet2()
	{
		r = 255;
		g = 255;
		b = 255;
		w = 6;
		h = 6;
		sx = 0;
		sy = -0.3;
	}

	void update()
	{
		Game_Object::update();

		if (x + w >= 800 || x <= 0)
		{
			deactivate();
		}

		if (y + h >= 600 || y <= 0)
		{
			deactivate();
		}
	}
};

class Particle : public Game_Object
{
	float life;
	float decay;

public:
	Particle()
	{
		r = 0;
		g = 50;
		b = 50;
		w = 4;
		h = 4;
		life = 800;
		decay = 1;
	}

	void reload()
	{
		sx = 0.5 - (double)rand() / RAND_MAX;
		sy = 0.5 * rand() / RAND_MAX;
		r = 255.0 * rand() / RAND_MAX;
		g = 255.0 * rand() / RAND_MAX;
		sx *= 0.2;
		sy *= 0.2;
	}

	void update()
	{
		Game_Object::update();

		if (x + w >= 800 || x <= 0)
		{
			deactivate();
		}

		if (y + h >= 600 || y <= 0)
		{
			deactivate();
		}

		life -= decay;
		if (life < 0)
		{
			deactivate();
			life = 800;
		}
	}
};

class Enemy : public Game_Object
{
public:
	Enemy()
	{
		r = 0;
		g = 0;
		b = 255;
		x = 800.0 * rand() / RAND_MAX;
		y = 400.0 * rand() / RAND_MAX;
		w = 4;
		h = 8;
		sy = 0.0;

		if (rand() % 2 == 0)
		{
			sx = -0.1;
		}
		
		else
		{
			sx = 0.1;
		}
		
		is_active = true;
	}

	void update()
	{
		Game_Object::update();

		sy = 0.01 * sin(4.0 * 3.14159265 * (360.0 * x / 800.0) / 180.0);

		if (x + w >= 800 || x <= 0)
		{
			sx = -sx;
		}

		if (y + h >= 600 || y <= 0)
		{
			sy = -sy;
		}
	}
};

int main(int argc, char **argv)
{
	srand(time(0));
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("galaga", 30, 30, 800, 600, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	int max_number_of_bullets2 = 100;			
	int max_number_of_bullets = 100;
	int max_number_of_enemies = 200;
	int max_number_of_particles = 100;
	Player *player = new Player();

	Bullet **bullets = new Bullet*[max_number_of_bullets];
	for (int i = 0; i < max_number_of_bullets; i++)
	{
		bullets[i] = new Bullet();
	}

	Bullet2 **bullets2 = new Bullet2*[max_number_of_bullets2];		
	for (int i = 0; i < max_number_of_bullets2; i++)
	{
		bullets2[i] = new Bullet2();
	}

	Enemy **enemies = new Enemy*[max_number_of_enemies];
	for (int i = 0; i < max_number_of_enemies; i++)
	{
		enemies[i] = new Enemy();
	}

	Particle **particles = new Particle*[max_number_of_particles];
	for (int i = 0; i < max_number_of_particles; i++)
	{
		particles[i] = new Particle();
	}

	unsigned char *keyboard = (unsigned char*)SDL_GetKeyboardState(NULL);;
	int previous_state_spacekey = 0;
	int previous_state_ukey = 0;

	unsigned int t1;

	int frame_counter = 0;
	t1 = SDL_GetTicks();
	int u = 0;
	int bomb_counter = 800;
	bool done = false;
	while (done == false)
	{
		previous_state_spacekey = keyboard[SDL_SCANCODE_SPACE];
		previous_state_ukey = keyboard[SDL_SCANCODE_U];

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}

		//update

		player->set_Speed(0, 0);

		if (keyboard[SDL_SCANCODE_A])
		{
			player->set_Speed(-0.4, 0);
		}

		if (keyboard[SDL_SCANCODE_D])
		{
			player->set_Speed(0.4, 0);
		}

		if (previous_state_ukey == 0 && keyboard[SDL_SCANCODE_U] == 1)
		{
			u += 1;
			if (u > 1)  //# of diff bullets -1
			{
				u = 0;
			}
			player->set_upgrade(u);
		}

		if (previous_state_spacekey == 0 && keyboard[SDL_SCANCODE_SPACE] == 1)
		{
			if (u == 0)
			{
				for (int i = 0; i < max_number_of_bullets; i++)
				{
					if (bullets[i]->is_Active() == false)
					{
						bullets[i]->activate();
						bullets[i]->set_Position(player->get_X() + player->get_W() / 2, player->get_Y());
						break;
					}
				}
			}

			if (u == 1)
			{
				for (int i = 0; i < max_number_of_bullets2; i++)
				{
					if (bullets2[i]->is_Active() == false)
					{
						bullets2[i]->activate();
						bullets2[i]->set_Position(player->get_X() + player->get_W() / 2, player->get_Y());
						break;
					}
				}
			}
		}

		player->update();

		for (int i = 0; i < max_number_of_bullets; i++)
		{
			bullets[i]->update();
		}

		for (int i = 0; i < max_number_of_bullets2; i++)		
		{
			bullets2[i]->update();
		}

		for (int i = 0; i < max_number_of_enemies; i++)
		{
			enemies[i]->update();
		}

		for (int i = 0; i < max_number_of_particles; i++)
		{
			particles[i]->update();
		}
		
		for (int i = 0; i < max_number_of_bullets; i++)
		{
			for (int j = 0; j < max_number_of_enemies; j++)
			{
				if (bullets[i]->is_Colliding(enemies[j]))
				{
					bullets[i]->deactivate();
					enemies[j]->deactivate();
					int number_of_particles_to_activate = 5;
					for (int k = 0; k < max_number_of_particles; k++)
					{
						if (number_of_particles_to_activate <= 0) break;
						if (particles[k]->is_Active() == false)
						{
							particles[k]->activate();
							particles[k]->set_Position(bullets[i]->get_X(), bullets[i]->get_Y());
							particles[k]->reload();
							number_of_particles_to_activate--;
						}
					}
				} 
			}
			
		}

		for (int i = 0; i < max_number_of_bullets2; i++)		
		{
			for (int j = 0; j < max_number_of_enemies; j++)
			{
				if (bullets2[i]->is_Colliding(enemies[j]))
				{
					
					for (int c = 0; c < bomb_counter; c++)
					{
						int w = bullets2[i] ->get_W();
						int h = bullets2[i] ->get_H();
						if (c <= bomb_counter / 2)
						{
							bullets2[i]->set_Size(w += 1, h += 1);
						}
						if (c >bomb_counter /2)
						{
							bullets2[i]->set_Size(w -= 1, h -= 1);
						}
						if (c == bomb_counter)
						{
							bullets2[i]->deactivate();
						}
						cout << c;
					}	
					enemies[j]->deactivate();
				}
			}

		}

		//draw
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		player->draw();
		for (int i = 0; i < max_number_of_bullets; i++)
		{
			bullets[i] -> draw();
		}

		for (int i = 0; i < max_number_of_bullets2; i++)		
		{
			bullets2[i]->draw();
		}

		for (int i = 0; i < max_number_of_enemies; i++)
		{
			enemies[i] -> draw();
		}

		for (int i = 0; i < max_number_of_particles; i++)
		{
			particles[i] -> draw();
		}

		SDL_RenderPresent(renderer);
	}

	return 0;
}

