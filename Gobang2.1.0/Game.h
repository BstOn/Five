#ifndef GAME_H
#define GAME_H
#include <QVector>
#include <utility>
using namespace std;

#define WHITE_CHESS 5
#define BLACK_CHESS 4
#define SPACE 3
#define SIZE 15
#define VCDEPTH 4  // AI ���� VC �����Ŀ�ʼ���
#define VCDEPTH_END 8  // AI ����VC������������
#define VCDEPTH_DIS 2  // AI ���� VC ��������� ����ֵ

#define VCDE_DEPTH 4  // AI ���� VC �����Ŀ�ʼ���
#define VCDE_DEPTH_END 8 // AI ���� VC ������������
#define VCDE_DEPTH_DIS 2 // AI ���� VC ��������� ����ֵ

typedef struct
{
	int x, y, l, r;
}value;

typedef struct
{
	int flag;  //��ɫ
	int seq;//����
}Chess;

typedef struct 
{
	int type[4];
	int dotN;
	int n;
	int w[2];    //������ǿ����
	int p[40][2];
}ChessType;

typedef struct
{
	int x;
	int y;
	int score;
}poinT;

typedef struct Message
{
	int type;
	int depth;
	int cnt;
	int score;
}Message;

typedef struct
{
	int left;
	int right;
}Count;

//�û���
typedef struct tagHASHE {
	_int64 key;
	int depth;
	int flags;//������ɫ
	int value;
} HASHE;
struct VCPoint  // VC����ʱ�Ĺ�����,�ɻ�4,��4,����4,��3,��3,���,���ȼ����εݼ�
{
	int row;
	int col;
	int pri;  // ���ȼ�
	friend bool operator < (const VCPoint &a, const VCPoint &b)
	{
		return a.pri > b.pri;
	}
	void init(int _r, int _c, int _p)
	{
		row = _r, col = _c, pri = _p;
	}
};
class Game
{
public:
	Game();
	Chess chess[15][15];
	Chess chess_B[15][15];   //����

	ChessType chessType[26];

	Message message;

	int *times;
	int timeo;
	int player;
	int dadianN;
	int dadiann;
	bool dafou;      //�ж��Ƿ�ѡ���˴��
	bool judge(int x, int y);
	bool ifWin(int x, int y);
	void backChess();
	int sta();
	int aspect;    //���ַ���
	int kaijun;    //���ֿ����
	int cComputer;  //����ִ����ɫ
	int cPeople;
	void initiChessType();
	HASHE*Hashtable[6];
	unsigned int m_nHashKey32[6][15][15];          //32λ������飬��������32λ��ϣֵ
	unsigned __int64 m_ulHashKey64[6][15][15];//64λ������飬��������64λ��ϣֵ
	unsigned int m_HashKey32;          //32λ��ϣֵ
	_int64 m_HashKey64;
	QPoint peoplePutdown(int x, int y);
	QPoint computerPutdown(int x, int y);
	QVector<QPoint> chessPoint;
	bool AIkongzhi;
	bool waitThread;
	bool waitBackAI;
private:


	//aiϵͳ   ��һ��ȫ������
public:
	void AI();
	int isDefend; //�ж��Ƿ���� VC����ʱʹ��
				  /* ����״̬
				  0 : ��������
				  1 ����ʾVCF����
				  2 ����ʾ��ֹVCF����
				  3 ����ʾVCF��VCT���Ϲ���
				  */
	int AIState;  // �� VC����ʱ��ֵ
	int comX, comY;
	int depthMM; // ��ȿ���

	void LayOut();//����
	void ComputerPlay();
	void PeoplePlay();
	int currentX, currentY;
	bool change();
	void Nda(double x1[15], double y1[15], int n);    //��N�����
	void daN();    //��N������� ѡ��һ��
	int kaiju(int i1, int j1, int i2, int j2, int n);
	void judge_Aspect(int i1, int j1, int i2, int j2);
	bool judge_Neighbored(int x, int y);
	poinT aspect_count(int i, int j);
	int heuristic(int left1, int right1, int top1, int bottom1, int flag, poinT result[11]);  //����ʽ����

	//�ڷ������µĹؼ��ĵ����Ӽ��γ�������ͬʱ,���γɽ���,��ʱ����ʧЧ,����ʤ
	//����1˵������Ǻ���ĳ�������,����2˵�������������Ľ���,�����ǽ���
	/*int IsKinjite(int row, int col);*/
	void UpdateLimit();//�����������ƿ�
	void Chang_UpddateLimit(int &left_temp, int& right_temp, int &top_temp, int &bottom_temp, int X, int Y);
	bool judge_IsKinjite(int row, int col);

	//�������ж�
	void Search(int row, int col, int chessFlag);
	void Findcount(int row, int col, int chessFlag, Count count[4]);
	void GetCheng5_New(int chessFlag, Count count);
	void GetHuo4Chong4_New(int row, int col, int n, Count count, int chessFlag, int left[5], int right[5]);
	void GetLian3Chong3TiaoChong4_New(int row, int col, int n, Count count, int chessFlag, int left[5], int right[5]);
	void GetLian2Tiao3Chong3TiaoChong4_New(int row, int col, int n, Count count, int chessFlag, int left[5], int right[5]);
	void GetTiao2Tiao3Tiaochong4_New(int row, int col, int n, Count count, int chessFlag, int left[5], int right[5]);
	poinT DirectionCount_New(int x, int y, int offset, int n);


	bool Judge_IsChangLian_New(Count count);
	int Judge_IsDoubleThree_New(int row, int col, int n, Count count, int left[5], int right[5]);
	int Judge_IsDoubleFour_New(int row, int col, int n, Count count, int left[5], int right[5]);
	bool IsKinjite(int row, int col);
	bool Judge_IsHuo4ShuangChong4(int row, int col, int chessFlag);
	bool GetHuo4ShuangChong4(int row, int col, int chessFlag, Count count, int left[5], int right[5]);
	bool Judge_IsSymmetry(double &x, double &y);

	//��ϣ��
	void _CTranspositionTable();
	void CTranspositionTable();
	void CalculateInitHashKey();



	QPoint ceshizhuanyong(int x, int y);
	void BackstageAI();
	bool huiqijudge;
	bool depthkongzhi;

private:
	pair<int, int> GetCurrentMaxPoint(int chessFlag);//�õ������ص�ǰ��ֵ���� chessFlag ����ͬ�Ƕ�
	int mustWin;   //��¼�Ƿ��б�Ӯ��
	int winPeople, winComputer;  //ͬ���Ǽ�¼��Ӯ��,��¼��˭�ı�Ӯ��
	int peoX, peoY;
	int computer[SIZE][SIZE], people[SIZE][SIZE];//��¼δ�¹��ĵ���ڼ�������˵ļ�ֵ
	bool VCAttack(); // �ж��Ƿ���Խ���VC����
	void ControlDepth(); //�жϾ����������


	void InitGameTree(int row, int col, bool& flag, int depth, int& depthC, int& depthP, bool& stop, int depthRecord);
	// GameTree�Ĳ�����,flag�ֱ��ʾ������Ƿ��ҵ���Ӯ��,
	// ���б�Ӯ��,��depthC,depthP��ʾ(�����,��)��Ӯ�����С���
	// �������ֻ�ǳ���ɸѡ
	// InitGameTree���� �� AgainGameTree��) �ĺ�������,һ�����ڼ������,һ�������˵ġ�
	int Evaluate(int left1, int right1, int top1, int bottom1);
	void MaxMin(int depth);//����С������
	int Max(int left1, int right1, int top1, int bottom1, int row, int col, int depth, int alpha, int beta);//�����
	int Min(int left1, int right1, int top1, int bottom1, int row, int col, int depth, int alpha, int beta);//��С��
	int Evaluate_Value(int chessFlag,int left1, int right1, int top1, int bottom1);//�жϾ���ֵ
	void InitializeHashKey();
	void EnterHashTable(int flag, int eval, int  depth, int chessFlag);
	int LookUpHashTable(int alpha, int beta, int depth, int flag);
	void Hash_MakeMove(int row, int col, int chessFlag);
	void Hash_UnMakeMove(int row, int col, int chessFlag);
	
	//haxi
	__int64 rand64();
	long rand32();
	int left=5, right=5, top=9, bottom=9;
	//	������������ص�,���Բ���һ���������ƿ�,��¼��ǰ�����������ӵ�����
	//	���ҡ����ϡ����µ㹹�ɵľ���,���һ���������Ϊ��һ�����λ�ò������������3������.

	bool PeopleAttack(int r, int c, int depthCFromIGT); //ģ���˹���

														//�õ� chessMap[row][col] ������ chessFlag ɫ����ļ�ֵ
	int GetScoreForComputer(int row, int col, char chessFlag);
	int GetScoreForPeople(int row, int col, char chessFlag);

	int IsCombForAllVC(int row, int col, int oneself, int oppsite); // �Ƿ񹹳ɿ���VC����
	int IsCombForOneselfVc(int row, int col, int oneself);
	void VCAttackTree(int type, int row, int col, char cOneself, char cOpposite, int depth, bool& flag, int& ansDepth, int depthRecord, int left1, int right1, int top1, int bottom1); // VC ������
	/*bool Max_Vc(int left1, int right1, int top1, int bottom1, int row, int col, int depth, int oneself, int oppsite, int max, int last);
	bool Min_Vc(int left1, int right1, int top1, int bottom1, int row, int col, int depth, int oneself, int oppsite, int max, int last);*/
	int FindVcPoint(int left1, int right1, int top1, int bottom1, int oneself, int oppsite, VCPoint p[]);
	int FindVcOneselfPoint(int left1, int right1, int top1, int bottom1, int oneself, VCPoint p[]);
	int FindVc(VCPoint p[], VCPoint opp[], int num, int numopp, int last, VCPoint all[], int oneself, int oppsite);
	bool MaxMin_Vc(int depth);
	bool Iteration();

	int Game::IsCombForVC(int row, int col, int oneself, int oppsite);
	bool Game::Max_Vc(int left1, int right1, int top1, int bottom1, int row, int col, int depth, int oneself, int oppsite, int max);
	bool Game::Min_Vc(int left1, int right1, int top1, int bottom1, int row, int col, int depth, int oneself, int oppsite, int max);
	int Game::IsCombForVcf(int row, int col, int oneself);
	int Game::FindVcOneselfPoint(int left1, int right1, int top1, int bottom1, int oneself, int oppsite, VCPoint p[]);
	int Game::IsCombForVC(int row, int col, char chessFlag);
	int DEPTH; // �� AI ��̬���� InitGameTree ���������
	//void Search(int row, int col, char chessFlag);//���� chessMap[row][col] ����ܹ���� chessFlag ɫ�����ܹ�������Щ����
	void AgainGameTree(int row, int col, int depth, int& depthC, int& depthP, bool& stop, int& depthRecord);
	int GetScore(int row, int col, char chessFlag, char flag);
	int AddScore(int row, int col, char chessFlag);

	//�ж�����
	pair<int, pair<int, int> > GetChong3Chong4(int row, int col, char chessFlag);// ������chessMap[row][col] ���Ӻ�,���ع��ɳ�3�ͳ�4(chong4��tiaochong4)������ 
	pair<int, int> GetLian2Lian3(int row, int col, char chessFlag);// ������chessMap[row][col] ���Ӻ�,���ع�����2����3������ 
	pair<int, int> GetTiao2Tiao3(int row, int col, char chessFlag);// ������chessMap[row][col] ���Ӻ�,���ع�����2����3������ 
	int GetCheng5ChangLian(int row, int col, char chessFlag);// ������chessMap[row][col] ���Ӻ�,�Ƿ��5������),����1��ʾ����,����2��ʾ����,���򷵻�0 
	int GetHuo4(int row, int col, char chessFlag);//������chessMap[row][col] ���Ӻ�,���ع��ɻ�4������ 

	int changlian, cheng5, huo4, chong4, tiaochong4, lian3, tiao3, chong3, lian2, tiao2;
	int chong2 = 0, huo1 = 0, chong1 = 0;

};



#endif // GAME_H









