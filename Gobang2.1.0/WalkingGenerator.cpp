#include "Game.h"
#include <memory>
#include <iostream>
#include <ctime> 
pair<int, int> Game::GetCurrentMaxPoint(int chessFlag)
{
	int row, col;
	memset(computer, 0, sizeof(computer));
	memset(people, 0, sizeof(people));

	for (row = 0; row<SIZE; ++row)
	{
		for (col = 0; col<SIZE; ++col)
		{
			if (SPACE == chess[row][col].flag)
			{
				if (chessFlag == cComputer)
				{
					people[row][col] = GetScoreForComputer(row, col, cPeople);
					computer[row][col] = GetScoreForComputer(row, col, cComputer);
				}
				else
				{
					people[row][col] = GetScoreForPeople(row, col, cPeople);
					computer[row][col] = GetScoreForPeople(row, col, cComputer);
				}
			}
		}
	}

	// 
	int cx, cy, px, py;
	int maxComputer = -10, maxPeople = -10;
	int maxScore = -10;
	int xx, yy;

	for (row = 0; row<SIZE; ++row)
	{
		for (col = 0; col<SIZE; ++col)
		{
			if (chess[row][col].flag != SPACE)
				continue;

			if (BLACK_CHESS == chessFlag) // ��ֹ���ֵ�
			{
				if (cPeople == chessFlag && -1 == people[row][col])
					continue;
				if (cComputer == chessFlag && -1 == computer[row][col])
					continue;
			}

			if (people[row][col]>maxPeople)
			{
				maxPeople = people[row][col];
				px = row;
				py = col;
			}

			if (computer[row][col]>maxComputer)
			{
				maxComputer = computer[row][col];
				cx = row;
				cy = col;
			}
			if (computer[row][col] + people[row][col]>maxScore)
			{
				maxScore = computer[row][col] + people[row][col];
				xx = row;
				yy = col;
			}
		}
	}

	winPeople = winComputer = mustWin = 0;
	if (chessFlag == cComputer)  ////վ�ڼ�����ĽǶ�
	{
		if (maxComputer >= 5000000)
			winComputer = 1;
		else if (maxComputer >= 200000)
			winComputer = 2;
		else if (maxComputer >= 8000)
			winComputer = 3;

		if (maxPeople >= 1000000)
			winPeople = 1;
		else if (maxPeople >= 40000)
			winPeople = 2;
		else if (maxPeople >= 1900)
			winPeople = 3;
	}
	else
	{
		if (maxComputer >= 1000000)
			winComputer = 1;
		else if (maxComputer >= 40000)
			winComputer = 2;
		else if (maxComputer >= 1900)
			winComputer = 3;

		if (maxPeople >= 5000000)
			winPeople = 1;
		else if (maxPeople >= 200000)
			winPeople = 2;
		else if (maxPeople >= 8000)
			winPeople = 3;
	}

	if (1 == winComputer || 1 == winPeople)
		mustWin = 1;
	else if (2 == winComputer || 2 == winPeople)
		mustWin = 2;
	else if (3 == winComputer || 3 == winPeople)
		mustWin = 3;

	//���������������ô maxComputer �� maxPeople ��Ӧ��Ϊ -10 
	if (-10 == maxComputer && -10 == maxPeople)
		return pair<int, int>(-1, -1);

	// ��� ��3 ,�����ǵ��ı�
	if (mustWin) //���б�Ӯ���ʱ��ȡ����֮����߷֣����������ܵı���ʧ��
		return pair<int, int>(xx, yy);

	return maxComputer >= maxPeople ? pair<int, int>(cx, cy) : pair<int, int>(px, py);
}
void Game::ComputerPlay()
{
	pair<int, int>pi = GetCurrentMaxPoint(cComputer);
	comX = pi.first;
	comY = pi.second;
	int tempX = comX, tempY = comY;

	// ���ڶԷ��� ��3����3�������Ƿ��ػ�4�㣬���ǳ�4�㣬�����Ҫ��һ���ж�
	// ������3����2���������ӵ㣬������3����3���������ӵ�
	// ���ｫ�� 4 �����һ���ķ�ֵ��45000��
	if (2 == winPeople && (!winComputer || winComputer>winPeople))
	{
		if (GetHuo4(comX, comY, cPeople)) //�˹��ɻ�4
		{
			int score = GetScoreForComputer(comX, comY, cPeople); //�õ�����
			score += GetScoreForComputer(comX, comY, cComputer);
			//////////////////////////////////////////////////////////////////////////
			int row = comX, col = comY;
			int i, j, left, right;
			bool flag;
			int chessFlag = cPeople;

			// �� 
			for (i = col - 1, left = 0; i >= 0 && chessFlag == chess[row][i].flag; --i, ++left);
			for (i = col + 1, right = 0; i < SIZE && chessFlag == chess[row][i].flag; ++i, ++right);

			if (3 == left + right && col - left - 1 >= 0 && SPACE == chess[row][col - left - 1].flag
				&& col + right + 1 < SIZE && SPACE == chess[row][col + right + 1].flag)
			{
				flag = true;
				if (BLACK_CHESS == chessFlag)
				{
					if (col - left - 2 >= 0 && chessFlag == chess[row][col - left - 2].flag)
						flag = false;//���ǶԷ����ֵ㣬�����Ǹ���
					if (col + right + 2 < SIZE && chessFlag == chess[row][col + right + 2].flag)
						flag = false;
				}
				if (flag)
				{
					int s1 = 0, s2 = 0;
					if (left)
						s1 = GetScoreForComputer(row, col - left - 1, cComputer) + 45000;
					if (right)
						s2 = GetScoreForComputer(row, col + right + 1, cComputer) + 45000;

					if (s1>score)
					{
						tempX = row, tempY = col - left - 1, score = s1;
					}
					if (s2>score)
					{
						tempX = row, tempY = col + right + 1, score = s2;
					}
				}
			}

			// ��
			for (i = row - 1, left = 0; i >= 0 && chessFlag == chess[i][col].flag; --i, ++left);
			for (i = row + 1, right = 0; i<SIZE && chessFlag == chess[i][col].flag; ++i, ++right);

			if (3 == left + right && row - left - 1 >= 0 && SPACE == chess[row - left - 1][col].flag
				&& row + right + 1 < SIZE && SPACE == chess[row + right + 1][col].flag)
			{
				flag = true;
				if (BLACK_CHESS == chessFlag)
				{
					if (row - left - 2 >= 0 && chessFlag == chess[row - left - 2][col].flag)
						flag = false;
					if (row + right + 2 < SIZE && chessFlag == chess[row + right + 2][col].flag)
						flag = false;
				}
				if (flag)
				{
					int s1 = 0, s2 = 0;
					if (left)
						s1 = GetScoreForComputer(row - left - 1, col, cComputer) + 45000;
					if (right)
						s2 = GetScoreForComputer(row + right + 1, col, cComputer) + 45000;

					if (s1>score)
					{
						tempX = row - left - 1, tempY = col, score = s1;
					}
					if (s2>score)
					{
						tempX = row + right + 1, tempY = col, score = s2;
					}
				}
			}

			// ' \ '
			for (i = row - 1, j = col - 1, left = 0; i >= 0 && j >= 0 && chessFlag == chess[i][j].flag; --i, --j, ++left);
			for (i = row + 1, j = col + 1, right = 0; i<SIZE && j<SIZE && chessFlag == chess[i][j].flag; ++i, ++j, ++right);
			if (3 == left + right && row - left - 1 >= 0 && col - left - 1 >= 0 && SPACE == chess[row - left - 1][col - left - 1].flag
				&& row + right + 1 < SIZE && col + right + 1 < SIZE && SPACE == chess[row + right + 1][col + right + 1].flag)
			{
				flag = true;
				if (BLACK_CHESS == chessFlag)
				{
					if (row - left - 2 >= 0 && col - left - 2 >= 0 && chessFlag == chess[row - left - 2][col - left - 2].flag)
						flag = false;
					if (row + right + 2 < SIZE && col + right + 2 < SIZE && chessFlag == chess[row + right + 2][col + right + 2].flag)
						flag = false;
				}
				if (flag)
				{
					int s1 = 0, s2 = 0;
					if (left)
						s1 = GetScoreForComputer(row - left - 1, col - left - 1, cComputer) + 45000;
					if (right)
						s2 = GetScoreForComputer(row + right + 1, col + right + 1, cComputer) + 45000;

					if (s1>score)
					{
						tempX = row - left - 1, tempY = col - left - 1, score = s1;
					}
					if (s2>score)
					{
						tempX = row + right + 1, tempY = col + right + 1, score = s2;
					}
				}
			}

			// ' / '
			for (i = row - 1, j = col + 1, left = 0; i >= 0 && j<SIZE && chessFlag == chess[i][j].flag; --i, ++j, ++left);
			for (i = row + 1, j = col - 1, right = 0; i<SIZE && j >= 0 && chessFlag == chess[i][j].flag; ++i, --j, ++right);
			if (3 == left + right && row - left - 1 >= 0 && col + left + 1 < SIZE && SPACE == chess[row - left - 1][col + left + 1].flag
				&& row + right + 1 < SIZE && col - right - 1 >= 0 && SPACE == chess[row + right + 1][col - right - 1].flag)
			{
				flag = true;
				if (BLACK_CHESS == chessFlag)
				{
					if (row - left - 2 >= 0 && col + left + 2 < SIZE && chessFlag == chess[row - left - 2][col + left + 2].flag)
						flag = false;
					if (row + right + 2 < SIZE && col - right - 2 >= 0 && chessFlag == chess[row + right + 2][col - right - 2].flag)
						flag = false;
				}
				if (flag)
				{
					int s1 = 0, s2 = 0;
					if (left)
						s1 = GetScoreForComputer(row - left - 1, col + left + 1, cComputer) + 45000;
					if (right)
						s2 = GetScoreForComputer(row + right + 1, col - right - 1, cComputer) + 45000;

					if (s1>score)
					{
						tempX = row - left - 1, tempY = col + left + 1, score = s1;
					}
					if (s2>score)
					{
						tempX = row + right + 1, tempY = col - right - 1, score = s2;
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}
	comX = tempX;
	comY = tempY;
}
void Game::PeoplePlay()
{
	//�õ���Ӧ������ĵ�ǰ���λ��
	pair<int, int>pi = GetCurrentMaxPoint(cPeople);
	peoX = pi.first;
	peoY = pi.second;
	int tempX = peoX, tempY = peoY;

	// ���ڶԷ��� ��3����3�������Ƿ��ػ�4�㣬���ǳ�4�㣬�����Ҫ��һ���ж�
	// ������3����2���������ӵ㣬������3����3���������ӵ�
	// ���ｫ�� 4 �����һ���ķ�ֵ��45000��
	if (2 == winComputer && (!winPeople || winPeople>winComputer))
	{
		if (GetHuo4(peoX, peoY, cComputer)) //��������ɻ�4
		{
			int score = GetScoreForPeople(peoX, peoY, cComputer); //�õ�����
			score += GetScoreForPeople(peoX, peoY, cPeople);
			//////////////////////////////////////////////////////////////////////////
			int row = peoX, col = peoY;
			int i, j, left, right;
			bool flag;
			char chessFlag = cComputer;

			// �� 
			for (i = col - 1, left = 0; i >= 0 && chessFlag == chess[row][i].flag; --i, ++left);
			for (i = col + 1, right = 0; i<SIZE && chessFlag == chess[row][i].flag; ++i, ++right);

			if (3 == left + right && col - left - 1 >= 0 && SPACE == chess[row][col - left - 1].flag
				&& col + right + 1 < SIZE && SPACE == chess[row][col + right + 1].flag)
			{
				flag = true;
				if (BLACK_CHESS == chessFlag)
				{
					if (col - left - 2 >= 0 && chessFlag == chess[row][col - left - 2].flag)
						flag = false;
					if (col + right + 2 < SIZE && chessFlag == chess[row][col + right + 2].flag)
						flag = false;
				}
				if (flag)
				{
					int s1 = 0, s2 = 0;
					if (left)
						s1 = GetScoreForPeople(row, col - left - 1, cPeople) + 45000;
					if (right)
						s2 = GetScoreForPeople(row, col + right + 1, cPeople) + 45000;

					if (s1>score)
					{
						tempX = row, tempY = col - left - 1, score = s1;
					}
					if (s2>score)
					{
						tempX = row, tempY = col + right + 1, score = s2;
					}
				}
			}

			// ��
			for (i = row - 1, left = 0; i >= 0 && chessFlag == chess[i][col].flag; --i, ++left);
			for (i = row + 1, right = 0; i<SIZE && chessFlag == chess[i][col].flag; ++i, ++right);

			if (3 == left + right && row - left - 1 >= 0 && SPACE == chess[row - left - 1][col].flag
				&& row + right + 1 < SIZE && SPACE == chess[row + right + 1][col].flag)
			{
				flag = true;
				if (BLACK_CHESS == chessFlag)
				{
					if (row - left - 2 >= 0 && chessFlag == chess[row - left - 2][col].flag)
						flag = false;
					if (row + right + 2 < SIZE && chessFlag == chess[row + right + 2][col].flag)
						flag = false;
				}
				if (flag)
				{
					int s1 = 0, s2 = 0;
					if (left)
						s1 = GetScoreForPeople(row - left - 1, col, cPeople) + 45000;
					if (right)
						s2 = GetScoreForPeople(row + right + 1, col, cPeople) + 45000;

					if (s1>score)
					{
						tempX = row - left - 1, tempY = col, score = s1;
					}
					if (s2>score)
					{
						tempX = row + right + 1, tempY = col, score = s2;
					}
				}
			}

			// ' \ '
			for (i = row - 1, j = col - 1, left = 0; i >= 0 && j >= 0 && chessFlag == chess[i][j].flag; --i, --j, ++left);
			for (i = row + 1, j = col + 1, right = 0; i<SIZE && j<SIZE && chessFlag == chess[i][j].flag; ++i, ++j, ++right);
			if (3 == left + right && row - left - 1 >= 0 && col - left - 1 >= 0 && SPACE == chess[row - left - 1][col - left - 1].flag
				&& row + right + 1 < SIZE && col + right + 1 < SIZE && SPACE == chess[row + right + 1][col + right + 1].flag)
			{
				flag = true;
				if (BLACK_CHESS == chessFlag)
				{
					if (row - left - 2 >= 0 && col - left - 2 >= 0 && chessFlag == chess[row - left - 2][col - left - 2].flag)
						flag = false;
					if (row + right + 2 < SIZE && col + right + 2 < SIZE && chessFlag == chess[row + right + 2][col + right + 2].flag)
						flag = false;
				}
				if (flag)
				{
					int s1 = 0, s2 = 0;
					if (left)
						s1 = GetScoreForPeople(row - left - 1, col - left - 1, cPeople) + 45000;
					if (right)
						s2 = GetScoreForPeople(row + right + 1, col + right + 1, cPeople) + 45000;

					if (s1>score)
					{
						tempX = row - left - 1, tempY = col - left - 1, score = s1;
					}
					if (s2>score)
					{
						tempX = row + right + 1, tempY = col + right + 1, score = s2;
					}
				}
			}

			// ' / '
			for (i = row - 1, j = col + 1, left = 0; i >= 0 && j<SIZE && chessFlag == chess[i][j].flag; --i, ++j, ++left);
			for (i = row + 1, j = col - 1, right = 0; i<SIZE && j >= 0 && chessFlag == chess[i][j].flag; ++i, --j, ++right);
			if (3 == left + right && row - left - 1 >= 0 && col + left + 1 < SIZE && SPACE == chess[row - left - 1][col + left + 1].flag
				&& row + right + 1 < SIZE && col - right - 1 >= 0 && SPACE == chess[row + right + 1][col - right - 1].flag)
			{
				flag = true;
				if (BLACK_CHESS == chessFlag)
				{
					if (row - left - 2 >= 0 && col + left + 2 < SIZE && chessFlag == chess[row - left - 2][col + left + 2].flag)
						flag = false;
					if (row + right + 2 < SIZE && col - right - 2 >= 0 && chessFlag == chess[row + right + 2][col - right - 2].flag)
						flag = false;
				}
				if (flag)
				{
					int s1 = 0, s2 = 0;
					if (left)
						s1 = GetScoreForPeople(row - left - 1, col + left + 1, cPeople) + 45000;
					if (right)
						s2 = GetScoreForPeople(row + right + 1, col - right - 1, cPeople) + 45000;

					if (s1>score)
					{
						tempX = row - left - 1, tempY = col + left + 1, score = s1;
					}
					if (s2>score)
					{
						tempX = row + right + 1, tempY = col - right - 1, score = s2;
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}
	peoX = tempX;
	peoY = tempY;
}
QPoint Game::peoplePutdown(int x, int y)
{
	if (player < 5)
		chess[x][y].flag = player % 2 + 4;
	else
		chess[x][y].flag = cPeople;
	chess[x][y].seq = player;
		player++;
	chessPoint.push_back(QPoint(x, y));
	return QPoint(x, y);
}
QPoint Game::computerPutdown(int x, int y)
{
	if (player < 5)
		chess[x][y].flag = player % 2 + 4;
	else
		chess[x][y].flag = cComputer;
	chess[x][y].seq = player;
		player++;
	chessPoint.push_back(QPoint(x, y));
	return QPoint(x, y);
}

QPoint Game::ceshizhuanyong(int x, int y)
{
	chess[x][y].flag = player % 2 + 4;
	chess[x][y].seq = player;
	player++;
	chessPoint.push_back(QPoint(x, y));
	return QPoint(x, y);
}
