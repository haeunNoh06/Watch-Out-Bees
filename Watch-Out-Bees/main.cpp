#include "all.h"

int main(void) {

	//이미지
	struct Textures t;
	t.background.loadFromFile("./resources/image/background.png");
	t.bee.loadFromFile("./resources/image/enemy.png");
	t.gameover.loadFromFile("./resources/image/gameover.png");
	t.player.loadFromFile("./resources/image/player.png");
	t.item_delay.loadFromFile("./resources/image/item_damage.png");
	t.item_heal.loadFromFile("./resources/image/item_speed.png");
	t.weapon.loadFromFile("./resources/image/bullet.png");

	// BGM
	struct SBuffers sb;
	sb.BGM.loadFromFile("./resources/sound/bgm.ogg");
	sb.rumble.loadFromFile("./resources/sound/rumble.flac");// 벌 죽을 때 효과음
	sb.item.loadFromFile("./resources/sound/item.flac");// item얻을 때 효과음
	// TODO : 게임종료됐을 때 브금 틀기
	sb.gameover.loadFromFile("./resources/sound/game_over.wav");// 게임이 종료되었을 떄의 효과음

	srand(time(0));//랜덤 함수 사용

	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "Watch-Out-Bees");
	window.setFramerateLimit(60);//1초에 60장 보여준다. 플레이어가 빨리 가지 않도록 하기

	long start_time = clock();// 게임 시작 시간
	long spent_time;// 게임 진행 시간
	long fired_time = 0;// 최근 파리채를 날린 시간
	int is_gameover = 0;

	// BGM
	Sound background_sound;
	background_sound.setBuffer(sb.BGM);
	background_sound.setLoop(1);// BGM 무한 반복
	background_sound.play();

	// text 폰트
	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\Candara.ttf");//C드라이브에 있는 폰트 가져오기

	Text text;
	char info[40];
	text.setFont(font);//폰트 세팅
	text.setCharacterSize(24);//폰트 크기
	text.setFillColor(Color::Black);
	text.setPosition(0, 0);//텍스트 위치

	//배경
	Sprite bg_sprite;
	bg_sprite.setTexture(t.background);
	bg_sprite.setPosition(0, 0);

	//gameover
	Gameover* gameover = new Gameover();
	Sound gameover_sound;
	gameover->sprite.setTexture(t.gameover);
	gameover->sprite.setPosition(0, 0);//game over 그림 가운데 나타내기
	gameover_sound.setBuffer(sb.gameover);

	// 플레이어
	Player* player = new Player(7, 10, 0, 5);// 속도, 최대속도, 초기점수, 생명
	player->set_x(player->sprite.getPosition().x);// x좌표
	player->set_y(player->sprite.getPosition().y);// y좌표
	player->sprite.setTexture(&t.player);// 포인터를 넘겨주기 때문에 주소값 넘겨주기
	player->sprite.setSize(Vector2f(100, 100));//플레이어 사이즈
	player->sprite.setPosition(100, 100);//플레이어 시작 위치

	// 총알
	int bullet_speed = 20;// 총알 속도
	int bullet_idx = 0;// 발사될 때마다 인덱스 증가시킬 것
	int weapon_delay = 500;	// 총알의 delay는 모두의 속성이므로 struct에 넣지 않음. 0.5초마다 나감
	int bullet_delay_max = 100;// 총알의 delay 최대값
	Sound bullet_sound;
	bullet_sound.setBuffer(sb.rumble);

	Weapon* weapon[WEAPON_NUM];
	for (int i = 0; i < WEAPON_NUM; i++)
	{
		weapon[i] = new Weapon(0);// 총알 발사 여부
		weapon[i]->sprite.setTexture(&t.weapon);// 포인터를 넘겨주기 때문에 주소값 넘겨주기
		weapon[i]->sprite.setSize(Vector2f(40, 40));// 총알 크기
		weapon[i]->sprite.setPosition(player->get_x() + 110, player->get_y() + 20);// 총알 초기 위치
	}

	// enemy
	Bee* bee[BEE_NUM];
	Sound bee_hit_sound;
	bee_hit_sound.setBuffer(sb.rumble);
	int bee_score = 100;
	int bee_respawn_time = 8;

	/* enemy update */
	for (int i = 0; i < BEE_NUM; i++)
	{
		bee[i] = new Bee(-(rand() % 5 + 1), 1);// 랜덤으로 주어지는 적의 속도, 적의 생명
		bee[i]->sprite.setTexture(&t.bee);// 포인터를 넘겨주기 때문에 주소값 넘겨주기
		bee[i]->sprite.setSize(Vector2f(70, 70));// 적 크기
		bee[i]->sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);// 가로 화면의 90%부터 적들이 나옴
	}

	// item의 고유 특성
	Item* item[ITEM_NUM];
	Sound item_sound;
	item_sound.setBuffer(sb.item);
	item[0] = new Item(25000, 0, 0, HEAL);// 25초마다 이속 아이템 나옴, 뜨지않은 상태, 나타난 시간, 생명 증가
	item[0]->sprite.setTexture(&t.item_heal);// 이속 이미지 주소 설정
	item[0]->sprite.setSize(Vector2f(50, 50));
	item[1] = new Item(20000, 0, 0, DELAY);// 25초마다 이속 아이템 나옴, 뜨지않은 상태, 나타난 시간, 생명 증가
	item[1]->sprite.setTexture(&t.item_delay);// 공속 이미지 주소 설정
	item[1]->sprite.setSize(Vector2f(50, 50));

	int go = 0;// 별 의미 없는 변수. 0이면 실행 0초과이면 실행하지 않음
	//유지 시키는 방법은? -> 무한 반복
	while (window.isOpen()) //윈도우창이 열려있는 동안 계속 반복
	{
		spent_time = clock() - start_time;// 시간이 지남에 따라 증가

		//총알이 플레이어 따라다닐 수 있도록 
		player->set_x(player->sprite.getPosition().x);	//플레이어 x좌표
		player->set_y(player->sprite.getPosition().y);	//플레이어 y좌표

		Event event;//이벤트 생성
		while (window.pollEvent(event)) //이벤트가 발생. 이벤트가 발생해야 event 초기화가 됨
		{
			switch (event.type)
			{
				//종료(x)버튼을 누르면 Event::Closed(0) 
			case Event::Closed://정수임
				window.close();//윈도우창이 닫힘
				break;
			}
		}


		/* game 상태 update */
		if (player->get_life() <= 0)
		{
			is_gameover = 1;// 1 == true
		}

		/* player update */
		//방향키
		if (player->get_life() > 0)
		{
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				player->sprite.move(-1 * player->get_speed(), 0);//왼쪽 이동
			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				player->sprite.move(0, -1 * player->get_speed());//위쪽 이동
			}
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				player->sprite.move(0, player->get_speed());//아래쪽 이동
			}
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				player->sprite.move(player->get_speed(), 0);//오른쪽 이동
			}
		}

		//player 이동 범위 제한
		if (player->sprite.getPosition().x < 0)
		{
			player->sprite.setPosition(0, player->sprite.getPosition().y);
		}
		else if (player->sprite.getPosition().x > W_WIDTH - 100)
		{
			player->sprite.setPosition(W_WIDTH - 100, player->sprite.getPosition().y);
		}
		if (player->sprite.getPosition().y < 0)
		{
			player->sprite.setPosition(player->sprite.getPosition().x, 0);
		}
		else if (player->sprite.getPosition().y > W_HEIGHT - 100)
		{
			player->sprite.setPosition(player->sprite.getPosition().x, W_HEIGHT - 100);
		}

		/* bullet update */
		// 총알 발사
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			// delay가 클 때 작동 ( 현재 시간과 발사 시간이 0.5초만큼의 차이가 나면 총알 발사)
			if (spent_time - fired_time > weapon_delay)
			{
				//총알이 발사 되어있지 않다면 총알 발사
				if ( !is_gameover && !weapon[bullet_idx]->get_is_fired() && player->get_life() > 0)
				{
					weapon[bullet_idx]->sprite.setPosition(player->sprite.getPosition().x + 110, player->sprite.getPosition().y + 20);// 총알 초기 위치 (임시 테스트)
					weapon[bullet_idx]->set_is_fired(1);
					bullet_idx++;// 다음 총알 발사 가능하도록
					bullet_idx = bullet_idx % WEAPON_NUM;// 50번 대신 idx가 0으로 바뀜
					bullet_sound.play();// 총알 발사 소리
					fired_time = spent_time;// 총알 장전 (총을 쏜 뒤에 총을 쏜 시점과 현재의 시점을 동일시할 것)
				}
			}
		}

		for (int i = 0; i < WEAPON_NUM; i++)
		{
			if (weapon[i]->get_is_fired()) {
				weapon[i]->sprite.move(bullet_speed, 0);
				if (weapon[i]->sprite.getPosition().x > W_WIDTH)
					weapon[i]->set_is_fired(0);
			}
		}

		//enemy와의 충돌
		//intersects : 플레이어와 적 사이에서 교집합이 있는가
		for (int i = 0; i < BEE_NUM; i++)
		{
			// 10초 마다 enemy가 젠
			if (spent_time % (1000 * bee_respawn_time) < 1000 / 60 + 1) // 1초동안 60프레임이 반복되기 때문에
			{
				// 게임이 진행중일때만 적이 나옴
				if (!is_gameover)
				{
					bee[i]->sprite.setSize(Vector2f(50, 50));
					bee[i]->sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 505);// 90%부터 적들이 나옴
					bee[i]->set_life(1);
					// 10초마다 enemy 속도 +1
					bee[i]->set_speed(-(rand() % 2 + 1 + (spent_time / 1000 / bee_respawn_time)));
				}
			}

			if (bee[i]->get_life() > 0)
			{
				// 파리채가 벌과 닿았을 때
				if (is_collide(weapon[i]->sprite, bee[i]->sprite))
				{
					bee[i]->set_life(bee[i]->get_life() - 1);
					player->set_score(player->get_score() + bee_score);

					if (bee[i]->get_life() == 0)
					{
						bee_hit_sound.play();
					}
				}
				// 벌과 닿았을 때
				else if (is_collide(player->sprite, bee[i]->sprite)) {
					bee[i]->set_life(bee[i]->get_life()-1);
					player->set_life(player->get_life() - 1);
				}

				// enemy의 x좌표가 0이 되면
				//else if (enemy[i].sprite.getPosition().x < 0)
				//{
				//	player.life -= 1;// player 목숨 1 차감
				//	enemy[i].life = 0;// 적 없애기
				//}

				// 총알과 enemy의 충돌
				for (int j = 0; j < WEAPON_NUM; j++)
				{
					if (is_collide(weapon[j]->sprite, bee[i]->sprite))
					{
						if (weapon[j]->get_is_fired())
						{
							bee[i]->set_life(0);
							player->set_score(player->get_score() + bee_score);

							bee_hit_sound.play();
							weapon[j]->set_is_fired(0);
						}
					}
				}
				bee[i]->sprite.move(bee[i]->get_speed(), 0);
			}
		}

		/* item update */
		for (int i = 0; i < ITEM_NUM; i++)
		{
			if (!item[i]->get_is_presented() && player->get_life() > 0)// 아이템이 나타나지 않았고 플레이어의 체력이 있다면
			{
				if (spent_time - item[i]->get_presented_time() > item[i]->get_delay())// 각각의 delay초보다 더 많은 시간이 지나면
				{
					item[i]->sprite.setPosition(rand() % (W_WIDTH) * 0.8, rand() % W_HEIGHT * 0.8);
					item[i]->set_is_presented(1);// 아이템이 뜸 (true)
				}
			}
			if (item[i]->get_is_presented())
			{
				// 아이템 획득 시 효과를 얻고 사라진다
				if (is_collide(player->sprite, item[i]->sprite))
				{
					switch (item[i]->get_type())
					{
					case HEAL: // 추가 생명
						player->set_life(player->get_life()+1);// player 목숨 1 증가
						break;
					case DELAY: // 공속 아이템
						weapon_delay -= 100;// 총알 딜레이 줄이기
						if (weapon_delay < bullet_delay_max)
						{
							weapon_delay = bullet_delay_max;
						}
						break;
					}
					item_sound.play();
					// 사라지는 코드
					item[i]->set_is_presented(0);
					item[i]->set_presented_time(spent_time);// 아이템을 먹는 순간 다시 쿨타임 시간 적용
				}
			}
		}

		// 시작 시간은 변하지 않음
		sprintf(info, "life: %d \n score: %d \n time: %d\n", player->get_life(), player->get_score(), spent_time / 1000);
		text.setString(info);


		window.clear();

		window.draw(bg_sprite);

		for (int i = 0; i < BEE_NUM; i++)
		{
			if (bee[i]->get_life() > 0)  window.draw(bee[i]->sprite);//적 보여주기
		}

		// 아이템 그려주기
		for (int i = 0; i < ITEM_NUM; i++)
		{
			if (item[i]->get_is_presented())
				window.draw(item[i]->sprite);
		}
		window.draw(player->sprite);//플레이어 보여주기(그려주기)
		window.draw(text);

		// 발사된 총알만 그리기
		for (int i = 0; i < WEAPON_NUM; i++)
		{
			if (weapon[i]->get_is_fired())
				window.draw(weapon[i]->sprite);
		}
		if (is_gameover) {
			window.draw(gameover->sprite);
			background_sound.stop();
			go++;

			if (go <= 1) {
				gameover_sound.play();
			}
		}

		window.display();
	}

	return 0;
}