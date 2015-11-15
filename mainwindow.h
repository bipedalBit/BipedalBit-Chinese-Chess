#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SearchCore.h"
#include "MoveGenerator.h"
#include <QMainWindow>
#include <vector>
#include "QLabel"
#include "QMovie"

namespace Ui {
class MainWindow;
}

class Move;
class State;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_pushButton_back_clicked();

	void on_pushButton_start_clicked();

	void on_spinBox_max_depth_valueChanged(int depth);

	void on_comboBox_algorithm_currentIndexChanged(int index);

	void on_comboBox_first_currentIndexChanged(int index);

	void on_label_R_KING_clicked();

	void on_label_R_MANDARIN1_clicked();

	void on_label_R_MANDARIN2_clicked();

	void on_label_R_ELEPHANT1_clicked();

	void on_label_R_ELEPHANT2_clicked();

	void on_label_R_KNIGHT1_clicked();

	void on_label_R_KNIGHT2_clicked();

	void on_label_R_ROOK1_clicked();

	void on_label_R_ROOK2_clicked();

	void on_label_R_CANNON1_clicked();

	void on_label_R_CANNON2_clicked();

	void on_label_R_PAWN1_clicked();

	void on_label_R_PAWN2_clicked();

	void on_label_R_PAWN3_clicked();

	void on_label_R_PAWN4_clicked();

	void on_label_R_PAWN5_clicked();

	void on_label_B_KING_clicked();

	void on_label_B_MANDARIN1_clicked();

	void on_label_B_MANDARIN2_clicked();

	void on_label_B_ELEPHANT1_clicked();

	void on_label_B_ELEPHANT2_clicked();

	void on_label_B_KNIGHT1_clicked();

	void on_label_B_KNIGHT2_clicked();

	void on_label_B_ROOK1_clicked();

	void on_label_B_ROOK2_clicked();

	void on_label_B_CANNON1_clicked();

	void on_label_B_CANNON2_clicked();

	void on_label_B_PAWN1_clicked();

	void on_label_B_PAWN2_clicked();

	void on_label_B_PAWN3_clicked();

	void on_label_B_PAWN4_clicked();

	void on_label_B_PAWN5_clicked();

	void on_label_board_clicked(const QPoint &pos);

private:
	Ui::MainWindow *ui;
	/* 当前局面状态 */
	State state;
	/* 搜索核心指针 */
	SearchCore *searcher;
	/* 走法生成器指针，不为搜索服务，而是为了通过规则限制玩家的走法 */
	MoveGenerator *generator;
	/* 棋子横纵坐标基准值 */
	const int BASE_X = 59;
	const int BASE_Y = 30;
	/* 棋子选择标记横纵坐标基准值 */
	const int BASE_SELECT_X = 52;
	const int BASE_SELECT_Y = 23;
	/* 棋子横纵方向移动单位距离 */
	const int UNIT_DISTANCE = 59;
	/* 局面记录，为循环检测和局面回退提供方便 */
	std::vector<State> stateRecord;
	/* 棋子label列表 */
	std::vector<QLabel*> chessList;
	/* 玩家选定的棋子号 */
	int selectedChessNo;
	/* 玩家执红先手 */
	bool playRed;
	/* 玩家可以行动了 */
	bool playerTurn;
	/* 正在搜索图标gif动态过程 */
	QMovie *movie;
	/* 电脑获得主动权，执行一个回合 */
	void computerPlay();
	/* 根据当前局面更新棋盘 */
	void updateBoard();
	/* 玩家下棋前的准备工作 */
	void playPreparation();
	/* 根据玩家在棋子上的点击判断并执行玩家想做出的行动 */
	void chessClicked(int chessNo);
	/* 根据玩家在棋盘上的点击判断并执行玩家想做出的行动 */
	void boardClicked(int x, int y);
	/* 检查玩家的走法是否合法 */
	bool checkLegal(Move move);
	/* 游戏结束的处理 */
	void gameOver();
};

#endif // MAINWINDOW_H
