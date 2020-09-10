#include <bangtal.h>
using namespace bangtal;


int main() {
	//1. 장면을 생성한다.
	ScenePtr scene1 = Scene::create("룸1", "image/배경-1.png");
	ScenePtr scene2 = Scene::create("룸2", "image/배경-2.png");
	ScenePtr on_desk = Scene::create("책상위", "image/책상위.png");
	ScenePtr memo = Scene::create("메모", "image/memo.jpg");
	ScenePtr game_over = Scene::create("게임종료", "image/gameover.png");
	ScenePtr paper = Scene::create("쓰레기에 적힌 암호", "image/paper.png");
	ScenePtr code = Scene::create("암호", "image/code.png");
	ScenePtr in_mirror = Scene::create("거울", "image/justmirror.png");
	

	auto open1 = false;
	auto door1 = Object::create("image/닫힌문.png", scene1, 900, 170);

	door1->setOnKeypadCallback([&](ObjectPtr objct)->bool {

		showMessage("문이 열렸다. room2로 이동할 수 있다.");
		open1 = true;
		door1->setImage("image/열린문.png");

		return true;

	});


	door1->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {

		if (open1 == true) {//문이 열린 상태에서 클릭할 경우
			scene2->enter();
		}
		else {				//문이 닫힌 상태에서 클릭할 경우
			showMessage("문이 닫혀있다.\n 암호가 필요하다!");
			showKeypad("BAEGOPA", door1);

		}
		return true;

	});

	auto door3 = Object::create("image/닫힌문3.png", scene1, 300, 170);
	door3->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {

		showMessage("굳게 잠겨있습니다.");

		return true;

	});


	auto door2 = Object::create("image/열린문2.png", scene2, 300, 170);
	door2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		scene1->enter();
		return true;

	});


	auto hind_door = Object::create("image/숨김문.png", scene2, 870, 280, true);
	hind_door->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		endGame();
		return true;
	});

	/*
	책상 위 상황
	1. 책상 클릭 -> 책상위로 이동
	2. 책상 위에 곰인형 클릭 -> 타이머 시작
	3. 리턴 이미지 클릭 -> scene1로 장면 이동
	4. 
	*/
	auto desk = Object::create("image/desk.png", scene1, 530, 150);
	auto doll = Object::create("image/곰인형.png", on_desk, 200, 300);
	auto come_back = Object::create("image/되돌아가기.png", on_desk, 600, 30);
	auto stop_button = Object::create("image/버튼.png", scene2, 1000, 145,false);
	

	desk->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		on_desk->enter();

		return true;
	});

	come_back->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		
		scene1->enter();
		return true;
	});

	auto timer = Timer::create(30.f);
	auto endbutton = Object::create("image/end.png", game_over, 600, 300);
	

	doll->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		
		showMessage("곰인형에 부착되어 있던 센서가 감지되어 타이머가 시작되었다.\n 30초안에 탈출해야한다!");
		stop_button->show();
		showTimer(timer);
		timer->start();

//타이머가 완료되었을때 어떻게 처리할 것인지

		timer->setOnTimerCallback([&](TimerPtr)->bool {			
			showMessage("탈출실패!");
			game_over->enter();
			return true;
		});

		return true;
	});

	/*
	scene1에 거울
	1. 종이를 들고 거울을 클릭하면 종이에 적힌 글자를 좌우반전해서 보여준다.
	2. 좌우반전한 문자로 입력하면 흰색문 열림.
	3. 종이 안들고 거울 클릭하면 아무것도 안보임.

	*/
	auto mirror = Object::create("image/mirror.png", scene1, 120, 100);
	auto trash = Object::create("image/trashpaper.png", scene1, 500, 130);
	trash->setScale(0.1f);
	auto come_back2 = Object::create("image/되돌아가기.png", paper, 600, 30);
	auto come_back3 = Object::create("image/되돌아가기.png", code, 600, 30);


	trash->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		trash->pick();
		paper->enter();
		showMessage("쓰레기를 주워보니 암호가 적혀있다. \n 무슨 뜻일까?");

		return true;
	});

	mirror->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		
		if (trash->isHanded()) {
			code->enter();
			showMessage("거울에 종이를 비추자 암호가 해독되었다. \n 빨리 암호를 사용해서 문을 열자.");

			
		}
		else {
			showMessage("거울을 이용하려면 어떤 아이템이 있어야 한다!");
		}

		return true;
	});

	come_back2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		scene1->enter();
		return true;
	});

	come_back3->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		scene1->enter();
		return true;
	});

	//키패드
	




	/*
	scene2에 게시판에 숨긴문 힌트
	1. 게시판 누르면 메모장면으로 가기
	2. 화살표 누르면 scene2로 돌아가기
	*/
	auto bulletin_board = Object::create("image/게시판.png", scene2, 700, 400);
	auto arrows = Object::create("image/화살표.png", memo, 600, 100);
	bulletin_board->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		memo->enter();
		return true;
	});
	arrows->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		scene2->enter();
		return true;
	});

	/*
	scene2에 책장 두고 책장을 옆으로 밀면 타이머를 멈추는 버튼 등장
	1.버튼 누르면 타이머 멈춤
	*/
	

	auto bookshelf = Object::create("image/책장.jpg", scene2, 1000, 145);
	bookshelf->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (action == MouseAction::MOUSE_DRAG_LEFT) {
			bookshelf->locate(scene2, 900, 145);
		}
		else if (action == MouseAction::MOUSE_DRAG_RIGHT) {
			bookshelf->locate(scene2, 1100, 145);
		}
		return true;
	});
	
	stop_button->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		showMessage("타이머를 멈추는 버튼을 발견하여 타이머가 멈춥니다.");
		stop_button->pick();
		timer->stop();
		return true;
	});

	endbutton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		endGame();

		return true;
		});


	//2. 게임을 시작한다.
	startGame(scene1);

	return 0;
}