#include <stdbool.h>
#include <SDL2/SDL.h>

struct game_t {
	SDL_Window   *win;
	SDL_Renderer *ren;
};
typedef struct game_t Game;

void game_init(Game *me, int w, int h, bool fs);
void game_fini(Game *me);


int main(int argc, const char *argv[])
{
	Game me;
	game_init(&me, 800, 800, false);
	printf(
			"controls:\n"
			"\tq - quit\n");

	SDL_Point pts[256];
	for (int i=0; i<256; ++i) {
		int sample = rand() % 100 + 100;
		float angle = 2*M_PI*i/256.0;
		pts[i].x = sample*sin(angle) + 400;
		pts[i].y = sample*cos(angle) + 400;
	}

	while (true) {
		SDL_Event event;
		SDL_WaitEvent(&event);

		switch (event.type) {
		case SDL_KEYDOWN: {
			SDL_Keysym *key = &event.key.keysym;
			if (key->sym == SDLK_q) goto finalize_program;
			break;
		} default:
			break;
		}
		SDL_SetRenderDrawColor(me.ren, 255, 0, 255, 255);
		SDL_RenderDrawLines(me.ren, pts, 256);
		SDL_RenderPresent(me.ren);
	}

finalize_program:
	game_fini(&me);
	return 0;
}

/*--------------------------------------------------------------------------- */
void game_init(Game *me, int w, int h, bool fs)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	me->win = SDL_CreateWindow("sonar",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			w, h,
			SDL_WINDOW_OPENGL
			| SDL_WINDOW_SHOWN
			| (fs ? SDL_WINDOW_FULLSCREEN : 0));
	me->ren = SDL_CreateRenderer(me->win,
			-1,
			SDL_RENDERER_ACCELERATED);
}

void game_fini(Game *me)
{
    SDL_DestroyWindow(me->win);
    SDL_Quit();
}
