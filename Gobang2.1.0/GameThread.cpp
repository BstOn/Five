#include "GameThread.h"

void GameThread::run()
{
	bool backAI = false;
	T_game->waitThread = true;
	backAI = true;
	if (T_game->player <= 7)
	{
		T_game->Hashtable[4] = new HASHE[1024 * 1024]();
		T_game->Hashtable[5] = new HASHE[1024 * 1024]();
	}
	T_game->CalculateInitHashKey();
	T_game->AI();
	T_game->_CTranspositionTable();
	T_game->Hashtable[4] = new HASHE[1024 * 1024]();
	T_game->Hashtable[5] = new HASHE[1024 * 1024]();
	QPoint point = T_game->computerPutdown(T_game->comX, T_game->comY);
	T_game->currentX = point.x();
	T_game->currentY = point.y();
	/*memcpy(T_game->chess_B, T_game->chess, sizeof(T_game->chess_B));*/
	emit goon();
	while (!T_game->waitBackAI);
	if (backAI)
		T_game->BackstageAI();
	T_game->waitThread = false;
}

//void GameThread::AI()
//{
//	T_game->isDefend = false;
//	T_game->AIState = 0;
//
//	T_game->ComputerPlay();
//	int tempX = T_game->comX, tempY = T_game->comY; //
//
//	if (1 == T_game->mustWin)  //�г����
//		return;
//
//	if (T_game->VCAttack()) // ���Խ��� VC ����
//		return;
//
//	T_game->ControlDepth(); // ���õݹ����
//
//	int row, col, depthRecord = 999, maxScore = -999;
//
//	// ���ҵ�һ����Ӯ�㣬�㽫����Ϊ true��������Ҫ������Ƿ����Ӯ����������磺
//	// �����������������Ӻ󹹳�һ����4��һ����3��Ȼ�����ڵ���4��ʱ���γ��˻�4�������˾���Ӯ��
//	// ���Բ��ܵ������������ֵȥ�ж�
//	bool safeFlag = false;
//
//	for (row = T_game->top; row <= T_game->bottom; ++row)
//	{
//		for (col = T_game->left; col <= T_game->right; ++col)
//		{
//			if (T_game->chess[row][col].flag != SPACE)
//				continue;
//			if (BLACK_CHESS == T_game->cComputer && T_game->IsKinjite(row, col))  //�����ִ�ڣ������Ϊ���ֵ�
//				continue;
//
//			bool flag = false, stop = false;
//			int depthC = 999, depthP = 999;
//
//			T_game->InitGameTree(row, col, flag, 0, depthC, depthP, stop, depthRecord);
//
//			//�����ǵ���������ôdepthC��depthP������һ���ǲ���ģ�
//			//depthP<=depthC��˵����������·���¼����������Ӯ
//			if (depthP <= depthC)
//				continue;
//
//			// �������ڼ�����Ľ�����ͼ�У������������ģ���˽�������˭��Ӯ������Ӯ���򷵻�true
//			// ע���������
//			if (T_game->PeopleAttack(row, col, depthC)) //// ����һ������������������
//				continue;
//
//			if (flag)  //������б�Ӯ�㣬�㽫��㱣������
//			{
//				// ��Ҫ���ǽ������,��Ҫԭ������Ϊ��������
//				bool ff = false;
//				if (BLACK_CHESS == T_game->cComputer && depthC >= 2)
//				{
//					T_game->chess[row][col].flag = T_game->cComputer;  //�����������
//					for (int i = T_game->top; i <= T_game->bottom; ++i)
//					{
//						for (int j = T_game->left; j <= T_game->right; ++j)
//						{
//							if (T_game->IsKinjite(i, j)) // �����˽��ֵ�
//							{
//								ff = true;
//								i = T_game->bottom + 1;
//								j = T_game->right + 1;
//							}
//						}
//					}
//					T_game->chess[row][col].flag = SPACE; //�ָ�
//				}
//				if (ff)
//					continue;
//
//				if (depthC < depthRecord)  //ѡȡ���С��
//				{
//					maxScore = T_game->GetScoreForComputer(row, col, T_game->cComputer);
//					depthRecord = depthC;
//					tempX = row;
//					tempY = col;
//				}
//				else if (depthC == depthRecord) //���һ��ʱѡȡȨֵ���
//				{
//					int res = T_game->GetScoreForComputer(row, col, T_game->cComputer);
//					if (res > maxScore)
//					{
//						maxScore = res;
//						tempX = row;
//						tempY = col;
//					}
//				}
//				safeFlag = true; ///
//			}
//		}
//	}
//}
//
//
//bool GameThread::VCAttack()
//{
//	// �ж��Ƿ��п��Խ��� VC �����ĵ�
//	// ���жϼ�����ܷ� VCF����ȡʤ�����ж����Ƿ�ͨ��VCF����ȡʤ��Ȼ�����жϼ�����Ƿ�ͨ��VCF��VCT����
//	//����ȡʤ��Ҳ�ж��˵�VCF��VCT���Ϲ���(��Ȼ��һ��).
//	// Ϊ����� ������ ���ϵĳ��Ը�����ȣ��ȴ�С����ȿ�ʼ���أ��������ܱ�֤����С�Ĳ���Ӯ�壬
//	// һ���ҵ���Ӯ�㣬��������
//	// Ŀǰ��Ȼ��bug���������ٶȺ�����֮��Ȩ��
//
//	// VC ��������4����4(��4������4)����3����3����3�������ȼ����ν���
//
//	int row, col;
//	VCPoint vcPoint[100]; //
//	int cnt;
//	int tempDepth; //��ȿ���
//
//	VCPoint dePoint[100]; //���� VC ���أ��������ģ���˵Ĺ����б��湥����
//	int num = 0; //
//
//	int tempX, tempY, depthRecord = 9999, maxScore = -9999;
//	bool ff = false;
//	int left1 = T_game->left, right1 = T_game->right, bottom1 = T_game->bottom, top1 = T_game->top;
//	// ���жϼ�����Ƿ�ɽ��� VCF ����,  �����򷵻�
//
//	cnt = 0;
//
//	for (row = T_game->top; row <= T_game->bottom; ++row)
//	{
//		for (col = T_game->left; col <= T_game->right; ++col)
//		{
//			if (T_game->chess[row][col].flag != SPACE)
//				continue;
//			if (BLACK_CHESS == T_game->cComputer && T_game->IsKinjite(row, col)) // ������Ľ��ֵ�
//				continue;
//
//			int res = T_game->IsCombForVC(row, col, T_game->cComputer);
//
//			if (res<3) // VCF����
//				continue;
//
//			vcPoint[cnt++].init(row, col, res);	//�ǽ��������ȱ���
//
//			if (cnt >= 100)
//			{
//				row = T_game->bottom + 1;
//				col = T_game->right + 1;
//			}
//		}
//	}
//
//	sort(vcPoint, vcPoint + cnt); //
//
//	for (tempDepth = VCDEPTH; tempDepth <= VCDEPTH_END; tempDepth += VCDEPTH_DIS)//��Ȳ��ϵ���
//	{
//		T_game->depthMM = tempDepth; //
//		for (int i = 0; i<cnt; ++i)
//		{
//			bool flag = false;
//			int ansDepth = 9999;
//			T_game->VCAttackTree(1, vcPoint[i].row, vcPoint[i].col, T_game->cComputer, T_game->cPeople, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);
//
//			if (flag) // �ҵ�һ����Ӯ������
//			{
//				ff = true; //
//				if (ansDepth<depthRecord) //ѡȡ�����Ӯ��
//				{
//					depthRecord = ansDepth;
//					tempX = vcPoint[i].row;
//					tempY = vcPoint[i].col;
//					maxScore = T_game->GetScoreForComputer(tempX, tempY, T_game->cComputer);
//					maxScore += T_game->GetScoreForComputer(tempX, tempY, T_game->cPeople);
//				}
//				else if (ansDepth == depthRecord) //���һ��ʱѡȡȨֵ���
//				{
//					int res = T_game->GetScoreForComputer(vcPoint[i].row, vcPoint[i].col, T_game->cComputer);
//					res += T_game->GetScoreForComputer(vcPoint[i].row, vcPoint[i].col, T_game->cPeople);
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
//			T_game->comX = tempX;
//			T_game->comY = tempY;
//			T_game->AIState = 1; //
//			return true;
//		}
//
//	}
//
//	// ���ж����Ƿ�ͨ�� VCF����ȡʤ�������ԣ������������� VCF��VCT�����Ϲ�������Ϊ������
//	// ����Ҫ�����ж� ���Ƿ񻹿��� ͨ�� VCF��VCT�����Ϲ���ȡʤ�����ϵ�������������
//	int peoIsVCF = false;
//	//////////////////////////////////////////////////////////////////////////
//	cnt = 0;
//
//	for (row = T_game->top; row <= T_game->bottom; ++row)
//	{
//		for (col = T_game->left; col <= T_game->right; ++col)
//		{
//			if (T_game->chess[row][col].flag != SPACE)
//				continue;
//			if (BLACK_CHESS == T_game->cPeople && T_game->IsKinjite(row, col)) // �˵Ľ��ֵ�
//				continue;
//
//			int res = T_game->IsCombForVC(row, col, T_game->cPeople);
//
//			if (res<3) // VCF����
//				continue;
//
//			vcPoint[cnt++].init(row, col, res);	//�ǽ��������ȱ���
//
//			if (cnt >= 100)
//			{
//				row = T_game->bottom + 1;
//				col = T_game->right + 1;
//			}
//		}
//	}
//
//	sort(vcPoint, vcPoint + cnt); //
//
//
//	for (tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//��Ȳ��ϵ���
//	{
//		T_game->depthMM = tempDepth; //
//		for (int i = 0; i<cnt; ++i)
//		{
//			bool flag = false;
//			int ansDepth = 9999;
//
//			T_game->VCAttackTree(1, vcPoint[i].row, vcPoint[i].col, T_game->cPeople, T_game->cComputer, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);
//
//			if (flag) // �ҵ�һ����Ӯ������
//			{
//				ff = true; //
//				i = cnt;
//				tempDepth = VCDE_DEPTH_END + 1;
//			}
//		}
//	}
//	if (ff)
//	{
//		peoIsVCF = true;
//		T_game->AIState = 2;
//	}
//
//	ff = false;
//
//	//�жϼ�����ܷ�ͨ�� VCF��VCT���Ϲ���ȡʤ,  ǰ�����˲��ܽ��� VCF����
//	//////////////////////////////////////////////////////////////////////////
//	if (!peoIsVCF)
//	{
//		cnt = 0;
//
//		for (row = T_game->top; row <= T_game->bottom; ++row)
//		{
//			for (col = T_game->left; col <= T_game->right; ++col)
//			{
//				if (T_game->chess[row][col].flag != SPACE)
//					continue;
//				if (BLACK_CHESS == T_game->cComputer && T_game->IsKinjite(row, col)) // ������Ľ��ֵ�
//					continue;
//
//				int res = T_game->IsCombForVC(row, col, T_game->cComputer);
//
//				if (!res) // VCF��VCT���Ϲ���
//					continue;
//
//				vcPoint[cnt++].init(row, col, res);	//�ǽ��������ȱ���
//
//				if (cnt >= 100)
//				{
//					row = T_game->bottom + 1;
//					col = T_game->right + 1;
//				}
//			}
//		}
//		sort(vcPoint, vcPoint + cnt); //
//
//		for (tempDepth = VCDEPTH; tempDepth <= VCDEPTH_END; tempDepth += VCDEPTH_DIS)//��Ȳ��ϵ���
//		{
//			T_game->depthMM = tempDepth; //
//			for (int i = 0; i<cnt; ++i)
//			{
//				bool flag = false;
//				int ansDepth = 9999;
//				T_game->VCAttackTree(2, vcPoint[i].row, vcPoint[i].col, T_game->cComputer, T_game->cPeople, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);
//
//				if (flag) //�ҵ�һ����Ӯ������
//				{
//					ff = true; //
//					if (ansDepth<depthRecord) //ѡȡ�����Ӯ��
//					{
//						depthRecord = ansDepth;
//						tempX = vcPoint[i].row;
//						tempY = vcPoint[i].col;
//						maxScore = T_game->GetScoreForComputer(tempX, tempY, T_game->cComputer);
//						maxScore += T_game->GetScoreForComputer(tempX, tempY, T_game->cPeople);
//					}
//					else if (ansDepth == depthRecord) //���һ��ʱѡȡȨֵ���
//					{
//						int res = T_game->GetScoreForComputer(vcPoint[i].row, vcPoint[i].col, T_game->cComputer);
//						res += T_game->GetScoreForComputer(vcPoint[i].row, vcPoint[i].col, T_game->cPeople);
//						if (res > maxScore)
//						{
//							maxScore = res;
//							tempX = vcPoint[i].row;
//							tempY = vcPoint[i].col;
//						}
//					}
//
//				}
//			}
//			if (ff)
//			{
//				T_game->comX = tempX;
//				T_game->comY = tempY;
//				T_game->AIState = 3;
//				return true;
//			}
//		}
//	}
//
//	//�ж����ܷ�ͨ�� VCF��VCT���Ϲ���ȡʤ��  VC ����
//	//////////////////////////////////////////////////////////////////////////
//	cnt = 0;
//	depthRecord = 9999;
//
//	for (row = T_game->top; row <= T_game->bottom; ++row)
//	{
//		for (col = T_game->left; col <= right; ++col)
//		{
//			if (chess[row][col].flag != SPACE)
//				continue;
//			if (BLACK_CHESS == cPeople && IsKinjite(row, col)) // �Լ��Ľ��ֵ�
//				continue;
//
//			int res = IsCombForVC(row, col, cPeople);
//
//			if (!res) // VCF��VCT���Ϲ���
//				continue;
//
//			vcPoint[cnt++].init(row, col, res);	//�ǽ��������ȱ���
//
//			if (cnt >= 100)
//			{
//				row = bottom + 1;
//				col = right + 1;
//			}
//		}
//	}
//
//	sort(vcPoint, vcPoint + cnt); //
//
//	for (tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//��Ȳ��ϵ���
//	{
//		depthMM = tempDepth; //
//		for (int i = 0; i<cnt; ++i)
//		{
//			bool flag = false;
//			int ansDepth = 9999;
//			isDefend = false;
//
//			VCAttackTree(2, vcPoint[i].row, vcPoint[i].col, cPeople, cComputer, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);
//
//			if (flag) // �ҵ�һ����Ӯ������
//			{
//				isDefend = true; //���أ���Ҫ��Ϊ�� ���ڶԷ������ķ���
//				ff = true; //		
//				if (ansDepth<depthRecord) //ѡȡ�����Ӯ��
//				{
//					depthRecord = ansDepth;
//					tempX = vcPoint[i].row;
//					tempY = vcPoint[i].col;
//					maxScore = GetScoreForComputer(tempX, tempY, cPeople);
//					maxScore += GetScoreForComputer(tempX, tempY, cComputer);
//				}
//				else if (ansDepth == depthRecord) //���һ��ʱѡȡȨֵ���
//				{
//					int res = GetScoreForComputer(vcPoint[i].row, vcPoint[i].col, cPeople);
//					res += GetScoreForComputer(vcPoint[i].row, vcPoint[i].col, cComputer);
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
//			// ���أ������ѡ�񹥻�λ�����ӣ���������Ϊ���˸õ��ʹ�˲������ VC ������
//			//////////////////////////////////////////////////////////////////////////
//			for (row = top; row <= bottom; ++row)
//			{
//				for (col = left; col <= right; ++col)
//				{
//					if (chess[row][col].flag != SPACE)
//						continue;
//					if (BLACK_CHESS == cComputer && IsKinjite(row, col))
//						continue;
//
//					int res1 = IsCombForVC(row, col, cComputer);
//					int res2 = IsCombForVC(row, col, cPeople);
//
//					if (!res1 && !res2)
//						continue;
//					else if ((3 == res1 || 4 == res1) && !res2) //һ������²�Ҫ���Լ���4������4
//						continue;
//
//					int res = GetScoreForComputer(row, col, cComputer);
//					res += GetScoreForComputer(row, col, cPeople);
//
//					dePoint[num++].init(row, col, res);
//				}
//			}
//
//			sort(dePoint, dePoint + num); //��Ȩֵ����
//
//			for (int p = 0; p<num; ++p)
//			{
//				bool noVC = true;
//
//				chess[dePoint[p].row][dePoint[p].col].flag = cComputer; //����������������
//
//																		// ����������ģ�� �˵� VC ����
//				cnt = 0;
//				depthRecord = 9999;
//
//				for (row = top; row <= bottom; ++row)
//				{
//					for (col = left; col <= right; ++col)
//					{
//						if (chess[row][col].flag != SPACE)
//							continue;
//						if (BLACK_CHESS == cPeople && IsKinjite(row, col)) // �˵Ľ��ֵ�
//							continue;
//
//						int res = IsCombForVC(row, col, cPeople);
//
//						if (!res) // VCF��VCT���Ϲ���
//							continue;
//
//						vcPoint[cnt++].init(row, col, res);	//�ǽ��������ȱ���
//
//						if (cnt >= 100)
//						{
//							row = bottom + 1;
//							col = right + 1;
//						}
//					}
//				}
//
//				sort(vcPoint, vcPoint + cnt); //
//
//
//				for (tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//��Ȳ��ϵ���
//				{
//					depthMM = tempDepth; //
//					for (int i = 0; i<cnt; ++i)
//					{
//						bool flag = false;
//						int ansDepth = 9999;
//						VCAttackTree(2, vcPoint[i].row, vcPoint[i].col, cPeople, cComputer, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);
//
//						if (flag) // �ҵ�����������
//						{
//							noVC = false;
//							i = cnt;
//							tempDepth = VCDE_DEPTH_END + 1;
//						}
//					}
//				}
//
//				chess[dePoint[p].row][dePoint[p].col].flag = SPACE; //�ָ�
//
//				if (noVC) //�����û�� VC ������
//				{
//					tempX = dePoint[p].row;
//					tempY = dePoint[p].col;
//					break;
//				}
//			}
//
//			//////////////////////////////////////////////////////////////////////////
//			comX = tempX;
//			comY = tempY;
//			AIState = 4;
//			return true;
//		}
//	}
//	return false;
//}