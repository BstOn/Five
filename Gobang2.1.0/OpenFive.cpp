#include "Game.h"
#include <memory>
#include <iostream>
#include <ctime> 
Game::Game()
{
	memset(chess, 0, sizeof(chess));
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			chess[i][j].flag = 3;
	player = 0;
	dafou = false;
	waitThread = false;
	depthkongzhi = false;
	waitBackAI = false;
	initiChessType();
	InitializeHashKey();
}

bool Game::change()
{
	for (int i = 0; i < 26; i++)
	{
		if (kaiju(chessType[i].type[0], chessType[i].type[1], chessType[i].type[2], chessType[i].type[3], chessType[i].dotN) == 0)
			continue;
		else
		{
			kaijun = i;
			if (kaiju(chessType[i].type[0], chessType[i].type[1], chessType[i].type[2], chessType[i].type[3], chessType[i].dotN) == 1)
				return false;
			else
				return true;
		}
	}

}
int Game::sta()
{
	int temp = 0;
	for (int i = 0; i<15; i++)
		for (int j = 0; j < 15; j++)
		{
			if (chess[i][j].flag == SPACE)
				temp++;
		}
	return 225 - temp;
}

void Game::LayOut()
{
	int a;
	srand((unsigned)time(NULL));
	a = rand() % 4 + 1;
	int opening = 1;
	opening = a;
	switch (opening)
	{
	case 1:{
				kaijun = 1;
				computerPutdown(7, 7);
				peoplePutdown(7 + chessType[kaijun].type[0], 7 + chessType[kaijun].type[1]);
				computerPutdown(7 + chessType[kaijun].type[2], 7 + chessType[kaijun].type[3]);
				currentX = chessType[kaijun].type[2], currentY = chessType[kaijun].type[2];
				dadianN = dadiann = chessType[kaijun].dotN; aspect = 1;  break;//溪月
			}
	case 2:{
				kaijun = 2;
				computerPutdown(7, 7);
				peoplePutdown(7 + chessType[kaijun].type[0], 7 + chessType[kaijun].type[1]);
				computerPutdown(7 + chessType[kaijun].type[2], 7 + chessType[kaijun].type[3]);
				currentX = chessType[kaijun].type[2], currentY = chessType[kaijun].type[2];
				dadianN = dadiann = chessType[kaijun].dotN; aspect = 1;  break;//疏星
			}
	case 3:{
				kaijun = 3;
				computerPutdown(7, 7);
				peoplePutdown(7 + chessType[kaijun].type[0], 7 + chessType[kaijun].type[1]);
				computerPutdown(7 + chessType[kaijun].type[2], 7 + chessType[kaijun].type[3]);
				currentX = chessType[kaijun].type[2], currentY = chessType[kaijun].type[2];
				dadianN = dadiann = chessType[kaijun].dotN; aspect = 1;  break;//花月
			}
	case 4:{
				kaijun = 19;
				computerPutdown(7, 7);
				peoplePutdown(7 + chessType[kaijun].type[0], 7 + chessType[kaijun].type[1]);
				computerPutdown(7 + chessType[kaijun].type[2], 7 + chessType[kaijun].type[3]);
				currentX = chessType[kaijun].type[2], currentY = chessType[kaijun].type[2];
				dadianN = dadiann = chessType[kaijun].dotN; aspect = 1;  break;//浦月
			}
	}
}

int Game::kaiju(int i1, int j1, int i2, int j2, int n)
{
	judge_Aspect(i1, j1, i2, j2);
	if (aspect)
		if (dadianN <= n)
			return 1;
		else
			return 2;
	else
		return 0;
}

void Game::backChess()
{
	int x, y;
	x = chessPoint.last().x();
	y = chessPoint.last().y();
	if (player == sta())
		player--;
	chess[x][y].flag = SPACE;
	chessPoint.pop_back();
}

void Game::judge_Aspect(int i1, int j1, int i2, int j2)
{
	if (chess[7 + i1][7 + j1].flag == WHITE_CHESS && chess[7 + i2][7 + j2].flag == BLACK_CHESS)
		aspect = 1;
	else if (chess[7 - j1][7 + i1].flag == WHITE_CHESS && chess[7 - j2][7 + i2].flag == BLACK_CHESS)
		aspect = 2;
	else if (chess[7 - i1][7 - j1].flag == WHITE_CHESS && chess[7 - i2][7 - j2].flag == BLACK_CHESS)
		aspect = 3;
	else if (chess[7 + j1][7 - i1].flag == WHITE_CHESS && chess[7 + j2][7 - i2].flag == BLACK_CHESS)
		aspect = 4;
	else if (chess[7 - i1][7 + j1].flag == WHITE_CHESS && chess[7 - i2][7 + j2].flag == BLACK_CHESS)
		aspect = 5;
	else if (chess[7 + i1][7 - j1].flag == WHITE_CHESS && chess[7 + i2][7 - j2].flag == BLACK_CHESS)
		aspect = 6;
	else if (chess[7 + j1][7 + i1].flag == WHITE_CHESS && chess[7 + j2][7 + i2].flag == BLACK_CHESS)
		aspect = 7;
	else if (chess[7 - j1][7 - i1].flag == WHITE_CHESS && chess[7 - j2][7 - i2].flag == BLACK_CHESS)
		aspect = 8;
	else
		aspect = 0;
}

poinT Game::aspect_count(int i, int j)
{
	poinT point;
	switch (aspect)
	{
	case 1:point.x = 7 + i; point.y = 7 + j; break;
	case 2:point.x = 7 - j; point.y = 7 + i; break;
	case 3:point.x = 7 - i; point.y = 7 - j; break;
	case 4:point.x = 7 + j; point.y = 7 - i; break;
	case 5:point.x = 7 - i; point.y = 7 + j; break;
	case 6:point.x = 7 + i; point.y = 7 - j; break;
	case 7:point.x = 7 + j; point.y = 7 + i; break;
	case 8:point.x = 7 - j; point.y = 7 - i; break;
	}
	return point;
}

void Game::Nda(double x1[15], double y1[15], int n)
{//给N个打点
	double x, y;
	bool jud = Judge_IsSymmetry(x, y);
	for (int i = 0, j = 0; i < chessType[kaijun].n; j++)
	{
		poinT point = aspect_count(chessType[kaijun].p[j][0], chessType[kaijun].p[j][1]);
		if (chess[point.x][point.y].flag == SPACE)
		{
			if (jud)
			{
				bool judd = false;
				for (int m = 0; m <= n; m++)
				{
					if (x1[m] == 2 * x - point.x && y1[m] == 2 * y - point.y)
					{
						judd = true;
						break;
					}
				}
				if (judd)
					continue;
			}
			comX = point.x;
			comY = point.y;
			i++;
			return;
		}
	}
}

void Game::daN()
{
	for (int i = 0; i < dadiann; i++)
	{
		bool n = false;
		for (int j = 0; j < chessType[kaijun].n; j++)
		{
			poinT point = aspect_count(chessType[kaijun].p[j][1], chessType[kaijun].p[j][2]);
			if (chessPoint[4 + i].x() == point.x&&chessPoint[4 + i].y() == point.y)
				n = true;
		}
		if (!n)
		{
			comX = chessPoint[4 + i].x();
			comY = chessPoint[4 + i].y();
			return;
		}
	}
	for (int j = chessType[kaijun].n-1; j >= 0; j--)
	{
		poinT point = aspect_count(chessType[kaijun].p[j][1], chessType[kaijun].p[j][2]);
		for (int i = 0; i < dadiann; i++)
			if (chessPoint[4 + i].x() == point.x&&chessPoint[4 + i].y() == point.y)
			{
				comX = chessPoint[4 + i].x();
				comY = chessPoint[4 + i].y();
				return;
			}
	}

}

void Game::initiChessType()
{
	chessType[0] = { {-1,0,-2,0 },3,15,{-1,1}, { { -1,-1 },{ -1,1 },{ -1,2 },{ 0,1 },{ -2,1 },{ -1,-2 },{ 1,-2 },{ -3,-2 },{ 0,-2 },
	{ -2,-2 },{ -1,3 },{ 2,1 },{ -4,1 },{ 1,2 },{ -3,2 } } };
		//寒星开局3打以下黑优,15个打点里：a级打点3个、b级打点2个（1个对称）、c级打点5个（2个对称）、d级打点5个（2个对称）
	chessType[1] = { { -1, 0, -2, 1 }, 5, 29,{-1,1}, { { -1,1 },{ -1,-1 },{ -1,2 },{ -2,-1 },{ 0,2 },{ -3,-1 },{ 1,2 },{ -1,3 },{ -1,-2 },
	{ 0,-2 },{ -2,3 },{ 0,1 },{ -2,0 },{ -3,1 },{ 1,0 },{ 0,3 },{ -2,-2 },{ -2,2 },{ 0,-1 },{ -4,1 },{ 2,0 },{ -4,-1 },{ 1,2 },
	{ 1,-1 },{ -3,2 },{ -4,2 },{ 2,-1 },{ -2,4 },{ 0,-3 } } };
		//溪月开局
	chessType[2] = { { -1,0,-2,2 },4,8,{-1,1}, { { -1,1 },{ -1,-1 },{ -1,2 },{ 0,1 },{ -2,1 },{ 0,2 },{ -1,3 },{ -3,1 } } };
		//疏星开局
	chessType[3] = { { -1,0,-1,1 },5,23,{0,1}, { { 0,1 },{ -2,2 },{ 1,-1 },{ -2,-1 },{ 1,2 },{ -2,0 },{ 0,2 },{ -1,-1 },{ 1,1 },{ -3,-1 },
	{ 1,3 },{ -2,-2 },{ 2,2 },{ -3,-2 },{ 2,3 },{ -3,0 },{ 0,3 },{ -3,1 },{ -1,3 },{ 0,-1 },{ -2,1 },{ -1,2 },{ 1,0 } } };
		//花月开局
	chessType[4] = { { -1,0,-1,2 },5,10,{0,-1}, { { 0,-1 },{ -2,1 },{ 0,1 },{ 1,-2 },{ 0,2 },{ -1,-1 },{ -2,2 },{ 1,-1 },{ -4,0 },{ -1,1 } } };
		//残月开局
	chessType[5] = { { -1,0,0,1 },3,9,{1,1}, { { 0,-1 },{ -2,1 },{ 1,-2 },{ -1,2 },{ -1,-1 },{ 1,-1 },{ -1,-2 },{ 0,-2 },{ -1,1 } } };
		//雨月开局
	chessType[6] = { { -1,0,0,2 },4,9,{0,-1}, { { 0,-1 },{ -2,1 },{ -1,1 },{ -1,-1 },{ -1,2 },{ 1,-2 },{ -2,0 },{ 1,-1 },{ -3,-1 } } };
		//金星开局
	chessType[7] = { { -1,0,1,0 },3,8,{2,0}, { { -1,1 },{ -1,-1 },{ -1,2 },{ 1,1 },{ -2,1 },{ 1,2 },{ -1,-2 },{ 0,1 } } };
		//松月开局
	chessType[8] = { { -1,0,1,1 },2,5,{-1,-1}, { { -1,-1 },{ -1,-2 },{ -1,1 },{ 0,-1 },{ -2,-1 } } };
		//丘月开局
	chessType[9] = { { -1,0,1,2 },2,7,{0,-1}, { { -1,-1 },{ 0,2 },{ -1,2 },{ -1,-2 },{ 1,-1 },{ -1,1 },{ -2,0 } } };
		//新月开局
	chessType[10] = { { -1,0,2,0 },2,10,{-2,1}, { { -2,1 },{ 1,1 },{ 2,1 },{ 0,-2 },{ 0,2 },{ -1,-1 },{ -2,2 },{ -3,2 },{ -2,-2 },{ 0,-1 } } };
		//瑞星开局
	chessType[11] = { { -1,0,2,1 }, 2, 6,{-1,-1}, { { -1,-1 },{ -1,1 },{ -1,-2 },{ 0,-1 },{ 0,1 },{ -2,-1 } } };
		//山月开局
	chessType[12] = { { -1,0,2,2 },0,18,{0,-1}, { { 0,-1 },{ 1,-2 },{ -2,1 },{ -1,-1 },{ 1,-1 },{ -1,1 },{ -1,-2 },{ -2,-1 },{ 3,1 },{ 1,3 },
	{ 4,0 },{ 0,4 },{ 2,1 },{ 1,2 },{ 0,-2 },{ -2,0 },{ 2,0 },{ 0,2 } } };
		//游星开局
	chessType[13] = { { -1,1,-2,2 },1,10,{-2,0}, { { -2,0 },{ 0,2 },{ -3,-1 },{ -1,-1 },{ 1,1 },{ -3,1 },{ -1,3 },{ -3,2 },{ -2,3 },{ -4,2 } } };
		//长星开局	
	chessType[14] = { { -1,1,-1,2 },5,29,{0,2}, { { 0,1 },{ 1,1 },{ -2,1 },{ 1,2 },{ -2,0 },{ -1,3 },{ -2,-1 },{ -3,1 },{ 2,1 },{ 2,0 },{ -3,2 },
	{ -1,0 },{ 0,2 },{ 0,-1 },{ -1,3 },{ 2,2 },{ -3,0 },{ -2,2 },{ 1,0 },{ 0,-2 },{ -1,4 },{ -2,-2 },{ 1,4 },{ 1,-1 },{ -2,3 },{ 1,-2 },
	{ -2,4 },{ 3,0 },{ -4,2 } } };
		//峡月开局
	chessType[15] = { { -1,1,0,2 },4,15,{0,1}, { { 0,1 },{ 1,1 },{ -2,1 },{ -1,0 },{ -1,2 },{ 2,1 },{ 2,3 },{ 2,-1 },{ 2,2 },{ 2,0 },{ -3,1 },
	{ -1,-2 },{ -1,4 },{ -2,-1 },{ -2,3 } } };
		//恒星开局
	chessType[16] = { { -1,1,1,2 },5,9,{-1,0}, { { -2,1 },{ 1,0 },{ 1,1 },{ 0,1 },{ 1,-1 },{ -1,-1 },{ -3,1 },{ 0,3 },{ 1,4 } } };
		//水月开局
	chessType[17] = { { -1,1,2,2 } , 2, 4,{-1,-1}, { { -1,-1 },{ -1,2 },{ 0,2 },{ -2,0 } } };
		//流星开局
	chessType[18] = { { -1,1,0,1 },3,9,{1,0}, { { 0,2 },{ -2,0 },{ 1,3 },{ -1,-1 },{ -1,2 },{ 1,2 },{ -1,3 },{ 0,3 },{ -1,0 } } };
		//云月开局
	chessType[19] = { { -1,1,1,1 },6,11,{-1,-1}, { { -1,-1 },{ 0,2 },{ 1,2 },{ -1,2 },{ 2,0 },{ -1,0 },{ 0,-1 },{ -1,-2 },{ 0,-2 },{ -1,3 },{ -3,-1 } } };
		//蒲月开局
	chessType[20] = { { -1, 1, 2, 1 }, 3, 11,{-1,0}, { { -1,0 },{ -1,-1 },{ -1,2 },{ 0,-1 },{ 0,1 },{ 0,2 },{ -2,1 },{ -2,-1 },{ 2,2 },{ -2,0 },{ 2,0 } } };
		//岚月开局
	chessType[21] = { { -1, 1, 1, 0 } , 3, 8,{-1,0}, { { -1,0 },{ -1,-1 },{ -1,2 },{ 1,1 },{ -2,1 },{ 1,2 },{ -1,-2 },{ 0,1 } } };
		//银月开局
	chessType[22] = { { -1,1,2,0 },3,8,{-1,0}, { { -1,0 },{ -1,-1 },{ 1,1 },{ -1,2 },{ 0,2 },{ 0,1 },{ 2,1 },{ 2,2 } } };
		//明星开局
	chessType[23] = { { -1,1,1,-1 },2,5,{0,1}, { { -1,0 },{ -1,2 },{ -1,-1 },{ 0,1 },{ -2,1 } } };
		//斜月开局
	chessType[24] = { { -1,1,2,-1 },2,7,{-1,0}, { { -1,0 },{ -1,-1 },{ -1,2 },{ 0,1 },{ -2,1 },{ 0,-1 },{ 2,2 } } };
		//名明开局
	chessType[25] = { { -1,1,2,-2 },0,12,{0,1}, { { -2,1 },{ 0,1 },{ 0,2 },{ -1,-1 },{ 0,-2 },{ 2,0 },{ 1,-2 },{ 2,-1 },{ -3,1 },{ 2,1 },{ 0,-1 },{ -2,0 } } };
		//彗星开局
};

bool Game::Judge_IsSymmetry(double &x,double &y)
{
	x = (chessPoint[0].x() + chessPoint[2].x()) / 2.0;
	y = (chessPoint[0].y() + chessPoint[2].y()) / 2.0;
	if (chessPoint[3].x() == 2 * x - chessPoint[1].x() && chessPoint[3].y() == 2 * y - chessPoint[1].y())
		return true;
	else
		return false;
}

	//	{ { -1,0,-2,0 },3,15,{ { -1,-1 },{ -1,1 },{ -1,2 },{ 0,1 },{ -2,1 },{ -1,-2 },{ 1,-2 },{ -3,-2 },{ 0,-2 },{ -2,-2 },
	//{ -1,3 },{ 2,1 },{ -4,1 },{ 1,2 },{ -3,2 } } },
	////寒星开局3打以下黑优,15个打点里：a级打点3个、b级打点2个（1个对称）、c级打点5个（2个对称）、d级打点5个（2个对称）
	//{ { -1, 0, -2, 1 }, 5, 29,{ { -1,1 },{ -1,-1 },{ -1,2 },{ -2,-1 },{ 0,2 },{ -3,-1 },{ 1,2 },{ -1,3 },{ -1,-2 },{ 0,-2 },
	//{ -2,3 },{ 0,1 },{ -2,0 },{ -3,1 },{ 1,0 },{ 0,3 },{ -2,-2 },{ -2,2 },{ 0,-1 },{ -4,1 },{ 2,0 },{ -4,-1 },{ 1,2 },
	//{ 1,-1 },{ -3,2 },{ -4,2 },{ 2,-1 },{ -2,4 },{ 0,-3 } } },
	////溪月开局
	//{ { -1,0,-2,2 },1,8,{ { -1,1 },{ -1,-1 },{ -1,2 },{ 0,1 },{ -2,1 },{ 0,2 },{ -1,3 },{ -3,1 } } },
	////疏星开局
	//{ { -1,0,-1,1 },5,23,{ { 0,1 },{ -2,2 },{ 1,-1 },{ -2,-1 },{ 1,2 },{ -2,0 },{ 0,2 },{ -1,-1 },{ 1,1 },{ -3,-1 },{ 1,3 },
	//{ -2,-2 },{ 2,2 },{ -3,-2 },{ 2,3 },{ -3,0 },{ 0,3 },{ -3,1 },{ -1,3 },{ 0,-1 },{ -2,1 },{ -1,2 },{ 1,0 } } },
	////花月开局
	//{ { -1,0,-1,2 },5,10,{ { 0,-1 },{ -2,1 },{ 0,1 },{ 1,-2 },{ 0,2 },{ -1,-1 },{ -2,2 },{ 1,-1 },{ -4,0 },{ -1,1 } } },
	////残月开局
	//{ { -1,0,0,1 },3,9,{ { 0,-1 },{ -2,1 },{ 1,-2 },{ -1,2 },{ -1,-1 },{ 1,-1 },{ -1,-2 },{ 0,-2 },{ -1,1 } } },
	////雨月开局
	//{ { -1,0,0,2 },4,9,{ { 0,-1 },{ -2,1 },{ -1,1 },{ -1,-1 },{ -1,2 },{ 1,-2 },{ -2,0 },{ 1,-1 },{ -3,-1 } } },
	////金星开局
	//{ { -1,0,1,0 },3,8,{ { -1,1 },{ -1,-1 },{ -1,2 },{ 1,1 },{ -2,1 },{ 1,2 },{ -1,-2 },{ 0,1 } } },
	////松月开局
	//{ { -1,0,1,1 },1,5,{ { -1,-1 },{ -1,-2 },{ -1,1 },{ 0,-1 },{ -2,-1 } } },
	////丘月开局
	//{ { -1,0,1,2 },2,7,{ { -1,-1 },{ 0,2 },{ -1,2 },{ -1,-2 },{ 1,-1 },{ -1,1 },{ -2,0 } } },
	////新月开局
	//{ { -1,0,2,0 },2,10,{ { -2,1 },{ 1,1 },{ 2,1 },{ 0,-2 },{ 0,2 },{ -1,-1 },{ -2,2 },{ -3,2 },{ -2,-2 },{ 0,-1 } } },
	////瑞星开局
	//{ { -1,0,2,1 }, 2, 6,{ { -1,-1 },{ -1,1 },{ -1,-2 },{ 0,-1 },{ 0,1 },{ -2,-1 } } },
	////山月开局
	//{ { -1,0,2,2 },0,18,{ { 0,-1 },{ 1,-2 },{ -2,1 },{ -1,-1 },{ 1,-1 },{ -1,1 },{ -1,-2 },{ -2,-1 },{ 3,1 },{ 1,3 },
	//{ 4,0 },{ 0,4 },{ 2,1 },{ 1,2 },{ 0,-2 },{ -2,0 },{ 2,0 },{ 0,2 } } },
	////游星开局
	//{ { -1,1,-2,2 },1,10,{ { -2,0 },{ 0,2 },{ -3,-1 },{ -1,-1 },{ 1,1 },{ -3,1 },{ -1,3 },{ -3,2 },{ -2,3 },{ -4,2 } } },
	////长星开局	
	//{ { -1,1,-1,2 },5,29,{ { 0,1 },{ 1,1 },{ -2,1 },{ 1,2 },{ -2,0 },{ -1,3 },{ -2,-1 },{ -3,1 },{ 2,1 },{ 2,0 },{ -3,2 },
	//{ -1,0 },{ 0,2 },{ 0,-1 },{ -1,3 },{ 2,2 },{ -3,0 },{ -2,2 },{ 1,0 },{ 0,-2 },{ -1,4 },{ -2,-2 },{ 1,4 },{ 1,-1 },
	//{ -2,3 },{ 1,-2 },{ -2,4 },{ 3,0 },{ -4,2 } } },
	////峡月开局
	//{ { -1,1,0,2 },3,15,{ { 0,1 },{ 1,1 },{ -2,1 },{ -1,0 },{ -1,2 },{ 2,1 },{ 2,3 },{ 2,-1 },{ 2,2 },{ 2,0 },{ -3,1 },
	//{ -1,-2 },{ -1,4 },{ -2,-1 },{ -2,3 } } },
	////恒星开局
	//{ { -1,1,1,2 },5,9,{ { -2,1 },{ 1,0 },{ 1,1 },{ 0,1 },{ 1,-1 },{ -1,-1 },{ -3,1 },{ 0,3 },{ 1,4 } } },
	////水月开局
	//{ { -1,1,2,2 } , 1, 4,{ { -1,-1 },{ -1,2 },{ 0,2 },{ -2,0 } } },
	////流星开局
	//{ { -1,1,0,1 },3,9,{ { 0,2 },{ -2,0 },{ 1,3 },{ -1,-1 },{ -1,2 },{ 1,2 },{ -1,3 },{ 0,3 },{ -1,0 } } },
	////云月开局
	//{ { -1,1,1,1 },6,11,{ { -1,-1 },{ 0,2 },{ 1,2 },{ -1,2 },{ 2,0 },{ -1,0 },{ 0,-1 },{ -1,-2 },{ 0,-2 },{ -1,3 },{ -3,-1 } } },
	////蒲月开局
	//{ { -1, 1, 2, 1 }, 3, 11,{ { -1,0 },{ -1,-1 },{ -1,2 },{ 0,-1 },{ 0,1 },{ 0,2 },{ -2,1 },{ -2,-1 },{ 2,2 },{ -2,0 },{ 2,0 } } },
	////岚月开局
	//{ { -1, 1, 1, 0 } , 3, 8,{ { -1,0 },{ -1,-1 },{ -1,2 },{ 1,1 },{ -2,1 },{ 1,2 },{ -1,-2 },{ 0,1 } } },
	////银月开局
	//{ { -1,1,2,0 },3,8,{ { -1,0 },{ -1,-1 },{ 1,1 },{ -1,2 },{ 0,2 },{ 0,1 },{ 2,1 },{ 2,2 } } },
	////明星开局
	//{ { -1,1,1,-1 },1,5,{ { -1,0 },{ -1,2 },{ -1,-1 },{ 0,1 },{ -2,1 } } },
	////斜月开局
	//{ { -1,1,2,-1 },2,7,{ { -1,0 },{ -1,-1 },{ -1,2 },{ 0,1 },{ -2,1 },{ 0,-1 },{ 2,2 } } },
	////名明开局
	//{ { -1,1,2,-2 },0,12,{ { -2,1 },{ 0,1 },{ 0,2 },{ -1,-1 },{ 0,-2 },{ 2,0 },{ 1,-2 },{ 2,-1 },{ -3,1 },{ 2,1 },{ 0,-1 },{ -2,0 } } }
	////彗星开局
	//};