#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#define SNAKE_MAX_LENGTH 20
#define SNAKE_HEAD 'H'
#define SNAKE_BODY 'X'
#define BLANK_CELL ' '
#define WALL_CELL '*'
#define SNAKE_FOOD '$'

//  -1表示向上(dy)或者向左(dy)  +1表示向下(dy)或者向右(dx)
void snakeMove(int, int);
//随机投放食物 
void put_money(void);
//输出二维数组图形 
void output(void);
//游戏结束 
void gameover(void);
//吃食物
int eat_money(int dx, int dy);
//自动寻找
char wheregonext(int hx, int hy, int fx, int fy);

char map[12][12] =
{ "***********",
"*XXXXH    *",
"*         *",
"*         *",
"*         *",
"*         *",
"*         *",
"*         *",
"*         *",
"*         *",
"*         *",
"***********" };
int snakeX[SNAKE_MAX_LENGTH] = { 1,2,3,4,5 };
int snakeY[SNAKE_MAX_LENGTH] = { 1,1,1,1,1 };
int moneyX, moneyY;
int snakeLength = 5;
int continuegame = 1;


int main() {
	char ch = 0;
	put_money();                               //  首先投放食物 
	output();
	while (continuegame == 1) {                   //  每次循环都要判断是否游戏已经结束
		ch = wheregonext(snakeX[snakeLength - 1], snakeY[snakeLength - 1], moneyX, moneyY);
		printf("\033[2J");
		switch (ch) {
		case 'A':
		case 'a':
			snakeMove(-1, 0);		//  向左移动 
			break;
		case 'D':
		case 'd':
			snakeMove(1, 0);		//  向右移动 
			break;
		case 'W':
		case 'w':
			snakeMove(0, -1);		//  向上移动 
			break;
		case 'S':
		case 's':
			snakeMove(0, 1);		//  向下移动 
			break;
		default: break;
		}
		output();
		gameover();
		sleep(1);
	}
	printf("Game Over");
}

void snakeMove(int dx, int dy) {
	int i;
	//判断是否吃到食物
	if (eat_money(dx, dy));
	else {
		map[snakeY[0]][snakeX[0]] = BLANK_CELL;
		//  如果不是，那么以前的最后一节变成空白
		map[snakeY[snakeLength - 1]][snakeX[snakeLength - 1]] = SNAKE_BODY;
		//  以前的头变成身子
		for (i = 0; i < snakeLength - 1; i++) {
			//  以前所有的坐标都向前移动
			snakeX[i] = snakeX[i + 1];
			snakeY[i] = snakeY[i + 1];
		}
		snakeX[snakeLength - 1] += dx;
		snakeY[snakeLength - 1] += dy;
		map[snakeY[snakeLength - 1]][snakeX[snakeLength - 1]] = SNAKE_HEAD;
		//  新的头
	}
}

void put_money(void) {
	int i;
	srand(time(NULL));
	moneyX = rand() % 9 + 1;
	//  钱的横纵坐标随机产生
	srand(time(NULL));
	moneyY = rand() % 9 + 1;
	//  如果钱砸身子上了，就换
	for (i = 0; i < snakeLength; i++) {
		while (moneyX == snakeX[i] && moneyY == snakeY[i]) {
			srand(time(NULL));
			moneyX = rand() % 9 + 1;
			srand(time(NULL));
			moneyY = rand() % 9 + 1;
			i = -1;
		}
	}
	map[moneyY][moneyX] = '$';
}

void output(void) {
	int i, j;
	for (i = 0; i < 12; ++i) {
		for (j = 0; j < 12; ++j) {
			printf("%c", map[i][j]);
		}
		putchar('\n');
	}
}

void gameover(void) {
	if (snakeX[snakeLength - 1] == 0 || snakeX[snakeLength - 1] == 10
		|| snakeY[snakeLength - 1] == 0 || snakeY[snakeLength - 1] == 10) {
		continuegame = 0;
		printf("撞到墙了！");
	}
	int i;
	for (i = 0; i < snakeLength - 1; i++) {
		//  以前所有的坐标都向前移动
		if (snakeX[i] == snakeX[snakeLength - 1] &&
			snakeY[i] == snakeY[snakeLength - 1]) {
			printf("咬到自己了！");
			continuegame = 0;
		}
	}
	if (SNAKE_MAX_LENGTH == snakeLength) {
		continuegame = 0;
		printf("congratulations！\twin!\n");
	}
}

int eat_money(int dx, int dy) {
	if (snakeX[snakeLength - 1] + dx == moneyX && snakeY[snakeLength - 1] + dy == moneyY) {
		//  判断是否吃到了money，即两者坐标是否完全相等
		snakeLength++;
		//  如果是，长度加 1
		snakeX[snakeLength - 1] = snakeX[snakeLength - 2] + dx;
		//  头坐标向前移动，表示头向前走了一步
		snakeY[snakeLength - 1] = snakeY[snakeLength - 2] + dy;
		map[snakeY[0]][snakeX[0]] = SNAKE_BODY;
		//  因为新生了一节，所以以前的最后一节依然是SNAKE_BODY
		map[snakeY[snakeLength - 2]][snakeX[snakeLength - 2]] = SNAKE_BODY;
		//  以前的头变成了身子
		map[snakeY[snakeLength - 1]][snakeX[snakeLength - 1]] = SNAKE_HEAD;
		//  现在的头
		put_money();
		//  再生一个'$'
		return 1;
	}
	else return 0;
}
//用数组distance[4]={0,0,0,0} 记录离食物的距离
char wheregonext(int hx, int hy, int fx, int fy) {// Hx,Hy: 头的位置
	// Fx,Fy：食物的位置
	char p = 0;
	int min = 9999;
	int distance[4];
	distance[0] = abs(fx - (hx - 1)) + abs(fy - hy);
	distance[1] = abs(fx - (hx + 1)) + abs(fy - hy);
	distance[2] = abs(fx - hx) + abs(fy - (hy - 1));
	distance[3] = abs(fx - hx) + abs(fy - (hy + 1));
	// 分别计算蛇头周边四个位置到食物的距离。H头的位置，F食物位置
	if (distance[0] <= min && (map[hy][hx - 1] == ' ' || map[hy][hx - 1] == '$')) {
		min = distance[0];
		p = 'a';
	}
	if (distance[1] <= min && (map[hy][hx + 1] == ' ' || map[hy][hx + 1] == '$')) {
		min = distance[1];
		p = 'd';
	}
	if (distance[2] <= min && (map[hy - 1][hx] == ' ' || map[hy - 1][hx] == '$')) {
		min = distance[2];
		p = 'w';
	}
	if (distance[3] <= min && (map[hy + 1][hx] == ' ' || map[hy + 1][hx] == '$')) {
		min = distance[3];
		p = 's';
	}
	// 选择distance中存最小距离的下标p，最小距离不能是9999
	return p;

}