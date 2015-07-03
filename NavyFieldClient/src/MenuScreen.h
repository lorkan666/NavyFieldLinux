#include "GameScreen.h"
#include <string>
using namespace std;

class MenuScreen :
	public GameScreen
{
public:
	MenuScreen(string name);
	void enter();
	void render(SDL_Surface *, int );
	~MenuScreen(void);
};

