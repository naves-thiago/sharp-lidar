#include <stdbool.h>
#include <SDL2/SDL.h>

struct game_t {
	SDL_Window   *win;
	SDL_Renderer *ren;
};
typedef struct game_t Game;

void game_init(Game *me, int w, int h, bool fs);
void game_fini(Game *me);

static int watch_for_io(void *data);
static void signal_update_in_dataset();
static SDL_Point make_dataset_point(float angle_in_deg, float sample);

#define DATASET_SIZE (sizeof dataset / sizeof *dataset)
SDL_Point dataset[360];

int main(int argc, const char *argv[])
{
	Game me;
	game_init(&me, 800, 800, false);
	printf(
			"controls:\n"
			"\tq - quit\n");

	// TODO: spin a thread to watch for IO
	SDL_CreateThread(watch_for_io, "IO", 0);

	for (int i=0; i<360; ++i) {
		int sample = rand() % 100 + 100;
		dataset[i] = make_dataset_point(i, sample);
	}

	while (true) {
		SDL_Event event;
		SDL_WaitEvent(&event);

		switch (event.type) {
		case SDL_KEYDOWN: {
			SDL_Keysym *key = &event.key.keysym;
			if (key->sym == SDLK_q) goto finalize_program;
			break;
		}
		case SDL_USEREVENT: { // updated dataset
		}
		default:
			break;
		}

		SDL_SetRenderDrawColor(me.ren, 0, 0, 0, 255);
		SDL_RenderClear(me.ren);

		SDL_SetRenderDrawColor(me.ren, 255, 0, 255, 255);
		SDL_RenderDrawLines(me.ren, dataset, DATASET_SIZE);
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

// must obey SDL_CreateThread signature
static int watch_for_io(void *data)
{
	char buf[256];

	while (true) {
		float angle_in_deg;
		float sample;

		fgets(buf, sizeof buf, stdin);
		sscanf(buf, " %f %f",  &angle_in_deg,&sample);
		angle_in_deg = fmod(angle_in_deg, 360);

		printf("echo: %f %f\n", angle_in_deg, sample);

		dataset[(int)angle_in_deg] = make_dataset_point(angle_in_deg, sample);
		signal_update_in_dataset();
	}
	return 0;
}

static void signal_update_in_dataset()
{
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
}

static SDL_Point make_dataset_point(float angle_in_deg, float sample)
{
	SDL_Point p;
	float angle_in_rad = 2*M_PI*(angle_in_deg/360);
	p.x = sample*sin(angle_in_rad) + 400;
	p.y = sample*cos(angle_in_rad) + 400;

	return p;
}

