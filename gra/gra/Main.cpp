#include "include.h"
#include "Timer.h"

void follow(sf::Sprite &character, sf::Vector2f coordsMouse, sf::Vector2f coordsCharacter);
void follow_v2(sf::Sprite &, sf::Vector2f coordsMouse, sf::Vector2f coordsCharacter);

int main()
{
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	//sf::RenderWindow window(DesktopMode,"gra", sf::Style::Titlebar | sf::Style::Close);
	sf::RenderWindow window(desktopMode, "gra", sf::Style::Fullscreen);

restart:
	//stale i zmienne
	const float r = 30.0f;
	float kat = 0.0f;
	bool n = 0;
	sf::Clock animation, countdown;
	Timer czas;
	sf::Vector2f predkosc = sf::Vector2f(0.0f, 0.0f);
	bool pause = false;
	srand(time(NULL));
	float lifebarWidth = 1000.0f;
	float lifebarHeight = 50.0f;
	int iloscPunktow = -1;
	bool naZycia = 1;
	sf::Vector2f coordsMouse = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)), coordsCharacter, mousePause;
	//muzyka
	sf::Music pointSound;
	if (!pointSound.openFromFile("pointSound.ogg"))
		return EXIT_FAILURE;
	pointSound.setVolume(5.0f);

	//napisy
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
		return EXIT_FAILURE;
	sf::Text text("", font, 300);
	text.setPosition(sf::Vector2f(desktopMode.width / 2 - 85, 130));

	sf::Text hpText("hp", font, 40);
	hpText.setPosition(sf::Vector2f(50, 0));

	sf::Text punkty("ilosc punktow: 0", font, 50);
	punkty.setPosition(sf::Vector2f(desktopMode.width * 0.7, 30));

	//textury i sprity
	sf::Texture texture[2];
	if (!texture[0].loadFromFile("character1.png"))
		return EXIT_FAILURE;
	if (!texture[1].loadFromFile("character2.png"))
		return EXIT_FAILURE;
	sf::Vector2f sizeCharacter = static_cast<sf::Vector2f > (texture[0].getSize());
	sf::Sprite character(texture[0]);
	character.setOrigin(sf::Vector2f(sizeCharacter.x / 2, sizeCharacter.y / 2));
	character.setPosition(desktopMode.width / 2, desktopMode.height / 2);

	sf::CircleShape mouseCircle(50);
	mouseCircle.setOrigin(50, 50);
	mouseCircle.setFillColor(sf::Color(200, 50, 100, 100));
	mouseCircle.setOutlineColor(sf::Color(250, 250, 250, 200));
	mouseCircle.setOutlineThickness(30);
	mouseCircle.setPosition(desktopMode.width / 2, desktopMode.height / 2);

	//punkty
	sf::CircleShape point(r);
	point.setOrigin(r, r);
	point.setFillColor(sf::Color(230, 200, 60));
	point.setPosition(desktopMode.width / 2, desktopMode.height / 2);

	//tabela
	sf::RectangleShape table;
	table.setSize(sf::Vector2f(desktopMode.width, 125));
	table.setFillColor(sf::Color(100, 100, 100));

	//zycie
	sf::RectangleShape lifebar;
	lifebar.setSize(sf::Vector2f(lifebarWidth, lifebarHeight));
	lifebar.setPosition(50, 50);
	lifebar.setFillColor(sf::Color::Red);

	sf::RectangleShape lifebar2;
	lifebar2.setSize(sf::Vector2f(lifebarWidth, lifebarHeight));
	lifebar2.setPosition(50, 50);
	lifebar2.setOutlineThickness(5);
	lifebar2.setOutlineColor(sf::Color(100, 150, 200, 200));
	lifebar2.setFillColor(sf::Color::Black);


	while (window.isOpen())
	{
		//eventy
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
				goto restart;
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
			{
				if (pause == true && mouseCircle.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition())))
				{
					pause = false;
					czas.start();
				}
				else if (pause == false)
				{
					pause = true;
					czas.pause();
					mouseCircle.setPosition(coordsMouse);
				}
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1)
			{
				if (naZycia)
				{
					naZycia = 0;
					lifebar.setSize(sf::Vector2f(lifebarWidth, lifebarHeight));
				}
					
				else
				{
					naZycia = 1;
					czas.restart();
				}
					
			}
		}
		
		if (countdown.getElapsedTime().asSeconds() > 3 && pause == false)
		{
			
			//koordynaty myszki i obiektu sprite oraz odleglosc miedzy nimi i kat
			coordsMouse = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
			coordsCharacter = character.getPosition();
			follow(character, coordsMouse, coordsCharacter);
			//follow_v2(character, coordsMouse, coordsCharacter);

			//animacja
			if (animation.getElapsedTime().asSeconds() >= 0.5)
			{
				if (!n)
				{
					character.setTexture(texture[1]);
					n = 1;
				}
				else
				{
					character.setTexture(texture[0]);
					n = 0;
				}

				animation.restart();
			}
			sf::FloatRect characterBounds = character.getGlobalBounds();
			sf::FloatRect pointBounds = point.getGlobalBounds();
			if (characterBounds.intersects(pointBounds))
				//punkty pojawiajace sie
			{
				point.setPosition(rand() % (desktopMode.width - 201) + 100, rand() % (desktopMode.height - 276) + 200);
				czas.restart();
				iloscPunktow++;
				std::string sIloscPunktow = std::to_string(iloscPunktow);
				punkty.setString("ilosc punktow: " + sIloscPunktow);
				if (iloscPunktow != 0)
				{
					pointSound.stop();
					pointSound.play();
				}
					
				
			}

			if (naZycia)
			{
				//spadek zycia
				float lifebarRealTime = lifebarWidth - 1000 * czas.getTime() * 1000 / lifebarWidth - 5 * iloscPunktow * czas.getTime();
				lifebar.setSize(sf::Vector2f(lifebarRealTime, lifebarHeight));
				if (lifebarRealTime <= 0)
					goto restart;
			}
		}

		window.clear(sf::Color(10,10,10));
		window.draw(point);
		window.draw(character);
		window.draw(table);
		window.draw(lifebar2);
		window.draw(lifebar);
		window.draw(hpText);
		window.draw(punkty);
		if (countdown.getElapsedTime().asSeconds() <= 3)
		{
			int iText = ceil(3 - countdown.getElapsedTime().asSeconds());
			text.setString(std::to_string(iText));
			window.draw(text);
		}
		if (pause)
			window.draw(mouseCircle);
	
		window.display();
	}
	return 0;
}
