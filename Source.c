#include"raylib.h"
#include <stdio.h>
#include <time.h>

#define tileWidth 100
#define playerRadius 40

// Define a structure Tile which consist of items related to tile.
typedef struct Tile
{
	int index;
	int drawX;
	int drawY;
	bool playerOn;
	bool stop;
}Tile;

// Define a structure Player which consist of items related to player.
typedef struct Player
{
	int indexOn;
	int team;
}Player;

// Setting the initials.
Player players[2];
Tile tiles[59];
int hoverIndex = -1;
int toMove = -1;
int rolled = -1;
char *strRolled[1];
bool isRolled = false;
int turn = 0;

// Function to define the position of tiles.
void initializeTiles()
{
	int dy = 0;
	int dx = 1;
	int drawX = 100;
	int drawY = 0;
	int step = tileWidth;
	int stop = 0;
	for (int i = 0; i < 59; i++)
	{
		if ((i - 5) % 12 == 0 || ((i + 1) % 12 == 0))
		{
			stop = 1;
		}
		else stop = 0;
		if ((i + 2) % 12 == 0)
		{
			dy = 1;
			dx = 0;
		}
		if (i % 12 == 0)
		{
			dy = 0;
			if (drawX == 100) dx = 1;
			else dx = -1;
		}
		tiles[i] = (Tile){ i, drawX, drawY, 0, stop };
		drawX += dx * step;
		drawY += dy * step;
	}
}

// Function to draw tile.
void drawTiles()
{
	for (int i = 0; i < 59; i++)
	{
		DrawRectangle(tiles[i].drawX, tiles[i].drawY, tileWidth, tileWidth, tiles[i].stop ? RED : BLACK );
		DrawRectangleLines(tiles[i].drawX, tiles[i].drawY, tileWidth, tileWidth, WHITE);
	}
}

// Funtion to draw players on board.
void drawPlayers()
{
	for (int player = 0; player < 2; player++)
	{
		int drawX = tiles[players[player].indexOn].drawX + tileWidth / 2;
		int drawY = tiles[players[player].indexOn].drawY + tileWidth / 2;
		DrawCircle(drawX, drawY, playerRadius, players[player].team ? BLUE : YELLOW);
		DrawCircleLines(drawX, drawY, playerRadius, WHITE);

	}
}

// Function to assigned path to players.
void initializePlayers()
{
	players[0] = (Player){ 0, 0 };
	players[1] = (Player){ 58, 1 };
}

// Function to change the turn of player.
void changeTurn()
{
	turn = (turn + 1) % 2;
}

// Funtion to tell player in which direction to move.
int getDirection(int team)
{
	return (team * -2) + 1;
}

// Function to hover player if on cursor in it.
void drawHover()
{
	int drawX = tiles[players[hoverIndex].indexOn].drawX + tileWidth / 2;
	int drawY = tiles[players[hoverIndex].indexOn].drawY + tileWidth / 2;
	DrawCircle(drawX, drawY, playerRadius, WHITE);
	DrawCircleLines(drawX, drawY, playerRadius, players[hoverIndex].team ? BLUE : YELLOW);
}

// Funtion to roll a dice.
void roll(Player p)
{
	rolled = (rand() % 6) + 1;
	switch (rolled)
	{
	case 1: strRolled[0] = "1"; break;
	case 2: strRolled[0] = "2"; break;
	case 3: strRolled[0] = "3"; break;
	case 4: strRolled[0] = "4"; break;
	case 5: strRolled[0] = "5"; break;
	case 6: strRolled[0] = "6"; break;
	default: strRolled[0] = "0";
	}
	isRolled = true;
	toMove = hoverIndex;
}

// Funtion to move player.
void move(int index)
{
	int direction = getDirection(players[index].team);
	if (isRolled)
	{
		tiles[players[index].indexOn].playerOn = false;
		players[index].indexOn += rolled * direction;
		if (!tiles[players[index].indexOn].stop && tiles[players[index].indexOn].playerOn)
		{
			players[(turn + 1) % 2].indexOn = ((turn + 1) % 2) * 58;
		}
		tiles[players[index].indexOn].playerOn = true;
	}
	isRolled = false;
	toMove = -1;
	changeTurn();
}

// Function to move player if dice is roll and index is hoverd.
void mouseClicked()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoverIndex != -1)
	{
		if (players[hoverIndex].team == turn)
			if (isRolled && hoverIndex == toMove)
				move(hoverIndex);
			else roll(players[hoverIndex]);
	}
}


// Function to check that player is on tile or not.
void checkHover()
{
	hoverIndex = -1;
	for (int player = 0; player < 2; player++)
	{
		int drawX = tiles[players[player].indexOn].drawX + tileWidth / 2;
		int drawY = tiles[players[player].indexOn].drawY + tileWidth / 2;
		if (CheckCollisionPointCircle(GetMousePosition(), (Vector2) { drawX, drawY }, playerRadius)) 
			hoverIndex = player;
	}
	if (hoverIndex != turn)
	{
		int drawX = tiles[players[turn].indexOn].drawX + tileWidth / 2;
		int drawY = tiles[players[turn].indexOn].drawY + tileWidth / 2;
		if (CheckCollisionPointCircle(GetMousePosition(), (Vector2) { drawX, drawY }, playerRadius)) 
			hoverIndex = turn;
	}
}

// Main function 
int main()
{
	srand(time(0));
	InitWindow(1300, 900, "project");
	initializeTiles();
	initializePlayers();
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(GRAY);
		drawTiles();
		drawPlayers();
		checkHover();
		mouseClicked();
		if (hoverIndex != -1) drawHover();
		if (isRolled && hoverIndex != -1 && toMove == hoverIndex) 
			DrawText(strRolled[0], tiles[players[hoverIndex].indexOn].drawX + playerRadius, tiles[players[hoverIndex].indexOn].drawY + playerRadius*0.90, 35, BLACK);
		EndDrawing();
	}
	CloseWindow();
}