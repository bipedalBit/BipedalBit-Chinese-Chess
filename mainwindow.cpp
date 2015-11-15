#include "MTD_f.h"
#include "AlphaBeta.h"
#include "SerialGenerator.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sstream>
#include "QMessageBox"
#include "QtConcurrent/QtConcurrent"

class Move;
class Position;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	/* 初始化棋子label列表 */
	chessList.push_back(ui->label_R_KING);
	chessList.push_back(ui->label_R_MANDARIN1);
	chessList.push_back(ui->label_R_MANDARIN2);
	chessList.push_back(ui->label_R_ELEPHANT1);
	chessList.push_back(ui->label_R_ELEPHANT2);
	chessList.push_back(ui->label_R_KNIGHT1);
	chessList.push_back(ui->label_R_KNIGHT2);
	chessList.push_back(ui->label_R_ROOK1);
	chessList.push_back(ui->label_R_ROOK2);
	chessList.push_back(ui->label_R_CANNON1);
	chessList.push_back(ui->label_R_CANNON2);
	chessList.push_back(ui->label_R_PAWN1);
	chessList.push_back(ui->label_R_PAWN2);
	chessList.push_back(ui->label_R_PAWN3);
	chessList.push_back(ui->label_R_PAWN4);
	chessList.push_back(ui->label_R_PAWN5);
	chessList.push_back(ui->label_B_KING);
	chessList.push_back(ui->label_B_MANDARIN1);
	chessList.push_back(ui->label_B_MANDARIN2);
	chessList.push_back(ui->label_B_ELEPHANT1);
	chessList.push_back(ui->label_B_ELEPHANT2);
	chessList.push_back(ui->label_B_KNIGHT1);
	chessList.push_back(ui->label_B_KNIGHT2);
	chessList.push_back(ui->label_B_ROOK1);
	chessList.push_back(ui->label_B_ROOK2);
	chessList.push_back(ui->label_B_CANNON1);
	chessList.push_back(ui->label_B_CANNON2);
	chessList.push_back(ui->label_B_PAWN1);
	chessList.push_back(ui->label_B_PAWN2);
	chessList.push_back(ui->label_B_PAWN3);
	chessList.push_back(ui->label_B_PAWN4);
	chessList.push_back(ui->label_B_PAWN5);
	/* 未开局时隐藏棋子选定标记 */
	ui->label_select->hide();
	/* 按照初始局面更新棋盘 */
	updateBoard();
	/* 初始化玩家执子颜色标记 */
	ui->label_first->setText("玩家执红先手");
	ui->label_first->setStyleSheet("color: rgb(255, 0, 0);");
	playRed = true;
	/* 初始化玩家行动回合标记 */
	playerTurn = false;
	/* 初始化搜索核心 */
	searcher = new AlphaBeta(1);
	/* 初始化走法生成器 */
	generator = new SerialGenerator();
	/* 初始化玩家选定的棋子号 */
	selectedChessNo = NONE;
	/* 初始化正在搜索图标 */
	movie = new QMovie(":/img/loading.gif");
	ui->label_searching->setMovie(movie);
	movie->start();
	/* 隐藏正在搜索图标 */
	ui->label_searching->hide();
	/* 禁用回退按钮 */
	ui->pushButton_back->setEnabled(false);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_spinBox_max_depth_valueChanged(int depth)
{
	/* 修改搜索深度后销毁原有搜索核心，构造新的搜索核心 */
	if (ui->comboBox_algorithm->currentIndex() == 0)
	{
		delete searcher;
		searcher = new AlphaBeta(depth);
	}
	else if (ui->comboBox_algorithm->currentIndex() == 1)
	{
		delete searcher;
		searcher = new MTD_f(depth);
	}
}

void MainWindow::on_comboBox_algorithm_currentIndexChanged(int index)
{
	/* 获取搜索深度 */
	std::stringstream stream;
	int depth;
	stream << ui->spinBox_max_depth->text().toStdString();
	stream >> depth;
	/* 修改搜索算法后销毁原有搜索核心，构造新的搜索核心 */
	if (index == 0)
	{
		delete searcher;
		searcher = new AlphaBeta(depth);
	}
	else if (index == 1)
	{
		delete searcher;
		searcher = new MTD_f(depth);
	}
}

void MainWindow::on_comboBox_first_currentIndexChanged(int index)
{
	/* 玩家执红 */
	if (index == 0)
	{
		ui->label_first->setText("玩家执红先手");
		ui->label_first->setStyleSheet("color: rgb(255, 0, 0);");
		playRed = true;
	}
	/* 玩家执黑 */
	else
	{
		ui->label_first->setText("玩家执黑后手");
		ui->label_first->setStyleSheet("color: rgb(0, 0, 0);");
		playRed = false;
	}
}

void MainWindow::on_pushButton_back_clicked()
{
	/* 清空胜利／失败宣言 */
	ui->label_win->setText("");
	/* 回退局面 */
	stateRecord.pop_back();
	/* 更新局面 */
	state = stateRecord.back();
	/* 更新棋盘 */
	updateBoard();
	/* 暂时隐藏棋子选定标记 */
	ui->label_select->hide();
	/* 局面状态已空，无法回退 */
	if (stateRecord.empty())
	{
		ui->pushButton_back->setEnabled(false);
	}
	/* 局面状态非空，可以回退 */
	else
	{
		ui->pushButton_back->setEnabled(true);
	}
	/* 不选中任何棋子 */
	selectedChessNo = NONE;
	/* 清空走法容器 */
	generator->moveList.clear();
	/* 生成所有合法走法 */
	generator->generate(state, stateRecord);
}

void MainWindow::on_pushButton_start_clicked()
{
	/* 清空胜利宣言 */
	ui->label_win->setText("");
	/* 开局后不可以修改先后手设置 */
	ui->comboBox_first->setEnabled(false);
	/* 开局后“开始”按钮不可用 */
	ui->pushButton_start->setEnabled(false);
	/* 电脑先手 */
	if(ui->comboBox_first->currentIndex() != 0)
	{
		/* 显示正在搜索标记 */
		ui->label_searching->show();
		/* 开始属于电脑的回合时不允许修改搜索深度 */
		ui->spinBox_max_depth->setEnabled(false);
		/* 并行执行电脑回合 */
//		QFuture<void> future = QtConcurrent::run(this, &MainWindow::computerPlay);
		computerPlay();
		/* 阻塞主线程，等待子线程任务完成 */
//		future.waitForFinished();
	}
	/* 玩家获得行动权 */
	playerTurn = true;
	/* 做一些玩家行动前的准备工作 */
	playPreparation();
}

void MainWindow::computerPlay()
{
	/* 夺走玩家行动权 */
	playerTurn = false;
	/* 开始属于电脑的回合时不允许修改搜索算法 */
//	ui->spinBox_max_depth->setEnabled(false);
	ui->comboBox_algorithm->setEnabled(false);
	/* 搜索一个好走法 */
	searcher->search(state, stateRecord);
	/* 隐藏正在搜索图标 */
	ui->label_searching->hide();
	/* 根据搜索到的走法更新局面 */
	state.move(searcher->getBestMove());
	/* 根据新局面更新棋盘 */
	updateBoard();
	/* 电脑移动棋子的横纵坐标 */
	unsigned char x = searcher->getBestMove().to.x();
	unsigned char y = searcher->getBestMove().to.y();
	/* 更新棋子选择标记 */
	ui->label_select->show();
	ui->label_select->setGeometry(BASE_SELECT_X + x*UNIT_DISTANCE, BASE_SELECT_Y + y*UNIT_DISTANCE, 64, 64);
	/* 更新评估结点数 */
	ui->label_search_cnt->setText(QString::number(searcher->searchCnt));
	/* 更新搜索时间 */
	ui->label_search_time->setText(QString::number(searcher->searchTime, 'f') + "秒");
	/* 结束属于电脑的回合后允许修改搜索深度或搜索算法 */
	ui->spinBox_max_depth->setEnabled(true);
	ui->comboBox_algorithm->setEnabled(true);
	/* 归还玩家行动权 */
	playerTurn = true;
}

void MainWindow::updateBoard()
{
	/* 依次摆放棋子 */
	for (int i = 0; i < 32; ++i)
	{
		chessList[i]->setGeometry(BASE_X + state.x(i)*UNIT_DISTANCE, BASE_Y + state.y(i)*UNIT_DISTANCE, 50, 50);
	}
}

void MainWindow::playPreparation()
{
	/* 如果棋局结束 */
	if (state.isDone())
	{
		gameOver();
		return;
	}
	/* 局面状态已空，无法回退 */
	if (stateRecord.empty())
	{
		ui->pushButton_back->setEnabled(false);
	}
	/* 局面状态非空，可以回退 */
	else
	{
		ui->pushButton_back->setEnabled(true);
	}
	/* 记录新局面 */
	stateRecord.push_back(state);
	/* 不选中任何棋子 */
	selectedChessNo = NONE;
	/* 清空走法容器 */
	generator->moveList.clear();
	/* 生成所有合法走法 */
	generator->generate(state, stateRecord);
}

void MainWindow::chessClicked(int chessNo)
{
	/* 如果还没有选定要控制的棋子 */
	if (selectedChessNo == NONE)
	{
		/* 如果玩家点击了属于自己的棋子 */
		if (!(playRed ^ state.isRed(chessNo)))
		{
			/* 标记控制 */
			selectedChessNo = chessNo;
			ui->label_select->show();
			ui->label_select->setGeometry(
				BASE_SELECT_X + state.x(chessNo)*UNIT_DISTANCE,
				BASE_SELECT_Y + state.y(chessNo)*UNIT_DISTANCE,
				64,
				64
			);
		}
	}
	/* 如果是在控制棋子移动 */
	else
	{
		/* 如果点击了属于玩家自己的棋子，标记新的控制棋子 */
		if (state.isFriend(selectedChessNo, chessNo))
		{
			selectedChessNo = chessNo;
			ui->label_select->show();
			ui->label_select->setGeometry(
				BASE_SELECT_X + state.x(chessNo)*UNIT_DISTANCE,
				BASE_SELECT_Y + state.y(chessNo)*UNIT_DISTANCE,
				64,
				64
			);
			return;
		}
		/* 如果点击了敌方棋子，生成对应走法 */
		Move move(
			selectedChessNo,
			chessNo,
			Position(state.x(selectedChessNo), state.y(selectedChessNo)),
			Position(state.x(chessNo), state.y(chessNo))
		);
		/* 如果走法不合法，无操作 */
		if (!checkLegal(move))
		{
			return;
		}
		/* 如果走法合法 */
		if (!state.isFriend(selectedChessNo, chessNo))
		{
			/* 更新局面状态 */
			state.move(move);
			/* 移动棋子 */
			chessList[chessNo]->setGeometry(
				BASE_X + state.x(chessNo)*UNIT_DISTANCE,
				BASE_Y + state.y(chessNo)*UNIT_DISTANCE,
				50,
				50
			);
			/* 移动棋子选定标记 */
			ui->label_select->show();
			ui->label_select->setGeometry(
				BASE_SELECT_X + state.x(chessNo)*UNIT_DISTANCE,
				BASE_SELECT_Y + state.y(chessNo)*UNIT_DISTANCE,
				64,
				64
			);
			/* 如果棋局结束 */
			if (state.isDone())
			{
				gameOver();
				return;
			}
			/* 显示正在搜索标记 */
			ui->label_searching->show();
			/* 开始属于电脑的回合时不允许修改搜索深度 */
			ui->spinBox_max_depth->setEnabled(false);
			/* 并行执行电脑回合 */
//			QFuture<void> future = QtConcurrent::run(this, &MainWindow::computerPlay);
			computerPlay();
			/* 阻塞主线程，等待子线程任务完成 */
//			future.waitForFinished();
			/* 做一些玩家行动前的准备工作 */
			playPreparation();
		}
	}
}

void MainWindow::boardClicked(int x, int y)
{
	/* 也有很小可能点到棋子 */
	int chessNo = state.getNo(x, y);
	if (chessNo == NONE)
	{
		/* 已经选定要控制的棋子，点击空白区域才有效 */
		if (selectedChessNo != NONE)
		{
			/* 如果正在控制棋子移动，生成对应走法 */
			Move move(
				selectedChessNo,
				NONE,
				Position(state.x(selectedChessNo), state.y(selectedChessNo)),
				Position(x, y)
			);
			/* 如果走法合法 */
			if (checkLegal(move))
			{
				/* 更新局面状态 */
				state.move(move);
				/* 移动棋子 */
				chessList[selectedChessNo]->setGeometry(
					BASE_X + x*UNIT_DISTANCE,
					BASE_Y + y*UNIT_DISTANCE,
					50,
					50
				);
				/* 移动棋子选定标记 */
				ui->label_select->show();
				ui->label_select->setGeometry(
					BASE_SELECT_X + x*UNIT_DISTANCE,
					BASE_SELECT_Y + y*UNIT_DISTANCE,
					64,
					64
				);
				/* 显示正在搜索标记 */
				ui->label_searching->show();
				/* 开始属于电脑的回合时不允许修改搜索深度 */
				ui->spinBox_max_depth->setEnabled(false);
				/* 并行执行电脑回合 */
//				QFuture<void> future = QtConcurrent::run(this, &MainWindow::computerPlay);
				computerPlay();
				/* 阻塞主线程，等待子线程任务完成 */
//				future.waitForFinished();
				/* 做一些玩家行动前的准备工作 */
				playPreparation();
			}
		}
	}
	/* 如果点到棋子，交由棋子点击处理方法来处理 */
	else
	{
		chessClicked(chessNo);
	}
}

bool MainWindow::checkLegal(Move move)
{
	/* 检查走法合法性实际上就是查看合法走法容器中是否存在指定走法 */
	for (auto it : generator->moveList)
	{
		if (it == move)
		{
			return true;
		}
	}
	return false;
}

void MainWindow::gameOver()
{
	if ((playRed && state.isAlive(R_KING)) || (!playRed && state.isAlive(B_KING)))
	{
		ui->label_win->setText("你赢了！");
	}
	else
	{
		ui->label_win->setText("你输了！");
	}
	/* 清空局面记录 */
	stateRecord.clear();
	/* 生成新初始局面 */
	state = State();
	/* 初始化棋盘 */
	updateBoard();
	/* 不选中任何棋子 */
	selectedChessNo = NONE;
	/* 剥夺玩家行动权 */
	playerTurn = false;
	/* 初始化控件 */
	/* 隐藏棋子选定标记 */
	ui->label_select->hide();
	/* 清空搜索统计数据 */
	ui->label_search_cnt->setText("");
	ui->label_search_time->setText("");
	/* 允许选择先后手 */
	ui->comboBox_first->setEnabled(true);
	/* 不允许回退 */
	ui->pushButton_back->setEnabled(false);
	/* 允许开始棋局 */
	ui->pushButton_start->setEnabled(true);
}

void MainWindow::on_label_R_KING_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_KING);
	}
}

void MainWindow::on_label_R_MANDARIN1_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_MANDARIN);
	}
}

void MainWindow::on_label_R_MANDARIN2_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_MANDARIN + 1);
	}
}

void MainWindow::on_label_R_ELEPHANT1_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_ELEPHANT);
	}
}

void MainWindow::on_label_R_ELEPHANT2_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_ELEPHANT + 1);
	}
}

void MainWindow::on_label_R_KNIGHT1_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_KNIGHT);
	}
}

void MainWindow::on_label_R_KNIGHT2_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_KNIGHT + 1);
	}
}

void MainWindow::on_label_R_ROOK1_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_ROOK);
	}
}

void MainWindow::on_label_R_ROOK2_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_ROOK + 1);
	}
}

void MainWindow::on_label_R_CANNON1_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_CANNON);
	}
}

void MainWindow::on_label_R_CANNON2_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_CANNON + 1);
	}
}

void MainWindow::on_label_R_PAWN1_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_PAWN);
	}
}

void MainWindow::on_label_R_PAWN2_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_PAWN + 1);
	}
}

void MainWindow::on_label_R_PAWN3_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_PAWN + 2);
	}
}

void MainWindow::on_label_R_PAWN4_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_PAWN + 3);
	}
}

void MainWindow::on_label_R_PAWN5_clicked()
{
	if (playerTurn)
	{
		chessClicked(R_PAWN + 4);
	}
}

void MainWindow::on_label_B_KING_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_KING);
	}
}

void MainWindow::on_label_B_MANDARIN1_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_MANDARIN);
	}
}

void MainWindow::on_label_B_MANDARIN2_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_MANDARIN + 1);
	}
}

void MainWindow::on_label_B_ELEPHANT1_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_ELEPHANT);
	}
}

void MainWindow::on_label_B_ELEPHANT2_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_ELEPHANT + 1);
	}
}

void MainWindow::on_label_B_KNIGHT1_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_KNIGHT);
	}
}

void MainWindow::on_label_B_KNIGHT2_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_KNIGHT + 1);
	}
}

void MainWindow::on_label_B_ROOK1_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_ROOK);
	}
}

void MainWindow::on_label_B_ROOK2_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_ROOK + 1);
	}
}

void MainWindow::on_label_B_CANNON1_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_CANNON);
	}
}

void MainWindow::on_label_B_CANNON2_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_CANNON + 1);
	}
}

void MainWindow::on_label_B_PAWN1_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_PAWN);
	}
}

void MainWindow::on_label_B_PAWN2_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_PAWN + 1);
	}
}

void MainWindow::on_label_B_PAWN3_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_PAWN + 2);
	}
}

void MainWindow::on_label_B_PAWN4_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_PAWN + 3);
	}
}

void MainWindow::on_label_B_PAWN5_clicked()
{
	if (playerTurn)
	{
		chessClicked(B_PAWN + 4);
	}
}

void MainWindow::on_label_board_clicked(const QPoint &pos)
{
	if (playerTurn)
	{
		int x = (pos.x() + 20 - BASE_X)/UNIT_DISTANCE;
		int y = (pos.y() + 20 - BASE_Y)/UNIT_DISTANCE;
		boardClicked(x, y);
	}
}
