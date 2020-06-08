#include "SDL2-2.0.8\include\SDL.h"

#pragma comment(lib, "SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib, "SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(linker, "/subsystem:console")
#include <iostream> 

using namespace std;

SDL_Renderer *renderer = NULL;

struct Data
{
	float pos_x, pos_y, speed_x, speed_y;
};

void init(Data *d)
{
	d->pos_x = rand() % 100;
	d->pos_y = rand() % 100;
	d->speed_x = 1.0*rand() / RAND_MAX;
	d->speed_y = 1.0*rand() / RAND_MAX;
}

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("simpleSDL", 30, 30, 800, 600, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	int r = 255;
	int g = 0;
	int b = 0;

	float width = 8;
	float height = 8;
	int screen_w = 800;
	int screen_h = 600;

	int n_data = 50000;
	Data *data = new Data[n_data];

	for (int i = 0; i < n_data; i++)
	{
		init(&data[i]);
	}

	unsigned int t1;
	int frame_counter = 0;
	t1 = SDL_GetTicks();

	bool done = false;
	while (done == false)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}

		frame_counter++;
		if (frame_counter >= 1000)
		{
			unsigned int t2 = SDL_GetTicks();
			double avg_frame_time_in_milliseconds = (double)(t2 - t1) / frame_counter;
			double avg_frame_time_in_seconds = avg_frame_time_in_milliseconds / 1000.0;

			cout << "single frame render time in seconds: " << avg_frame_time_in_seconds << endl;
			cout << "fps: " << 1.0 / avg_frame_time_in_seconds << endl;

			frame_counter = 0;
			t1 = t2;
		}

		for (int i = 0; i < n_data; i++)
		{
			data[i].pos_x += data[i].speed_x;
			data[i].pos_y += data[i].speed_y;
			if (data[i].pos_x > screen_w || data[i].pos_x < 0) data[i].speed_x - 1;
			if (data[i].pos_y > screen_h || data[i].pos_y < 0) data[i].speed_y - 1;
		}

		SDL_SetRenderDrawColor(renderer, 50, 50, 50, 0);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		for (int i = 0; i < n_data; i++)
		{
			SDL_Rect rect = { data[i].pos_x, data[i].pos_y, width, height };
			SDL_RenderFillRect(renderer, &rect);
		}

		SDL_RenderPresent(renderer);
	}
	return 0;
}