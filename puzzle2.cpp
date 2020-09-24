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


	//1. ����ȭ�� : ���Ӽ���
	auto menu = Scene::create("���Ӽ���", "images/menu.png");

	//1. ����ȭ�� : ���� �����, ���� ����
	auto totoro = Object::create("images/totoro.png", menu, 80, 150);
	auto ponyo = Object::create("images/ponyo.png", menu, 700, 150);

	//2. ��������, ����ΰ��� ��� ����
	auto tpuzzle = Scene::create("���������", "images/playbackground2.png");
	auto ppuzzle = Scene::create("��������", "images/playbackground.png");

	ObjectPtr game_board[16];
	ObjectPtr init_board[16];
	int blank = 15;

	srand((unsigned int)time(NULL));

	//2. ����� Ŭ���ϸ� ����� ���� ���� 
	totoro->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		tpuzzle->enter();
		start_time = clock();
		timer->start();

		//i�� index
		for (int i = 0; i < 16; i++) {
			string filename = "images/totoro_" + to_string(i + 1) + ".png";
			printf("%d", select_game);
			game_board[i] = Object::create(filename, tpuzzle, 213 + (i % 4) * 213, 480 - (i / 4) * 120);
			init_board[i] = game_board[i];
			game_board[i]->setOnMouseCallback([&](ObjectPtr object, int, int, MouseAction)->bool {

				//1. j�� Ŭ���� ����� index
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
					//3. �׸��� �ٲ�
					game_board[j]->locate(tpuzzle, 213 + (blank % 4) * 213, 480 - (blank / 4) * 120);
					game_board[blank]->locate(tpuzzle, 213 + (j % 4) * 213, 480 - (j / 4) * 120);
					//2. blank�� j�� �ٲ�
					game_board[j] = game_board[blank];
					game_board[blank] = object;
					blank = j;

					int k;
					for (k = 0; k < 16; k++) {
						if (game_board[k] != init_board[k]) break;
					}
					if (k == 16) {
						//4.�ϼ��ϸ� �ð����
						end_time = clock();

						int do_time = (end_time - start_time) / CLOCKS_PER_SEC;
						char s1[100], s2[50];

						sprintf_s(s1, "%d", do_time / 60);	//������ ���ڿ��� �ٲٱ�
						strcat_s(s1, "��");
						sprintf_s(s2, "%d", do_time % 60);
						strcat_s(s2, "��");
						strcat_s(s1, s2);
						showMessage(s1);
					}
				}
				return true;
				});
		}

		game_board[blank]->hide();

		//��������
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
				//�ȿ����̴� ����
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

	//2. ���� Ŭ���ϸ� ���� ���� ���� 
	ponyo->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		ppuzzle->enter();
		start_time = clock();
		timer->start();
		//i�� index
		for (int i = 0; i < 16; i++) {
			string filename = "images/" + to_string(i + 1) + ".png";
			printf("%d", select_game);
			game_board[i] = Object::create(filename, ppuzzle, 213 + (i % 4) * 213, 480 - (i / 4) * 120);
			init_board[i] = game_board[i];
			game_board[i]->setOnMouseCallback([&](ObjectPtr object, int, int, MouseAction)->bool {

				//1. j�� Ŭ���� ����� index
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
					//3. �׸��� �ٲ�
					game_board[j]->locate(ppuzzle, 213 + (blank % 4) * 213, 480 - (blank / 4) * 120);
					game_board[blank]->locate(ppuzzle, 213 + (j % 4) * 213, 480 - (j / 4) * 120);
					//2. blank�� j�� �ٲ�
					game_board[j] = game_board[blank];
					game_board[blank] = object;
					blank = j;

					int k;
					for (k = 0; k < 16; k++) {
						if (game_board[k] != init_board[k]) break;
					}
					if (k == 16) {
						//4.�ϼ��ϸ� �ð����
						end_time = clock();

						int do_time = (end_time - start_time) / CLOCKS_PER_SEC;
						char s1[100], s2[50];

						sprintf_s(s1, "%d", do_time / 60);	//������ ���ڿ��� �ٲٱ�
						strcat_s(s1, "��");
						sprintf_s(s2, "%d", do_time % 60);
						strcat_s(s2, "��");
						strcat_s(s1, s2);
						showMessage(s1);
					}
				}
				return true;
				});
		}

		game_board[blank]->hide();

		//��������
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
				//�ȿ����̴� ����
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