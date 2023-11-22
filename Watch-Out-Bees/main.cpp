#include "all.h"

int main(void) {

	//�̹���
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
	sb.rumble.loadFromFile("./resources/sound/rumble.flac");// �� ���� �� ȿ����
	sb.item.loadFromFile("./resources/sound/item.flac");// item���� �� ȿ����
	// TODO : ����������� �� ��� Ʋ��
	sb.gameover.loadFromFile("./resources/sound/game_over.wav");// ������ ����Ǿ��� ���� ȿ����

	srand(time(0));//���� �Լ� ���

	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "Watch-Out-Bees");
	window.setFramerateLimit(60);//1�ʿ� 60�� �����ش�. �÷��̾ ���� ���� �ʵ��� �ϱ�

	long start_time = clock();// ���� ���� �ð�
	long spent_time;// ���� ���� �ð�
	long fired_time = 0;// �ֱ� �ĸ�ä�� ���� �ð�
	int is_gameover = 0;

	// BGM
	Sound background_sound;
	background_sound.setBuffer(sb.BGM);
	background_sound.setLoop(1);// BGM ���� �ݺ�
	background_sound.play();

	// text ��Ʈ
	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\Candara.ttf");//C����̺꿡 �ִ� ��Ʈ ��������

	Text text;
	char info[40];
	text.setFont(font);//��Ʈ ����
	text.setCharacterSize(24);//��Ʈ ũ��
	text.setFillColor(Color::Black);
	text.setPosition(0, 0);//�ؽ�Ʈ ��ġ

	//���
	Sprite bg_sprite;
	bg_sprite.setTexture(t.background);
	bg_sprite.setPosition(0, 0);

	//gameover
	Gameover* gameover = new Gameover();
	Sound gameover_sound;
	gameover->sprite.setTexture(t.gameover);
	gameover->sprite.setPosition(0, 0);//game over �׸� ��� ��Ÿ����
	gameover_sound.setBuffer(sb.gameover);

	// �÷��̾�
	Player* player = new Player(7, 10, 0, 5);// �ӵ�, �ִ�ӵ�, �ʱ�����, ����
	player->set_x(player->sprite.getPosition().x);// x��ǥ
	player->set_y(player->sprite.getPosition().y);// y��ǥ
	player->sprite.setTexture(&t.player);// �����͸� �Ѱ��ֱ� ������ �ּҰ� �Ѱ��ֱ�
	player->sprite.setSize(Vector2f(100, 100));//�÷��̾� ������
	player->sprite.setPosition(100, 100);//�÷��̾� ���� ��ġ

	// �Ѿ�
	int bullet_speed = 20;// �Ѿ� �ӵ�
	int bullet_idx = 0;// �߻�� ������ �ε��� ������ų ��
	int weapon_delay = 500;	// �Ѿ��� delay�� ����� �Ӽ��̹Ƿ� struct�� ���� ����. 0.5�ʸ��� ����
	int bullet_delay_max = 100;// �Ѿ��� delay �ִ밪
	Sound bullet_sound;
	bullet_sound.setBuffer(sb.rumble);

	Weapon* weapon[WEAPON_NUM];
	for (int i = 0; i < WEAPON_NUM; i++)
	{
		weapon[i] = new Weapon(0);// �Ѿ� �߻� ����
		weapon[i]->sprite.setTexture(&t.weapon);// �����͸� �Ѱ��ֱ� ������ �ּҰ� �Ѱ��ֱ�
		weapon[i]->sprite.setSize(Vector2f(40, 40));// �Ѿ� ũ��
		weapon[i]->sprite.setPosition(player->get_x() + 110, player->get_y() + 20);// �Ѿ� �ʱ� ��ġ
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
		bee[i] = new Bee(-(rand() % 5 + 1), 1);// �������� �־����� ���� �ӵ�, ���� ����
		bee[i]->sprite.setTexture(&t.bee);// �����͸� �Ѱ��ֱ� ������ �ּҰ� �Ѱ��ֱ�
		bee[i]->sprite.setSize(Vector2f(70, 70));// �� ũ��
		bee[i]->sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);// ���� ȭ���� 90%���� ������ ����
	}

	// item�� ���� Ư��
	Item* item[ITEM_NUM];
	Sound item_sound;
	item_sound.setBuffer(sb.item);
	item[0] = new Item(25000, 0, 0, HEAL);// 25�ʸ��� �̼� ������ ����, �������� ����, ��Ÿ�� �ð�, ���� ����
	item[0]->sprite.setTexture(&t.item_heal);// �̼� �̹��� �ּ� ����
	item[0]->sprite.setSize(Vector2f(50, 50));
	item[1] = new Item(20000, 0, 0, DELAY);// 25�ʸ��� �̼� ������ ����, �������� ����, ��Ÿ�� �ð�, ���� ����
	item[1]->sprite.setTexture(&t.item_delay);// ���� �̹��� �ּ� ����
	item[1]->sprite.setSize(Vector2f(50, 50));

	int go = 0;// �� �ǹ� ���� ����. 0�̸� ���� 0�ʰ��̸� �������� ����
	//���� ��Ű�� �����? -> ���� �ݺ�
	while (window.isOpen()) //������â�� �����ִ� ���� ��� �ݺ�
	{
		spent_time = clock() - start_time;// �ð��� ������ ���� ����

		//�Ѿ��� �÷��̾� ����ٴ� �� �ֵ��� 
		player->set_x(player->sprite.getPosition().x);	//�÷��̾� x��ǥ
		player->set_y(player->sprite.getPosition().y);	//�÷��̾� y��ǥ

		Event event;//�̺�Ʈ ����
		while (window.pollEvent(event)) //�̺�Ʈ�� �߻�. �̺�Ʈ�� �߻��ؾ� event �ʱ�ȭ�� ��
		{
			switch (event.type)
			{
				//����(x)��ư�� ������ Event::Closed(0) 
			case Event::Closed://������
				window.close();//������â�� ����
				break;
			}
		}


		/* game ���� update */
		if (player->get_life() <= 0)
		{
			is_gameover = 1;// 1 == true
		}

		/* player update */
		//����Ű
		if (player->get_life() > 0)
		{
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				player->sprite.move(-1 * player->get_speed(), 0);//���� �̵�
			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				player->sprite.move(0, -1 * player->get_speed());//���� �̵�
			}
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				player->sprite.move(0, player->get_speed());//�Ʒ��� �̵�
			}
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				player->sprite.move(player->get_speed(), 0);//������ �̵�
			}
		}

		//player �̵� ���� ����
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
		// �Ѿ� �߻�
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			// delay�� Ŭ �� �۵� ( ���� �ð��� �߻� �ð��� 0.5�ʸ�ŭ�� ���̰� ���� �Ѿ� �߻�)
			if (spent_time - fired_time > weapon_delay)
			{
				//�Ѿ��� �߻� �Ǿ����� �ʴٸ� �Ѿ� �߻�
				if ( !is_gameover && !weapon[bullet_idx]->get_is_fired() && player->get_life() > 0)
				{
					weapon[bullet_idx]->sprite.setPosition(player->sprite.getPosition().x + 110, player->sprite.getPosition().y + 20);// �Ѿ� �ʱ� ��ġ (�ӽ� �׽�Ʈ)
					weapon[bullet_idx]->set_is_fired(1);
					bullet_idx++;// ���� �Ѿ� �߻� �����ϵ���
					bullet_idx = bullet_idx % WEAPON_NUM;// 50�� ��� idx�� 0���� �ٲ�
					bullet_sound.play();// �Ѿ� �߻� �Ҹ�
					fired_time = spent_time;// �Ѿ� ���� (���� �� �ڿ� ���� �� ������ ������ ������ ���Ͻ��� ��)
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

		//enemy���� �浹
		//intersects : �÷��̾�� �� ���̿��� �������� �ִ°�
		for (int i = 0; i < BEE_NUM; i++)
		{
			// 10�� ���� enemy�� ��
			if (spent_time % (1000 * bee_respawn_time) < 1000 / 60 + 1) // 1�ʵ��� 60�������� �ݺ��Ǳ� ������
			{
				// ������ �������϶��� ���� ����
				if (!is_gameover)
				{
					bee[i]->sprite.setSize(Vector2f(50, 50));
					bee[i]->sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 505);// 90%���� ������ ����
					bee[i]->set_life(1);
					// 10�ʸ��� enemy �ӵ� +1
					bee[i]->set_speed(-(rand() % 2 + 1 + (spent_time / 1000 / bee_respawn_time)));
				}
			}

			if (bee[i]->get_life() > 0)
			{
				// �ĸ�ä�� ���� ����� ��
				if (is_collide(weapon[i]->sprite, bee[i]->sprite))
				{
					bee[i]->set_life(bee[i]->get_life() - 1);
					player->set_score(player->get_score() + bee_score);

					if (bee[i]->get_life() == 0)
					{
						bee_hit_sound.play();
					}
				}
				// ���� ����� ��
				else if (is_collide(player->sprite, bee[i]->sprite)) {
					bee[i]->set_life(bee[i]->get_life()-1);
					player->set_life(player->get_life() - 1);
				}

				// enemy�� x��ǥ�� 0�� �Ǹ�
				//else if (enemy[i].sprite.getPosition().x < 0)
				//{
				//	player.life -= 1;// player ��� 1 ����
				//	enemy[i].life = 0;// �� ���ֱ�
				//}

				// �Ѿ˰� enemy�� �浹
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
			if (!item[i]->get_is_presented() && player->get_life() > 0)// �������� ��Ÿ���� �ʾҰ� �÷��̾��� ü���� �ִٸ�
			{
				if (spent_time - item[i]->get_presented_time() > item[i]->get_delay())// ������ delay�ʺ��� �� ���� �ð��� ������
				{
					item[i]->sprite.setPosition(rand() % (W_WIDTH) * 0.8, rand() % W_HEIGHT * 0.8);
					item[i]->set_is_presented(1);// �������� �� (true)
				}
			}
			if (item[i]->get_is_presented())
			{
				// ������ ȹ�� �� ȿ���� ��� �������
				if (is_collide(player->sprite, item[i]->sprite))
				{
					switch (item[i]->get_type())
					{
					case HEAL: // �߰� ����
						player->set_life(player->get_life()+1);// player ��� 1 ����
						break;
					case DELAY: // ���� ������
						weapon_delay -= 100;// �Ѿ� ������ ���̱�
						if (weapon_delay < bullet_delay_max)
						{
							weapon_delay = bullet_delay_max;
						}
						break;
					}
					item_sound.play();
					// ������� �ڵ�
					item[i]->set_is_presented(0);
					item[i]->set_presented_time(spent_time);// �������� �Դ� ���� �ٽ� ��Ÿ�� �ð� ����
				}
			}
		}

		// ���� �ð��� ������ ����
		sprintf(info, "life: %d \n score: %d \n time: %d\n", player->get_life(), player->get_score(), spent_time / 1000);
		text.setString(info);


		window.clear();

		window.draw(bg_sprite);

		for (int i = 0; i < BEE_NUM; i++)
		{
			if (bee[i]->get_life() > 0)  window.draw(bee[i]->sprite);//�� �����ֱ�
		}

		// ������ �׷��ֱ�
		for (int i = 0; i < ITEM_NUM; i++)
		{
			if (item[i]->get_is_presented())
				window.draw(item[i]->sprite);
		}
		window.draw(player->sprite);//�÷��̾� �����ֱ�(�׷��ֱ�)
		window.draw(text);

		// �߻�� �Ѿ˸� �׸���
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