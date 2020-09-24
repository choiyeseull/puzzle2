#include <bangtal.h>
#include <iostream>
#include <ctime>

using namespace bangtal;
using namespace std;

auto timer = Timer::create(0.1f);
clock_t start_time, end_time;

int main() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);


	//1. 시작화면 : 게임선택
	auto menu = Scene::create("게임선택", "images/menu.png");

	//1. 시작화면 : 선택 토토로, 포뇨 사진
	auto totoro = Object::create("images/totoro.png", menu, 80, 150);
	auto ponyo = Object::create("images/ponyo.png", menu, 700, 150);

	//2. 포뇨게임, 토토로게임 장면 생성
	auto tpuzzle = Scene::create("토토로퍼즐", "images/playbackground2.png");
	auto ppuzzle = Scene::create("포뇨퍼즐", "images/playbackground.png");

	ObjectPtr game_board[16];
	ObjectPtr init_board[16];
	int blank = 15;

	srand((unsigned int)time(NULL));

	//2. 토토로 클릭하면 토토로 게임 실행 
	totoro->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		tpuzzle->enter();
		start_time = clock();
		timer->start();

		//i는 index
		for (int i = 0; i < 16; i++) {
			string filename = "images/totoro_" + to_string(i + 1) + ".png";
			printf("%d", select_game);
			game_board[i] = Object::create(filename, tpuzzle, 213 + (i % 4) * 213, 480 - (i / 4) * 120);
			init_board[i] = game_board[i];
			game_board[i]->setOnMouseCallback([&](ObjectPtr object, int, int, MouseAction)->bool {

				//1. j는 클릭한 블록의 index
				int j;
				for (j = 0; j < 16; j++) {
					if (game_board[j] == object)
						break;
				}

				if ((j % 4 > 0 && j - 1 == blank) ||
					(j % 4 < 3 && j + 1 == blank) ||
					(j > 3 && j - 4 == blank) ||
					(j < 12 && j + 4 == blank))
				{
					//3. 그림도 바꿈
					game_board[j]->locate(tpuzzle, 213 + (blank % 4) * 213, 480 - (blank / 4) * 120);
					game_board[blank]->locate(tpuzzle, 213 + (j % 4) * 213, 480 - (j / 4) * 120);
					//2. blank랑 j를 바꿈
					game_board[j] = game_board[blank];
					game_board[blank] = object;
					blank = j;

					int k;
					for (k = 0; k < 16; k++) {
						if (game_board[k] != init_board[k]) break;
					}
					if (k == 16) {
						//4.완성하면 시간출력
						end_time = clock();

						int do_time = (end_time - start_time) / CLOCKS_PER_SEC;
						char s1[100], s2[50];

						sprintf_s(s1, "%d", do_time / 60);	//정수를 문자열로 바꾸기
						strcat_s(s1, "분");
						sprintf_s(s2, "%d", do_time % 60);
						strcat_s(s2, "초");
						strcat_s(s1, s2);
						showMessage(s1);
					}
				}
				return true;
				});
		}

		game_board[blank]->hide();

		//랜덤섞기
		auto count = 0;

		timer->setOnTimerCallback([&](TimerPtr t)->bool {
			int j = 0;
			do {
				switch (rand() % 4) {
				case 0: j = blank - 1; break;
				case 1: j = blank + 1; break;
				case 2: j = blank - 4; break;
				case 3: j = blank + 4; break;
				}
				//안움직이는 조건
			} while (j < 0 || j > 15 || !((j % 4 > 0 && j - 1 == blank) ||
				(j % 4 < 3 && j + 1 == blank) ||
				(j > 3 && j - 4 == blank) ||
				(j < 12 && j + 4 == blank)));

			game_board[j]->locate(tpuzzle, 213 + (blank % 4) * 213, 480 - (blank / 4) * 120);
			game_board[blank]->locate(tpuzzle, 213 + (j % 4) * 213, 480 - (j / 4) * 120);

			auto object = game_board[j];
			game_board[j] = game_board[blank];
			game_board[blank] = object;
			blank = j;

			count++;

			if (count < 100) {
				t->set(0.1f);
				t->start();
			}
			return true;
			});

		return true;
		});

	//2. 포뇨 클릭하면 포뇨 게임 실행 
	ponyo->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		ppuzzle->enter();
		start_time = clock();
		timer->start();
		//i는 index
		for (int i = 0; i < 16; i++) {
			string filename = "images/" + to_string(i + 1) + ".png";
			printf("%d", select_game);
			game_board[i] = Object::create(filename, ppuzzle, 213 + (i % 4) * 213, 480 - (i / 4) * 120);
			init_board[i] = game_board[i];
			game_board[i]->setOnMouseCallback([&](ObjectPtr object, int, int, MouseAction)->bool {

				//1. j는 클릭한 블록의 index
				int j;
				for (j = 0; j < 16; j++) {
					if (game_board[j] == object)
						break;
				}

				if ((j % 4 > 0 && j - 1 == blank) ||
					(j % 4 < 3 && j + 1 == blank) ||
					(j > 3 && j - 4 == blank) ||
					(j < 12 && j + 4 == blank))
				{
					//3. 그림도 바꿈
					game_board[j]->locate(ppuzzle, 213 + (blank % 4) * 213, 480 - (blank / 4) * 120);
					game_board[blank]->locate(ppuzzle, 213 + (j % 4) * 213, 480 - (j / 4) * 120);
					//2. blank랑 j를 바꿈
					game_board[j] = game_board[blank];
					game_board[blank] = object;
					blank = j;

					int k;
					for (k = 0; k < 16; k++) {
						if (game_board[k] != init_board[k]) break;
					}
					if (k == 16) {
						//4.완성하면 시간출력
						end_time = clock();

						int do_time = (end_time - start_time) / CLOCKS_PER_SEC;
						char s1[100], s2[50];

						sprintf_s(s1, "%d", do_time / 60);	//정수를 문자열로 바꾸기
						strcat_s(s1, "분");
						sprintf_s(s2, "%d", do_time % 60);
						strcat_s(s2, "초");
						strcat_s(s1, s2);
						showMessage(s1);
					}
				}
				return true;
				});
		}

		game_board[blank]->hide();

		//랜덤섞기
		auto count = 0;

		timer->setOnTimerCallback([&](TimerPtr t)->bool {
			int j = 0;
			do {
				switch (rand() % 4) {
				case 0: j = blank - 1; break;
				case 1: j = blank + 1; break;
				case 2: j = blank - 4; break;
				case 3: j = blank + 4; break;
				}
				//안움직이는 조건
			} while (j < 0 || j > 15 || !((j % 4 > 0 && j - 1 == blank) ||
				(j % 4 < 3 && j + 1 == blank) ||
				(j > 3 && j - 4 == blank) ||
				(j < 12 && j + 4 == blank)));

			game_board[j]->locate(ppuzzle, 213 + (blank % 4) * 213, 480 - (blank / 4) * 120);
			game_board[blank]->locate(ppuzzle, 213 + (j % 4) * 213, 480 - (j / 4) * 120);

			auto object = game_board[j];
			game_board[j] = game_board[blank];
			game_board[blank] = object;
			blank = j;

			count++;

			if (count < 100) {
				t->set(0.1f);
				t->start();
			}
			return true;
			});

		return true;
		});

	startGame(menu);

	return 0;

}