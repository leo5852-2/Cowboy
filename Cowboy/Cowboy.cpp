#include <bangtal.h>
#include <stdio.h>
#define wallSize 80

SceneID gamestage, backgound;
ObjectID wall[72], wallX[72], wallY[72], player, enemyIcon[3], enemy[3];
TimerID timer;
 
int playerX = 100, playerY = 120, enemyIconX[3] = {520, 120, 320}, enemyIconY[3] = {200, 360, 640}, stage = 10;
bool Wpress = 0, Apress = 0, Spress = 0, Dpress = 0, moveable = 1;

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);

	locateObject(object, scene, x, y);

	if (shown) {
		showObject(object);
	}
	return object;
}

void keyboardCallback(KeyCode code, KeyState state) 
{
	if (code == KeyCode::KEY_W && state == KeyState::KEY_PRESSED)
	{
		Wpress = 1;
		startTimer(timer);
	}
	else if (code == KeyCode::KEY_W && state == KeyState::KEY_RELEASED)
	{
		Wpress = 0;
	}
	
	else if (code == KeyCode::KEY_A && state == KeyState::KEY_PRESSED)
	{
		Apress = 1;
		startTimer(timer);
	}
	else if (code == KeyCode::KEY_A && state == KeyState::KEY_RELEASED)
	{
		Apress = 0;
	}
	
	else if (code == KeyCode::KEY_S && state == KeyState::KEY_PRESSED)
	{
		Spress = 1;
		startTimer(timer);
	}
	else if (code == KeyCode::KEY_S && state == KeyState::KEY_RELEASED)
	{
		Spress = 0;
	}
	
	else if (code == KeyCode::KEY_D && state == KeyState::KEY_PRESSED)
	{
		Dpress = 1;
		startTimer(timer);
	}
	else if (code == KeyCode::KEY_D && state == KeyState::KEY_RELEASED)
	{
		Dpress = 0;
	}
}

void enterStage(int stage)
{
	backgound = createScene("background", "object/background.png");
	enterScene(backgound);
}

void detect()
{
	for (int i = 0; i < 3; i++)
	{
		if (enemyIconX[i] - 10 <= playerX <= enemyIconX[i] + 10 && enemyIconY[i] - 10 <= playerY <= enemyIconY[i] + 10)
		{
			moveable = 0;
			stage = i;
			enterStage(stage);
		}
	}
}

void timerCallback(TimerID timer)
{
	locateObject(player, gamestage, playerX, playerY);
	setTimer(timer, 0.01f);
	startTimer(timer);
	detect();

	if (moveable == 1)
	{
		if (Wpress == 1) playerY = playerY + 2;
		else if (Apress == 1) playerX = playerX - 2;
		else if (Spress == 1) playerY = playerY - 2;
		else if (Dpress == 1) playerX = playerX + 2;
	}
}

void Mapping() 
{
	for (int i = 0; i < 72; i++) 
	{
		wallX[i] = (i % 8) * 80;
		wallY[i] = 640 - (i / 8) * 80;
		wall[i] = createObject("object/wall.png", gamestage, wallX[i], wallY[i], false);
	}
	
	int x, y, mapX = 8, mapY = 9,
		map[] =
	{
	1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,1,
	1,1,1,0,0,1,1,1,
	1,0,0,0,0,0,0,1,
	1,0,1,1,1,1,1,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1
	};

	for (x = 0; x < mapX; x++)
	{
		for (y = 0; y < mapY; y++)
		{
			if (map[mapX * y + x] == 1)
			{
				showObject(wall[mapX * y + x]);
			}
		}
	}
}

int main() 
{
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setKeyboardCallback(keyboardCallback);
	setTimerCallback(timerCallback);

	gamestage = createScene("");
	
	player = createObject("object/player_icon.png", gamestage, playerX, playerY, true);
	enemyIcon[0] = createObject("object/enemy_icon1.png", gamestage, enemyIconX[0], enemyIconY[0], true);
	enemyIcon[1] = createObject("object/enemy_icon1.png", gamestage, enemyIconX[1], enemyIconY[1], true);
	enemyIcon[2] = createObject("object/enemy_icon1.png", gamestage, enemyIconX[2], enemyIconY[2], true);
	
	Mapping();
	
	timer = createTimer(0.01f);

	startGame(gamestage);

}