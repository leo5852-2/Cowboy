#define _CRT_SECURE_NO_WARNINGS

#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define wallSize 80

SceneID gamestage, background;
ObjectID wall[72], wallX[72], wallY[72], player, enemyIcon[3], enemy_stand[3], enemy_move[3], enemy_shoot[3], blast, wound;
TimerID timer1, standTimer, moveTimer, deathTimer;
 
int playerX = 100, playerY = 120, enemyIconX[3] = { 520, 120, 320 }, enemyIconY[3] = { 200, 360, 540 }, woundX[2] = {390, 250}, stage = -1;
bool Wpress = 0, Apress = 0, Spress = 0, Dpress = 0, moveable = 1, win = 0, fighting = 0;
float moveTime[3] = {1.0f, 0.5f, 0.3f};

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
		startTimer(timer1);
	}
	else if (code == KeyCode::KEY_W && state == KeyState::KEY_RELEASED)
	{
		Wpress = 0;
	}
	
	else if (code == KeyCode::KEY_A && state == KeyState::KEY_PRESSED)
	{
		Apress = 1;
		startTimer(timer1);
	}
	else if (code == KeyCode::KEY_A && state == KeyState::KEY_RELEASED)
	{
		Apress = 0;
	}
	
	else if (code == KeyCode::KEY_S && state == KeyState::KEY_PRESSED)
	{
		Spress = 1;
		startTimer(timer1);
	}
	else if (code == KeyCode::KEY_S && state == KeyState::KEY_RELEASED)
	{
		Spress = 0;
	}
	
	else if (code == KeyCode::KEY_D && state == KeyState::KEY_PRESSED)
	{
		Dpress = 1;
		startTimer(timer1);
	}
	else if (code == KeyCode::KEY_D && state == KeyState::KEY_RELEASED)
	{
		Dpress = 0;
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
	if (object == enemy_move[stage]) 
	{
		stopTimer(moveTimer);
		locateObject(wound, background, woundX[stage], 350);
		showObject(wound);
		showMessage("YOU WIN!");
		win = 1;
		setTimer(deathTimer, 1.0f);
		startTimer(deathTimer);
	}
}

void fight()
{
	fighting = 1;
	
	srand(time(NULL));
	int t = rand() % 5 + 3;
	
	setTimer(standTimer, (float)t);
	startTimer(standTimer);
}

void enterStage(int stage)
{
	enterScene(background);
	showObject(enemy_stand[stage]);
}

void clearStage() 
{
	for (int a = 0; a < 3; a++)
	{
		hideObject(enemy_move[a]);
		hideObject(enemy_shoot[a]);
		hideObject(wound);
	}
}

void detect()
{
	for (int i = 0; i < 3; i++)
	{
		if (playerX == enemyIconX[i] && playerY == enemyIconY[i] && fighting == 0 && stage == i - 1)
		{
			stopTimer(timer1);
			moveable = 0;
			stage = i;
			
			clearStage();
			enterStage(stage);
			fight();
		}
	}
}

void lose()
{
	showMessage("YOU LOST!");
}

void timerCallback(TimerID timer)
{ 
	if (timer == timer1)
	{
		locateObject(player, gamestage, playerX, playerY);
		detect();
		setTimer(timer, 0.01f);
		startTimer(timer);
		
		if (moveable == 1)
		{
			if (Wpress == 1) playerY = playerY + 2;
			else if (Apress == 1) playerX = playerX - 2;
			else if (Spress == 1) playerY = playerY - 2;
			else if (Dpress == 1) playerX = playerX + 2;
		}
	}
	else if (timer == standTimer)
	{
		hideObject(enemy_stand[stage]);
		showObject(enemy_move[stage]);
		setTimer(moveTimer, moveTime[stage]);
		startTimer(moveTimer);
	}
	else if (timer == moveTimer)
	{
		hideObject(enemy_move[stage]);
		showObject(enemy_shoot[stage]);
		setTimer(deathTimer, 1.0f);
		startTimer(deathTimer);
	}
	else if (timer == deathTimer)
	{
		if(win)
		{
			hideObject(enemyIcon[stage]);
			fighting = 0;
			moveable = 1;
			win = 0;
			enterScene(gamestage);
		}
		else
		{
			lose();
		}
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
	setMouseCallback(mouseCallback);

	gamestage = createScene("");
	background = createScene("background", "object/background.png");

	player = createObject("object/player_icon.png", gamestage, playerX, playerY, true);
	
	char stand[50], move[50], shoot[50];
	for (int i = 0; i < 3; i++)
	{
		sprintf(stand, "object/enemy%d_stand.png", i);
		sprintf(move, "object/enemy%d_move.png", i);
		sprintf(shoot, "object/enemy%d_shoot.png", i);
		enemyIcon[i] = createObject("object/enemy_icon1.png", gamestage, enemyIconX[i], enemyIconY[i], true);
		enemy_stand[i] = createObject(stand, background, 150, 200, false);
		enemy_move[i] = createObject(move, background, 150, 200, false);
		enemy_shoot[i] = createObject(shoot, background, 150, 200, false);
		blast = createObject("object/blast.png");
		wound = createObject("object/wound.png");
	}
		
	Mapping();
	
	timer1 = createTimer(0.01f);
	standTimer = createTimer(1.0f); 
	moveTimer = createTimer(1.0f);
	deathTimer = createTimer(1.0f);

	startGame(gamestage);

}