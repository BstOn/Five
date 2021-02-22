#include "Game.h"
#include <memory>
#include <iostream>
#include <ctime>
#define hashfEXACT 0
#define hashfALPHA 1
#define hashfBETA 2
void Game::UpdateLimit()
{
	int MM = 2;
	if (currentX >= top + MM && currentX <= bottom - MM &&
		currentY >= left + MM && currentY <= right - MM)
		return;
	else if (currentX >= top + MM && currentX <= bottom - MM)
	{
		if (currentY > right - MM)
			right = min(SIZE - 1, currentY + MM);
		else if (currentY < left + MM)
			left = max(0, currentY - MM);
	}
	else if (currentY >= left + MM && currentY <= right - MM)
	{
		if (currentX > bottom - MM)
			bottom = min(SIZE - 1, currentX + MM);
		else if (currentX < top + MM)
			top = max(0, currentX - MM);
	}
	else
	{
		if (currentX < top + MM && currentY > right - MM)
		{
			right = min(SIZE - 1, currentY + MM);
			top = max(0, currentX - MM);
		}
		else if (currentX < top + MM && currentY < left + MM)
		{
			left = max(0, currentY - MM);
			top = max(0, currentX - MM);
		}
		else if (currentX > bottom - MM && currentY < left + MM)
		{
			left = max(0, currentY - MM);
			bottom = min(SIZE - 1, currentX + MM);
		}
		else if (currentX > bottom - MM && currentY > right - MM)
		{
			right = min(SIZE - 1, currentY + MM);
			bottom = min(SIZE - 1, currentX + MM);
		}
	}
}
void Game::ControlDepth()
{
	DEPTH = 12; //Ĭ�� 

	if (player < 6)
	{
		DEPTH = 4;
		return;
	}

	return;
}
void Game::AI()
{
	isDefend = false;
	AIState = 0;
	ComputerPlay();
	int tempX = comX, tempY = comY;

	//if (1 == mustWin)  //�г����
	//	return;
	//if (VCAttack()) // ���Խ��� VC ����
	//	return;
	timeo = *times;
	depthkongzhi = false;
	for (int i = 1; i <= 8; i += 2)
	{
		message.type = 5; message.depth = i;
		if (timeo - *times >= 5||depthkongzhi)
		{
			message.type = 0;
			break;
		}
		MaxMin(i);
	}
}
void Game::InitGameTree(int row, int col, bool& flag, int depth, int& depthC, int& depthP, bool& stop, int depthRecord)
{
	if (stop || depth > DEPTH || depth > depthRecord + 1)
		return;

	if (BLACK_CHESS == cComputer && IsKinjite(row, col)) // ��������˽���λ��,����Ӯ
	{
		depthP = depth;
		stop = true;
		return;
	}
	chess[row][col].flag = cComputer;   //�������

	PeoplePlay();

	if (peoX != -1 && peoY != -1) //����δ��
	{
		if (1 == winPeople) //���г���������ֹͣ����
		{
			depthP = depth;
			stop = true;
		}
		else if (BLACK_CHESS == cPeople && IsKinjite(peoX, peoY))//�����˽��ֵ㣬������Ӯ
		{
			flag = true;
			depthC = depth;
			stop = true;
		}
		else
		{
			int px = peoX, py = peoY;
			chess[px][py].flag = cPeople;  //����

			ComputerPlay();

			if (comX != -1 && comY != -1) //����δ��
			{
				if (1 == winComputer)  //������г���㣬��������
				{
					flag = true;
					depthC = depth;
					stop = true;
				}
				else
					InitGameTree(comX, comY, flag, depth + 1, depthC, depthP, stop, depthRecord); //�ݹ�
			}

			chess[px][py].flag = SPACE; // �ָ�
		}
	}
	chess[row][col].flag = SPACE; // �ָ�
}
void Game::AgainGameTree(int row, int col, int depth, int& depthC, int& depthP, bool& stop, int& depthRecord)
{
	// �����depth����Ӧ���ʵ���Щ����ֹ����vc����
	if (stop || depth > 12 || depth >= depthRecord) // �����depthCFromIGT��������� PeopleAttack() 
		return;

	if (BLACK_CHESS == cPeople && IsKinjite(row, col)) // �����˽���λ��,������Ӯ
	{
		depthC = depth;
		stop = true;
		return;
	}
	chess[row][col].flag = cPeople;   //����

	ComputerPlay();

	if (comX != -1 && comY != -1) //����δ��
	{
		if (1 == winComputer) //������г���������ֹͣ����
		{
			depthC = depth;
			stop = true;
		}
		else if (BLACK_CHESS == cComputer && IsKinjite(comX, comY))//��������˽��ֵ㣬����Ӯ
		{
			depthP = depth;
			stop = true;
		}
		else
		{
			//˵������vc��������ô���������Ӧ������
			if (winPeople && (!winComputer || winPeople<winComputer))
			{
				++depthRecord;
			}

			int cx = comX, cy = comY;
			chess[cx][cy].flag = cComputer;  //�������

			PeoplePlay();

			if (peoX != -1 && peoY != -1) //����δ��
			{
				if (1 == winPeople)  //���г���㣬��������
				{
					depthP = depth;
					stop = true;
				}
				else
					AgainGameTree(peoX, peoY, depth + 1, depthC, depthP, stop, depthRecord); //�ݹ�
			}

			chess[cx][cy].flag = SPACE; // �ָ�
		}
	}
	chess[row][col].flag = SPACE; // �ָ�
}
bool Game::PeopleAttack(int r, int c, int depthCFromIGT)
{
	// ģ���˽��й����������ܷ��� depthCFromIGT ���ڻ��ܼ����
	chess[r][c].flag = cComputer; //������������

	int depthRecord = depthCFromIGT; // depth from InitGameTree()

	bool flag = false; //������Ƿ��ܻ��ܼ����
	int row, col;
	for (row = top; row <= bottom; ++row)
	{
		for (col = left; col <= right; ++col)
		{
			if (chess[row][col].flag != SPACE)
				continue;
			if (BLACK_CHESS == cPeople && IsKinjite(row, col))  //��ִ�ڣ������Ϊ���ֵ�
				continue;

			bool stop = false;
			int depthC = 999, depthP = 999;

			depthRecord = depthCFromIGT; //


			AgainGameTree(row, col, 0, depthC, depthP, stop, depthRecord);

			if (depthC <= depthP) // �������Ӯ
				continue;

			// ������Ӯ��������
			flag = true;
			col = right + 1;
			row = bottom + 1;
		}
	}

	chess[r][c].flag = SPACE; //�ָ�

	if (flag)
		return true;
	return false;
}
void Game::MaxMin( int depth)
{
	if (AIkongzhi)
		return;
	int alpha = 99999999;
	int beta = -99999999;
	int tempx = 0, tempy = 0;
	int best = -99999999;
	poinT result[11];
	int num = heuristic(left, right, top, bottom, cComputer, result);
	for (int i = 0; i < num; i++)
	{
		if (chess[result[i].x][result[i].y].flag != SPACE)
			continue;
		if (BLACK_CHESS == cComputer && IsKinjite(result[i].x, result[i].y)) //������������Լ��Ľ��ֵ�
			continue;
		chess[result[i].x][result[i].y].flag = cComputer;//��������
		Hash_MakeMove(result[i].x, result[i].y, cComputer);
		int score = 0;

		int val = 0;
		score = LookUpHashTable(alpha, beta, depth, cComputer);
		if (score == 666666)
		{
			val = Min(left, right, top, bottom, result[i].x, result[i].y, depth-1 , alpha, best > beta ? best : beta);
		}
		else val = score;
		chess[result[i].x][result[i].y].flag = SPACE;//��������
		Hash_UnMakeMove(result[i].x, result[i].y, cComputer);
		if (val > best)
		{
			best = val;
			tempx = result[i].x, tempy = result[i].y;
		}
	}
	if (best >= 35000)
		depthkongzhi = true;
	message.type = 0;
	message.score = best;
	comX = tempx, comY = tempy;
}
int Game::Max(int left1, int right1, int top1, int bottom1, int row, int col, int depth, int alpha, int beta)
{
	if (AIkongzhi)
		return -99999999;
	int score = 0;
	int val = 0;
	int tempx = 0, tempy = 0;//��ŵõ�����õ�
	int bestmove = -1;
	score = LookUpHashTable(alpha, beta, depth, cPeople);
	if (score != 666666)
		return score;
	int best = -99999999;
	if (depth <= 0 || Judge_IsHuo4ShuangChong4(row, col, cPeople))
	{
		score = Evaluate_Value(cPeople, left1, right1, top1, bottom1);
		EnterHashTable(cPeople, score, depth, hashfEXACT);
		return score;
		//����Ӧ�ü�¼ 
	}
	Chang_UpddateLimit(left1, right1, top1, bottom1, row, col);
	poinT result[11];
	int num = heuristic(left1, right1, top1, bottom1, cComputer, result);
	for (int i = 0; i < num; i++)
	{
		if (chess[result[i].x][result[i].y].flag != SPACE)
			continue;
		if (BLACK_CHESS == cComputer && IsKinjite(result[i].x, result[i].y)) //������������Լ��Ľ��ֵ�
			continue;
		chess[result[i].x][result[i].y].flag = cComputer;//��������
		Hash_MakeMove(result[i].x, result[i].y, cComputer);
		val = Min(left1, right1, top1, bottom1, result[i].x, result[i].y, depth - 1, alpha, best > beta ? best : beta);
		chess[result[i].x][result[i].y].flag = SPACE;//��������
		Hash_UnMakeMove(result[i].x, result[i].y, cComputer);//��������������������㳷��
		if (val > best)
		{
			best = val;
		}
		if (val > alpha)
		{//�����¼һ��ֵ����������ͬʱ���ڣ���������
			bestmove = 1;
			break;
		}
	}
	if (bestmove == -1 && !AIkongzhi)
	{
		EnterHashTable(cPeople, best, depth, hashfEXACT);
		//��¼����߷�����ʱ��32Ϊ��һ�δ����ֵ
	}
	else if (!AIkongzhi)  EnterHashTable(cPeople, val, depth, hashfALPHA);
	//��������Ϊ��һ����ӣ��ʷ�
	return best;
}
int Game::Min(int left1, int right1, int top1, int bottom1, int row, int col, int depth, int alpha, int beta)
{
	if (AIkongzhi)
		return 99999999;
	int best = 99999999;
	int val;
	int bestmove = -1;
	int score = 0;
	score = LookUpHashTable(alpha, beta, depth, cComputer);
	if (score != 666666)
		return score;
	if (depth <= 0 || Judge_IsHuo4ShuangChong4(row, col, cComputer))
	{
		score = Evaluate_Value(cComputer, left1, right1, top1, bottom1);
		EnterHashTable(cComputer, score, depth, hashfEXACT);
		return score;
	}
	Chang_UpddateLimit(left1, right1, top1, bottom1, row, col);
	poinT result[11];
	int num = heuristic(left1, right1, top1, bottom1, cPeople, result);
	for (int i = 0; i < num; i++)
	{
		if (chess[result[i].x][result[i].y].flag != SPACE)
			continue;
		if (BLACK_CHESS == cPeople && IsKinjite(result[i].x, result[i].y)) //������������Լ��Ľ��ֵ�
			continue;
		chess[result[i].x][result[i].y].flag = cPeople;
		Hash_MakeMove(result[i].x, result[i].y, cPeople);
		val = Max(left1, right1, top1, bottom1, result[i].x, result[i].y, depth - 1, best < alpha ? best : alpha, beta);
		chess[result[i].x][result[i].y].flag = SPACE;
		Hash_UnMakeMove(result[i].x, result[i].y, cPeople);
		if (val < best)
		{
			best = val;
		}
		if (val<beta)
		{
			bestmove = 1;

			break;
		}
	}
	if (bestmove == -1 && !AIkongzhi)
	{
		EnterHashTable(cComputer, best, depth, hashfEXACT);
		//��¼����߷�����ʱ��32Ϊ��һ�δ����ֵ
	}
	else if (!AIkongzhi)  EnterHashTable(cComputer, val, depth, hashfBETA);
	//��������Ϊ��һ����ӣ��ʷ�
	return best;
}

bool Game::judge_Neighbored(int x, int y)
{
	for (int i = x - 2; i <= x + 2; i++)
	{
		for (int j = y - 2; j <= y + 2; j++)
		{
			if (i < 0 || i>14 || j < 0 || j>14)
				continue;
			if (chess[i][j].flag != SPACE)
				return false;
		}
	}
	return true;
}
int Game::heuristic(int left1, int right1, int top1, int bottom1, int flag, poinT result[11]) 
{
	int n = 0;
	for(int i= top1; i <=bottom1; i++)
		for (int j = left1; j <=right1; j++)
		{
			if (chess[i][j].flag==SPACE&&!judge_Neighbored(i, j))
			{
				result[n].x = i;
				result[n].y = j;
				if (flag == cComputer)
					result[n].score = GetScoreForComputer(i, j, cComputer) + GetScoreForComputer(i, j, cPeople);
				else
					result[n].score = GetScoreForPeople(i, j, cPeople) + GetScoreForPeople(i, j, cComputer);
				poinT point;
				for (int m = 0 ; m <= n; m++)
					if (result[m].score < result[n].score)
					{
						point = result[m];
						result[m] = result[n];
						result[n] = point;
					}
				if (n != 10)
					n++;
			}
		}
	return n;
}

//QVector<QPoint> Game::gen(int deep, int type) //���ؿ���ѡ��ĵ�
//{
//	int top = -1;
//	bool first = true;
//	QVector<node * > result(12);
//	for (int x = 0; x<15; x++)
//	{
//		for (int y = 0; y<15; y++)
//		{
//			if (chess[x][y] == 0 && hasNeighbor(x, y))
//			{
//				int flag = 1;
//				for (int i = 0; i <= top; i++)
//				{
//					if (result[i]->point.x() == x && result[i]->point.y() == y)
//					{
//						flag = 0;
//						break;
//					}
//				}
//				if (!flag)
//				{
//					continue;
//				}
//				node * snode = new node;
//				snode->point = QPoint(x, y);
//				int tmpscore = snode->sscore = evaluteScore2(QPoint(x, y), type);
//				//evaluteScore2��һ����������������֮ǰ���Ա�ʹ�õ��жϷ������ƣ������жϸĵ���ɵ�������Ԫ��ĵ÷�
//
//				if (first)
//				{
//					result[0] = snode;
//					first = !first;
//					top = 0;
//				}
//				else
//				{
//					if (top == 9 && tmpscore <result[9]->sscore)
//					{
//						continue;
//					}
//
//					int tmp = top;
//					while (tmp >= 0 && result[tmp]->sscore<tmpscore)
//					{
//						result[tmp + 1] = result[tmp];
//						tmp--;
//					}
//					result[tmp + 1] = snode;
//					top++;
//					if (top>9)
//					{
//						top = 9;
//					}
//				}
//			}
//		}
//	}
//	QVector<QPoint> temp;
//	for (int i = 0; i<10; i++)
//	{
//		if (i <= top)
//			temp.push_back(result[i]->point);
//	}
//	for (int i = 0; i <= top; i++)
//	{
//		free(result[i]);
//	}
//	return temp;
//}

void Game::InitializeHashKey()
{
	int i = 0, j = 0, k = 0;
	srand((unsigned)time(NULL));
	//����������
	for (i = 0; i <= 5; i++)
		for (j = 0; j<15; j++)
			for (k = 0; k<15; k++)
			{
				m_nHashKey32[i][j][k] = rand32();
				m_ulHashKey64[i][j][k] = rand64();
			}

	//�����û������ÿռ䡣1M "2 ����Ŀ������Ҳ��ָ��������С
}
void Game::EnterHashTable(int flag, int eval, int  depth, int type)
{
	int x;
	x = m_HashKey32 & 0xFFFFF;//�����ʮλ��ϣ��ַ
	if (Hashtable[flag][x].flags != 0 && Hashtable[flag][x].depth > depth)
		return;//������Ȳ���
	Hashtable[flag][x].key = m_HashKey64; //64λУ����1
	Hashtable[flag][x].flags = type;//��������
	Hashtable[flag][x].value = eval;          //Ҫ�����ֵ
	Hashtable[flag][x].depth = depth;          //���
}
int Game::LookUpHashTable(int alpha, int beta, int depth, int flag)
{
	int x;

	//�����ʮλ��ϣ��ַ����������趨�Ĺ�ϣ���С���� 1M*2 ��,
	//���� TableSize*2��TableSizeΪ�����趨�Ĵ�С
	//����Ҫ�޸���һ��Ϊm_HashKey32% TableSize
	//��һ����������һ��Ҳһ��
	x = m_HashKey32 & 0xFFFFF;

	//if (hashtable[flag][x].depth >= depth && hashtable[flag][x].key == m_HashKey64)
	//{
	//	switch (hashtable[flag][x].flags)//�ж���������
	//	{
	//	case hashfEXACT://ȷ��ֵ
	//		return hashtable[flag][x].value;  break;

	//	case hashfBETA://�±߽�
	//		if (hashtable[flag][x].value >= beta)
	//			return beta;
	//		else
	//			break;
	//	case hashfALPHA://�ϱ߽�
	//		if (hashtable[flag][x].value >= alpha)
	//			return alpha;
	//		else
	//			break;
	//	}
	//}

	if (Hashtable[flag][x].depth >= depth && Hashtable[flag][x].key == m_HashKey64)
	{
		if (Hashtable[flag][x].flags == hashfEXACT)
		{
			return Hashtable[flag][x].value;
		}
		else if (Hashtable[flag][x].flags == hashfBETA)
		{
			if (Hashtable[flag][x].value <= beta)
				return Hashtable[flag][x].value;
		}
		else
		{
			if (Hashtable[flag][x].value >= alpha)
				return Hashtable[flag][x].value;
		}
	}
	return 666666;
}
void Game::Hash_MakeMove(int row, int col, int chessFlag)
{
	//��������Ŀ��λ�õ����������
	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[chessFlag][row][col];
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[chessFlag][row][col];
}
void Game::Hash_UnMakeMove(int row, int col, int chessFlag)
{
	//��������Ŀ��λ�õ�������Ƴ����ٴθ�ֵ
	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[chessFlag][row][col];
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[chessFlag][row][col];
}
void Game::_CTranspositionTable()
{
	delete Hashtable[4];
	delete Hashtable[5];
}
void Game::CTranspositionTable()
{
	InitializeHashKey();//������ϣ�������������
}
void Game::CalculateInitHashKey()

{
	int j, k;
	m_HashKey32 = 0;
	m_HashKey64 = 0;
	//���������Ӷ�Ӧ�Ĺ�ϣ������
	for (int i = 0; i < player; i++)
	{
		m_HashKey32 = m_HashKey32 ^ m_nHashKey32[chess[chessPoint[i].x()][chessPoint[i].y()].flag][chessPoint[i].x()][chessPoint[i].y()];
		m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[chess[chessPoint[i].x()][chessPoint[i].y()].flag][chessPoint[i].x()][chessPoint[i].y()];

	}

}
__int64 Game::rand64()
{
	return rand() ^ ((__int64)rand() << 15) ^ ((__int64)rand() << 30) ^
		((__int64)rand() << 45) ^ ((__int64)rand() << 60);
}
long  Game::rand32()
{
	return rand() ^ ((long)rand() << 15) ^ ((long)rand() << 30);
}
