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
	// 判断是否有可以进行 VC 攻击的点
	// 先判断计算机能否 VCF攻击取胜，再判断人是否通过VCF攻击取胜，然后再判断计算机是否通过VCF和VCT联合
	//攻击取胜，也判断人的VCF和VCT联合攻击(虽然慢一点).
	// 为了提高 棋力， 不断的尝试各个深度，先从小的深度开始搜素，这样就能保证以最小的步数赢棋，
	// 一旦找到必赢点，立即返回
	// 目前仍然有bug，很难在速度和棋力之间权衡

	// VC 攻击，活4，冲4(冲4和跳冲4)，活3（连3和跳3），优先级依次降低

	int row, col;
	VCPoint vcPoint[100]; //
	int cnt;
	int tempDepth; //深度控制

	VCPoint dePoint[100]; //用于 VC 防守，在下面的模拟人的攻击中保存攻击点
	int num = 0; //

	int tempX, tempY, depthRecord = 9999, maxScore = -9999;
	bool ff = false;
	int left1 = left, right1 = right, bottom1 = bottom, top1 = top;
	// 先判断计算机是否可进行 VCF 攻击,  可以则返回

	cnt = 0;
	message.cnt = cnt;

	message.type = 1;
	for (row = top; row <= bottom; ++row)
	{
		for (col = left; col <= right; ++col)
		{
			if (chess[row][col].flag != SPACE)
				continue;
			if (BLACK_CHESS == cComputer && IsKinjite(row, col)) // 计算机的禁手点
				continue;

			int res = IsCombForVC(row, col, cComputer);

			if (res<3) // VCF攻击
				continue;

			vcPoint[cnt++].init(row, col, res);	//是进攻点则先保存

			if (cnt >= 100)
			{
				row = bottom + 1;
				col = right + 1;
			}
		}
	}

	sort(vcPoint, vcPoint + cnt); //
	message.cnt = cnt;
	for (tempDepth = VCDEPTH; tempDepth <= VCDEPTH_END; tempDepth += VCDEPTH_DIS)//深度不断递增
	{
		depthMM = tempDepth; //
		for (int i = 0; i<cnt; ++i)
		{
			bool flag = false;
			int ansDepth = 9999;
			VCAttackTree(1, vcPoint[i].row, vcPoint[i].col, cComputer, cPeople, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);

			if (flag) // 找到一个必赢攻击点
			{
				ff = true; //
				if (ansDepth<depthRecord) //选取最快能赢的
				{
					depthRecord = ansDepth;
					tempX = vcPoint[i].row;
					tempY = vcPoint[i].col;
					maxScore = GetScoreForComputer(tempX, tempY, cComputer);
					maxScore += GetScoreForComputer(tempX, tempY, cPeople);
				}
				else if (ansDepth == depthRecord) //深度一样时选取权值大的
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
	// 再判断人是否通过 VCF攻击取胜，若可以，则计算机不进行 VCF和VCT的联合攻击，因为会先输
	// 但还要继续判断 人是否还可以 通过 VCF和VCT的联合攻击取胜，这关系到计算机的落子
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
			if (BLACK_CHESS == cPeople && IsKinjite(row, col)) // 人的禁手点
				continue;

			int res = IsCombForVC(row, col, cPeople);

			if (res<3) // VCF攻击
				continue;

			vcPoint[cnt++].init(row, col, res);	//是进攻点则先保存
			if (cnt >= 100)
			{
				row = bottom + 1;
				col = right + 1;
			}
		}
	}
	message.cnt = cnt;
	sort(vcPoint, vcPoint + cnt); //


	for (tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//深度不断递增
	{
		depthMM = tempDepth; //
		for (int i = 0; i<cnt; ++i)
		{
			bool flag = false;
			int ansDepth = 9999;

			VCAttackTree(1, vcPoint[i].row, vcPoint[i].col, cPeople, cComputer, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);

			if (flag) // 找到一个必赢攻击点
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
	//判断计算机能否通过 VCF和VCT联合攻击取胜,  前提是人不能进行 VCF攻击
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
	//			if (BLACK_CHESS == cComputer && IsKinjite(row, col)) // 计算机的禁手点
	//				continue;

	//			int res = IsCombForVC(row, col, cComputer);

	//			if (!res) // VCF和VCT联合攻击
	//				continue;

	//			vcPoint[cnt++].init(row, col, res);	//是进攻点则先保存

	//			if (cnt >= 100)
	//			{
	//				row = bottom + 1;
	//				col = right + 1;
	//			}
	//		}
	//	}
	//	sort(vcPoint, vcPoint + cnt); //
	//	message.cnt = cnt;
	//	for (tempDepth = VCDEPTH; tempDepth <= VCDEPTH_END; tempDepth += VCDEPTH_DIS)//深度不断递增
	//	{
	//		depthMM = tempDepth; //
	//		for (int i = 0; i<cnt; ++i)
	//		{
	//			bool flag = false;
	//			int ansDepth = 9999;
	//			VCAttackTree(2, vcPoint[i].row, vcPoint[i].col, cComputer, cPeople, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);

	//			if (flag) //找到一个必赢攻击点
	//			{
	//				ff = true; //
	//				if (ansDepth<depthRecord) //选取最快能赢的
	//				{
	//					depthRecord = ansDepth;
	//					tempX = vcPoint[i].row;
	//					tempY = vcPoint[i].col;
	//					maxScore = GetScoreForComputer(tempX, tempY, cComputer);
	//					maxScore += GetScoreForComputer(tempX, tempY, cPeople);
	//				}
	//				else if (ansDepth == depthRecord) //深度一样时选取权值大的
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
	//判断人能否通过 VCF和VCT联合攻击取胜，  VC 防御
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
	//		if (BLACK_CHESS == cPeople && IsKinjite(row, col)) // 自己的禁手点
	//			continue;

	//		int res = IsCombForVC(row, col, cPeople);

	//		if (!res) // VCF和VCT联合攻击
	//			continue;

	//		vcPoint[cnt++].init(row, col, res);	//是进攻点则先保存

	//		if (cnt >= 100)
	//		{
	//			row = bottom + 1;
	//			col = right + 1;
	//		}
	//	}
	//}

	//sort(vcPoint, vcPoint + cnt); //
	//message.cnt = cnt;
	//for (tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//深度不断递增
	//{
	//	depthMM = tempDepth; //
	//	for (int i = 0; i<cnt; ++i)
	//	{
	//		bool flag = false;
	//		int ansDepth = 9999;
	//		isDefend = false;

	//		VCAttackTree(2, vcPoint[i].row, vcPoint[i].col, cPeople, cComputer, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);

	//		if (flag) // 找到一个必赢攻击点
	//		{
	//			isDefend = true; //防守，主要是为了 防在对方进攻的方向
	//			ff = true; //		
	//			if (ansDepth<depthRecord) //选取最快能赢的
	//			{
	//				depthRecord = ansDepth;
	//				tempX = vcPoint[i].row;
	//				tempY = vcPoint[i].col;
	//				maxScore = GetScoreForComputer(tempX, tempY, cPeople);
	//				maxScore += GetScoreForComputer(tempX, tempY, cComputer);
	//			}
	//			else if (ansDepth == depthRecord) //深度一样时选取权值大的
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
	//		// 防守，计算机选择攻击位置落子，落子条件为下了该点后使人不会产生 VC 攻击点
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
	//				else if ((3 == res1 || 4 == res1) && !res2) //一般情况下不要下自己冲4和跳冲4
	//					continue;

	//				int res = GetScoreForComputer(row, col, cComputer);
	//				res += GetScoreForComputer(row, col, cPeople);

	//				dePoint[num++].init(row, col, res);
	//			}
	//		}

	//		sort(dePoint, dePoint + num); //以权值排序
	//		message.cnt = num;
	//		for (int p = 0; p<num; ++p)
	//		{
	//			bool noVC = true;

	//			chess[dePoint[p].row][dePoint[p].col].flag = cComputer; //假设计算机在这落子

	//																	// 下面依旧是模拟 人的 VC 攻击
	//			cnt = 0;          
	//			depthRecord = 9999;
	//			message.cnt = cnt;
	//			for (row = top; row <= bottom; ++row)
	//			{
	//				for (col = left; col <= right; ++col)
	//				{
	//					if (chess[row][col].flag != SPACE)
	//						continue;
	//					if (BLACK_CHESS == cPeople && IsKinjite(row, col)) // 人的禁手点
	//						continue;

	//					int res = IsCombForVC(row, col, cPeople);

	//					if (!res) // VCF和VCT联合攻击
	//						continue;

	//					vcPoint[cnt++].init(row, col, res);	//是进攻点则先保存

	//					if (cnt >= 100)
	//					{
	//						row = bottom + 1;
	//						col = right + 1;
	//					}
	//				}
	//			}

	//			sort(vcPoint, vcPoint + cnt); //
	//			message.cnt = cnt;

	//			for (tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//深度不断递增
	//			{
	//				depthMM = tempDepth; //
	//				for (int i = 0; i<cnt; ++i)
	//				{
	//					bool flag = false;
	//					int ansDepth = 9999;
	//					VCAttackTree(2, vcPoint[i].row, vcPoint[i].col, cPeople, cComputer, 0, flag, ansDepth, depthRecord, left1, right1, top1, bottom1);

	//					if (flag) // 找到便立即跳出
	//					{
	//						noVC = false;
	//						i = cnt;
	//						tempDepth = VCDE_DEPTH_END + 1;
	//					}
	//				}
	//			}

	//			chess[dePoint[p].row][dePoint[p].col].flag = SPACE; //恢复

	//			if (noVC) //如果人没有 VC 攻击点
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
	// 深度控制,深度太深的话，复杂度就太高了，depthRecord用来记录已经搜过的点中达必赢点的最小深度
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
	if (flag) // flag 为 true 时，表示可以必赢，能必赢则返回，不需要去找最优的，否则太慢
	{
		ansDepth = depth; //记录多少步能赢
		return;
	}
	chess[row][col].flag = cOneself; // 自己落子

	int oppositeX, oppositeY;
	int winOpposite = 0;
	int dd = 1; // 深度的递增值

				//////////////////////对方思考/////////////////////////////////////////////
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

	// 如果对方在防守的同时 也构成了冲4或活3，那么自己又需要反过来防守对方，此时应该将深度加1
	if (1 == type) // VCF 攻击
	{
		pair<int, pair<int, int> > ppi = GetChong3Chong4(oppositeX, oppositeY, cOpposite);
		if (ppi.second.first + ppi.second.second) //构成冲4或跳冲4
			dd = 2;

		//		if(ppi.second.first + ppi.second.second >=2 ) //
		//			winOpposite = 1;
	}
	else if (2 == type) // VCF 和 VCT 联合攻击
	{
		pair<int, pair<int, int> > ppi = GetChong3Chong4(oppositeX, oppositeY, cOpposite);
		if (ppi.second.first + ppi.second.second) //构成冲4或跳冲4
			dd = 2;
		pair<int, int> pi1 = GetLian2Lian3(oppositeX, oppositeY, cOpposite);
		if (pi1.second) //构成连3
			dd = 2;
		pair<int, int> pi2 = GetTiao2Tiao3(oppositeX, oppositeY, cOpposite);
		if (pi2.second) //构成跳3
			dd = 2;

		//		if(ppi.second.first+ppi.second.second + pi1.second+pi2.second >=2 ) //
		//			winOpposite = 1;
	}
	//////////////////////////////////////////////////////////////////////////
	Chang_UpddateLimit(left1, right1, top1, bottom1, oppositeX, oppositeY);
	int left_temp = left1, right_temp = right1, top_temp = top1, bottom_temp = bottom1;
	if ((oppositeX != -1 && oppositeY != -1) && (winOpposite != 1)) // 棋盘未满 且 对方 未成五
	{
		if (cOpposite == BLACK_CHESS && IsKinjite(oppositeX, oppositeY)) // 该点是对方的禁手
			flag = true;
		else
		{
			chess[oppositeX][oppositeY].flag = cOpposite; // 对方落子

			VCPoint vcPoint[100];// 保存活3，冲4，活4这些点
			int cnt = 0;
			int i, j;

			for (i = top_temp; i <bottom_temp; ++i)
			{
				for (j = left_temp; j <right_temp; ++j)
				{
					if (chess[i][j].flag != SPACE)
						continue;
					if (BLACK_CHESS == cOneself && IsKinjite(i, j)) // 自己的禁手点
						continue;

					int res = IsCombForVC(i, j, cOneself);
					//是攻击点则保存,攻击类型分2种，VCF，和VCF、VCT联合攻击
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

			sort(vcPoint, vcPoint + cnt);  //按优先级排序

			for (i = 0; i<cnt; ++i)
			{
				if (!flag)
				{
					VCAttackTree(type, vcPoint[i].row, vcPoint[i].col, cOneself, cOpposite, depth + dd, flag, ansDepth, depthRecord, left_temp, right_temp, top_temp, bottom_temp); //
				}
			}

			chess[oppositeX][oppositeY].flag = SPACE; // 恢复
		}
	}
	chess[row][col].flag = SPACE; //恢复
}
int Game::IsCombForVC(int row, int col, char chessFlag)
{
	//判断是否是 VC 攻击的攻击点，返回其优先级
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
