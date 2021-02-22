#include "Game.h"
#include <memory>
#include <iostream>
#include <ctime> 
void Game::Chang_UpddateLimit(int &left_temp, int& right_temp, int &top_temp, int &bottom_temp, int X, int Y) {

	int MM = 3;
	if (X >= top_temp + MM && X <= bottom_temp - MM &&
		Y >= left_temp + MM && Y <= right_temp - MM)
		return;
	else if (X >= top_temp + MM && X <= bottom_temp - MM)
	{
		if (Y > right_temp - MM)
			right_temp = min(SIZE - 1, Y + MM);
		else if (Y < left_temp + MM)
			left_temp = max(0, Y - MM);
	}
	else if (Y >= left_temp + MM && Y <= right_temp - MM)
	{
		if (X > bottom_temp - MM)
			bottom_temp = min(SIZE - 1, X + MM);
		else if (X < top_temp + MM)
			top_temp = max(0, X - MM);
	}
	else
	{
		if (X < top_temp + MM && Y > right_temp - MM)
		{
			right_temp = min(SIZE - 1, Y + MM);
			top_temp = max(0, X - MM);
		}
		else if (X < top_temp + MM && Y < left_temp + MM)
		{
			left_temp = max(0, Y - MM);
			top_temp = max(0, X - MM);
		}
		else if (X > bottom_temp - MM && Y < left_temp + MM)
		{
			left_temp = max(0, Y - MM);
			bottom_temp = min(SIZE - 1, X + MM);
		}
		else if (X > bottom_temp - MM && Y > right_temp - MM)
		{
			right_temp = min(SIZE - 1, Y + MM);
			bottom_temp = min(SIZE - 1, X + MM);
		}
	}
}
bool Game::VCAttack()
{
	// �ж��Ƿ��п��Խ��� VC �����ĵ�
	// ���жϼ�����ܷ� VCF����ȡʤ�����ж����Ƿ�ͨ��VCF����ȡʤ��Ȼ�����жϼ�����Ƿ�ͨ��VCF��VCT����
	//����ȡʤ��Ҳ�ж��˵�VCF��VCT���Ϲ���(��Ȼ��һ��).
	// Ϊ����� ������ ���ϵĳ��Ը�����ȣ��ȴ�С����ȿ�ʼ���أ��������ܱ�֤����С�Ĳ���Ӯ�壬
	// һ���ҵ���Ӯ�㣬��������
	// Ŀǰ��Ȼ��bug���������ٶȺ�����֮��Ȩ��

	// VC ��������4����4(��4������4)����3����3����3�������ȼ����ν���

	int row, col;
	VCPoint vcPoint[100]; //
	int cnt;
	int tempDepth; //��ȿ���

	VCPoint dePoint[100]; //���� VC ���أ��������ģ���˵Ĺ����б��湥����
	int num = 0; //

	int tempX, tempY, depthRecord = 9999, maxScore = -9999;
	bool ff = false;
	int left1 = left, right1 = right, bottom1 = bottom, top1 = top;
	// ���жϼ�����Ƿ�ɽ��� VCF ����,  �����򷵻�

	cnt = 0;
	message.cnt = cnt;

	message.type = 1;
	for (row = top; row <= bottom; ++row)
	{
		for (col = left; col <= right; ++col)
		{
			if (chess[row][col].flag != SPACE)
				continue;
			if (BLACK_CHESS == cComputer && IsKinjite(row, col)) // ������Ľ��ֵ�
				continue;

			int res = IsCombForVC(row, col, cComputer);

			if (res<3) // VCF����
				continue;

			vcPoint[cnt++].init(row, col, res);	//�ǽ��������ȱ���

			if (cnt >= 100)
			{
				row = bottom + 1;
				col = right + 1;
			}
		}
	}

	sort(vcPoint, vcPoint + cnt); //
	message.cnt = cnt;
	for (tempDepth = VCDEPTH; tempDepth <= VCDEPTH_END; tempDepth += VCDEPTH_DIS)//��Ȳ��ϵ���
	{
		depthMM = tempDepth; //
		for (int i = 0; i<cnt; ++i)
		{
			bool flag = false;
			int ansDepth = 9999;
			VCAttackTree(1, vcPoint[i].row, vcPoint[i].col, cComputer, cPeople, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);

			if (flag) // �ҵ�һ����Ӯ������
			{
				ff = true; //
				if (ansDepth<depthRecord) //ѡȡ�����Ӯ��
				{
					depthRecord = ansDepth;
					tempX = vcPoint[i].row;
					tempY = vcPoint[i].col;
					maxScore = GetScoreForComputer(tempX, tempY, cComputer);
					maxScore += GetScoreForComputer(tempX, tempY, cPeople);
				}
				else if (ansDepth == depthRecord) //���һ��ʱѡȡȨֵ���
				{
					int res = GetScoreForComputer(vcPoint[i].row, vcPoint[i].col, cComputer);
					res += GetScoreForComputer(vcPoint[i].row, vcPoint[i].col, cPeople);
					if (res > maxScore)
					{
						maxScore = res;
						tempX = vcPoint[i].row;
						tempY = vcPoint[i].col;
					}
				}
			}
		}
		if (ff)
		{
			comX = tempX;
			comY = tempY;
			AIState = 1; //
			message.type = 0;
			return true;
		}

	}


	message.type = 2;
	// ���ж����Ƿ�ͨ�� VCF����ȡʤ�������ԣ������������� VCF��VCT�����Ϲ�������Ϊ������
	// ����Ҫ�����ж� ���Ƿ񻹿��� ͨ�� VCF��VCT�����Ϲ���ȡʤ�����ϵ�������������
	int peoIsVCF = false;
	int j;
	//////////////////////////////////////////////////////////////////////////
	cnt = 0;
	message.cnt = cnt;
	for (row = top; row <= bottom; ++row)
	{
		for (col = left; col <= right; ++col)
		{
			if (chess[row][col].flag != SPACE)
				continue;
			if (BLACK_CHESS == cPeople && IsKinjite(row, col)) // �˵Ľ��ֵ�
				continue;

			int res = IsCombForVC(row, col, cPeople);

			if (res<3) // VCF����
				continue;

			vcPoint[cnt++].init(row, col, res);	//�ǽ��������ȱ���
			if (cnt >= 100)
			{
				row = bottom + 1;
				col = right + 1;
			}
		}
	}
	message.cnt = cnt;
	sort(vcPoint, vcPoint + cnt); //


	for (tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//��Ȳ��ϵ���
	{
		depthMM = tempDepth; //
		for (int i = 0; i<cnt; ++i)
		{
			bool flag = false;
			int ansDepth = 9999;

			VCAttackTree(1, vcPoint[i].row, vcPoint[i].col, cPeople, cComputer, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);

			if (flag) // �ҵ�һ����Ӯ������
			{
				ff = true; //
				j = i;
				i = cnt;
				break;
				tempDepth = VCDE_DEPTH_END + 1;
			}
		}
	}
	if (ff)
	{
		comX = vcPoint[j].row;
		comY = vcPoint[j].col;
		peoIsVCF = true;
		AIState = 2;


	}

	ff = false;


	message.type = 4;
	//�жϼ�����ܷ�ͨ�� VCF��VCT���Ϲ���ȡʤ,  ǰ�����˲��ܽ��� VCF����
	//////////////////////////////////////////////////////////////////////////
	//if (!peoIsVCF)
	//{
	//	cnt = 0;
	//	message.cnt = cnt;
	//	for (row = top; row <= bottom; ++row)
	//	{
	//		for (col = left; col <= right; ++col)
	//		{
	//			if (chess[row][col].flag != SPACE)
	//				continue;
	//			if (BLACK_CHESS == cComputer && IsKinjite(row, col)) // ������Ľ��ֵ�
	//				continue;

	//			int res = IsCombForVC(row, col, cComputer);

	//			if (!res) // VCF��VCT���Ϲ���
	//				continue;

	//			vcPoint[cnt++].init(row, col, res);	//�ǽ��������ȱ���

	//			if (cnt >= 100)
	//			{
	//				row = bottom + 1;
	//				col = right + 1;
	//			}
	//		}
	//	}
	//	sort(vcPoint, vcPoint + cnt); //
	//	message.cnt = cnt;
	//	for (tempDepth = VCDEPTH; tempDepth <= VCDEPTH_END; tempDepth += VCDEPTH_DIS)//��Ȳ��ϵ���
	//	{
	//		depthMM = tempDepth; //
	//		for (int i = 0; i<cnt; ++i)
	//		{
	//			bool flag = false;
	//			int ansDepth = 9999;
	//			VCAttackTree(2, vcPoint[i].row, vcPoint[i].col, cComputer, cPeople, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);

	//			if (flag) //�ҵ�һ����Ӯ������
	//			{
	//				ff = true; //
	//				if (ansDepth<depthRecord) //ѡȡ�����Ӯ��
	//				{
	//					depthRecord = ansDepth;
	//					tempX = vcPoint[i].row;
	//					tempY = vcPoint[i].col;
	//					maxScore = GetScoreForComputer(tempX, tempY, cComputer);
	//					maxScore += GetScoreForComputer(tempX, tempY, cPeople);
	//				}
	//				else if (ansDepth == depthRecord) //���һ��ʱѡȡȨֵ���
	//				{
	//					int res = GetScoreForComputer(vcPoint[i].row, vcPoint[i].col, cComputer);
	//					res += GetScoreForComputer(vcPoint[i].row, vcPoint[i].col, cPeople);
	//					if (res > maxScore)
	//					{
	//						maxScore = res;
	//						tempX = vcPoint[i].row;
	//						tempY = vcPoint[i].col;
	//					}
	//				}

	//			}
	//		}
	//		if (ff)
	//		{
	//			comX = tempX;
	//			comY = tempY;
	//			AIState = 3;
	//			message.type = 0;
	//			return true;
	//		}
	//	}
	//}


	message.type = 3;
	//�ж����ܷ�ͨ�� VCF��VCT���Ϲ���ȡʤ��  VC ����
	//////////////////////////////////////////////////////////////////////////
	//cnt = 0;
	//depthRecord = 9999;
	//message.cnt = cnt;
	//for (row = top; row <= bottom; ++row)
	//{
	//	for (col = left; col <= right; ++col)
	//	{
	//		if (chess[row][col].flag != SPACE)
	//			continue;
	//		if (BLACK_CHESS == cPeople && IsKinjite(row, col)) // �Լ��Ľ��ֵ�
	//			continue;

	//		int res = IsCombForVC(row, col, cPeople);

	//		if (!res) // VCF��VCT���Ϲ���
	//			continue;

	//		vcPoint[cnt++].init(row, col, res);	//�ǽ��������ȱ���

	//		if (cnt >= 100)
	//		{
	//			row = bottom + 1;
	//			col = right + 1;
	//		}
	//	}
	//}

	//sort(vcPoint, vcPoint + cnt); //
	//message.cnt = cnt;
	//for (tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//��Ȳ��ϵ���
	//{
	//	depthMM = tempDepth; //
	//	for (int i = 0; i<cnt; ++i)
	//	{
	//		bool flag = false;
	//		int ansDepth = 9999;
	//		isDefend = false;

	//		VCAttackTree(2, vcPoint[i].row, vcPoint[i].col, cPeople, cComputer, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);

	//		if (flag) // �ҵ�һ����Ӯ������
	//		{
	//			isDefend = true; //���أ���Ҫ��Ϊ�� ���ڶԷ������ķ���
	//			ff = true; //		
	//			if (ansDepth<depthRecord) //ѡȡ�����Ӯ��
	//			{
	//				depthRecord = ansDepth;
	//				tempX = vcPoint[i].row;
	//				tempY = vcPoint[i].col;
	//				maxScore = GetScoreForComputer(tempX, tempY, cPeople);
	//				maxScore += GetScoreForComputer(tempX, tempY, cComputer);
	//			}
	//			else if (ansDepth == depthRecord) //���һ��ʱѡȡȨֵ���
	//			{
	//				int res = GetScoreForComputer(vcPoint[i].row, vcPoint[i].col, cPeople);
	//				res += GetScoreForComputer(vcPoint[i].row, vcPoint[i].col, cComputer);
	//				if (res > maxScore)
	//				{
	//					maxScore = res;
	//					tempX = vcPoint[i].row;
	//					tempY = vcPoint[i].col;
	//				}
	//			}
	//		}
	//	}
	//	if (ff)
	//	{
	//		// ���أ������ѡ�񹥻�λ�����ӣ���������Ϊ���˸õ��ʹ�˲������ VC ������
	//		//////////////////////////////////////////////////////////////////////////
	//		message.cnt = num;
	//		for (row = top; row <= bottom; ++row)
	//		{
	//			for (col = left; col <= right; ++col)
	//			{
	//				if (chess[row][col].flag != SPACE)
	//					continue;
	//				if (BLACK_CHESS == cComputer && IsKinjite(row, col))
	//					continue;

	//				int res1 = IsCombForVC(row, col, cComputer);
	//				int res2 = IsCombForVC(row, col, cPeople);

	//				if (!res1 && !res2)
	//					continue;
	//				else if ((3 == res1 || 4 == res1) && !res2) //һ������²�Ҫ���Լ���4������4
	//					continue;

	//				int res = GetScoreForComputer(row, col, cComputer);
	//				res += GetScoreForComputer(row, col, cPeople);

	//				dePoint[num++].init(row, col, res);
	//			}
	//		}

	//		sort(dePoint, dePoint + num); //��Ȩֵ����
	//		message.cnt = num;
	//		for (int p = 0; p<num; ++p)
	//		{
	//			bool noVC = true;

	//			chess[dePoint[p].row][dePoint[p].col].flag = cComputer; //����������������

	//																	// ����������ģ�� �˵� VC ����
	//			cnt = 0;          
	//			depthRecord = 9999;
	//			message.cnt = cnt;
	//			for (row = top; row <= bottom; ++row)
	//			{
	//				for (col = left; col <= right; ++col)
	//				{
	//					if (chess[row][col].flag != SPACE)
	//						continue;
	//					if (BLACK_CHESS == cPeople && IsKinjite(row, col)) // �˵Ľ��ֵ�
	//						continue;

	//					int res = IsCombForVC(row, col, cPeople);

	//					if (!res) // VCF��VCT���Ϲ���
	//						continue;

	//					vcPoint[cnt++].init(row, col, res);	//�ǽ��������ȱ���

	//					if (cnt >= 100)
	//					{
	//						row = bottom + 1;
	//						col = right + 1;
	//					}
	//				}
	//			}

	//			sort(vcPoint, vcPoint + cnt); //
	//			message.cnt = cnt;

	//			for (tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//��Ȳ��ϵ���
	//			{
	//				depthMM = tempDepth; //
	//				for (int i = 0; i<cnt; ++i)
	//				{
	//					bool flag = false;
	//					int ansDepth = 9999;
	//					VCAttackTree(2, vcPoint[i].row, vcPoint[i].col, cPeople, cComputer, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);

	//					if (flag) // �ҵ�����������
	//					{
	//						noVC = false;
	//						i = cnt;
	//						tempDepth = VCDE_DEPTH_END + 1;
	//					}
	//				}
	//			}

	//			chess[dePoint[p].row][dePoint[p].col].flag = SPACE; //�ָ�

	//			if (noVC) //�����û�� VC ������
	//			{
	//				tempX = dePoint[p].row;
	//				tempY = dePoint[p].col;
	//				break;
	//			}
	//		}

	//		//////////////////////////////////////////////////////////////////////////
	//		comX = tempX;
	//		comY = tempY;
	//		AIState = 4;
	//		message.type = 0;
	//		return true;
	//	}
	//}
	//message.type = 0;
	return false;
}
void Game::VCAttackTree(int type, int row, int col, char cOneself, char cOpposite, int depth, bool& flag, int& ansDepth, int depthRecord, int left1, int right1, int top1, int bottom1)
{
	message.depth = depth;
	// ��ȿ���,���̫��Ļ������ӶȾ�̫���ˣ�depthRecord������¼�Ѿ��ѹ��ĵ��д��Ӯ�����С���
	if (depth>depthMM || depth>depthRecord)
		return;

	if (cOneself == cComputer)
	{
		ComputerPlay();
		if (1 == winComputer)
			flag = true;
	}
	else
	{
		PeoplePlay();
		if (1 == winPeople)
			flag = true;
	}
	if (flag) // flag Ϊ true ʱ����ʾ���Ա�Ӯ���ܱ�Ӯ�򷵻أ�����Ҫȥ�����ŵģ�����̫��
	{
		ansDepth = depth; //��¼���ٲ���Ӯ
		return;
	}
	chess[row][col].flag = cOneself; // �Լ�����

	int oppositeX, oppositeY;
	int winOpposite = 0;
	int dd = 1; // ��ȵĵ���ֵ

				//////////////////////�Է�˼��/////////////////////////////////////////////
	if (cOpposite == cPeople)
	{
		PeoplePlay();
		oppositeX = peoX, oppositeY = peoY;
		winOpposite = winPeople;

	}
	else //
	{
		ComputerPlay();
		oppositeX = comX, oppositeY = comY;
		winOpposite = winComputer;
	}

	// ����Է��ڷ��ص�ͬʱ Ҳ�����˳�4���3����ô�Լ�����Ҫ���������ضԷ�����ʱӦ�ý���ȼ�1
	if (1 == type) // VCF ����
	{
		pair<int, pair<int, int> > ppi = GetChong3Chong4(oppositeX, oppositeY, cOpposite);
		if (ppi.second.first + ppi.second.second) //���ɳ�4������4
			dd = 2;

		//		if(ppi.second.first + ppi.second.second >=2 ) //
		//			winOpposite = 1;
	}
	else if (2 == type) // VCF �� VCT ���Ϲ���
	{
		pair<int, pair<int, int> > ppi = GetChong3Chong4(oppositeX, oppositeY, cOpposite);
		if (ppi.second.first + ppi.second.second) //���ɳ�4������4
			dd = 2;
		pair<int, int> pi1 = GetLian2Lian3(oppositeX, oppositeY, cOpposite);
		if (pi1.second) //������3
			dd = 2;
		pair<int, int> pi2 = GetTiao2Tiao3(oppositeX, oppositeY, cOpposite);
		if (pi2.second) //������3
			dd = 2;

		//		if(ppi.second.first+ppi.second.second + pi1.second+pi2.second >=2 ) //
		//			winOpposite = 1;
	}
	//////////////////////////////////////////////////////////////////////////
	Chang_UpddateLimit(left1, right1, top1, bottom1, oppositeX, oppositeY);
	int left_temp = left1, right_temp = right1, top_temp = top1, bottom_temp = bottom1;
	if ((oppositeX != -1 && oppositeY != -1) && (winOpposite != 1)) // ����δ�� �� �Է� δ����
	{
		if (cOpposite == BLACK_CHESS && IsKinjite(oppositeX, oppositeY)) // �õ��ǶԷ��Ľ���
			flag = true;
		else
		{
			chess[oppositeX][oppositeY].flag = cOpposite; // �Է�����

			VCPoint vcPoint[100];// �����3����4����4��Щ��
			int cnt = 0;
			int i, j;

			for (i = top_temp; i <bottom_temp; ++i)
			{
				for (j = left_temp; j <right_temp; ++j)
				{
					if (chess[i][j].flag != SPACE)
						continue;
					if (BLACK_CHESS == cOneself && IsKinjite(i, j)) // �Լ��Ľ��ֵ�
						continue;

					int res = IsCombForVC(i, j, cOneself);
					//�ǹ������򱣴�,�������ͷ�2�֣�VCF����VCF��VCT���Ϲ���
					if (1 == type && res<3)
						continue;
					if (2 == type && !res)
						continue;

					vcPoint[cnt++].init(i, j, res);
					if (cnt >= 100)
					{
						i = j = bottom_temp;
					}
				}
			}

			sort(vcPoint, vcPoint + cnt);  //�����ȼ�����

			for (i = 0; i<cnt; ++i)
			{
				if (!flag)
				{
					VCAttackTree(type, vcPoint[i].row, vcPoint[i].col, cOneself, cOpposite, depth + dd, flag, ansDepth, depthRecord, left_temp, right_temp, top_temp, bottom_temp); //
				}
			}

			chess[oppositeX][oppositeY].flag = SPACE; // �ָ�
		}
	}
	chess[row][col].flag = SPACE; //�ָ�
}
int Game::IsCombForVC(int row, int col, char chessFlag)
{
	//�ж��Ƿ��� VC �����Ĺ����㣬���������ȼ�
	Search(row, col, chessFlag);

	if (huo4)
		return 5;
	if (chong4)
		return 4;
	if (tiaochong4)
		return 3;
	if (lian3)
		return 2;
	if (tiao3)
		return 1;

	return 0;
}
