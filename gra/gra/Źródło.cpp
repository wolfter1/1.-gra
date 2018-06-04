#include <iostream>
#include <vector>
#include <SFML\Graphics.hpp>
#include <time.h>
#include <cmath>
#include <string>

#define M_PI       3.14159265358979323846

int main()
{
	sf::VideoMode DesktopMode = sf::VideoMode::getDesktopMode();
	//sf::RenderWindow window(DesktopMode,"gra", sf::Style::Titlebar | sf::Style::Close);
	sf::RenderWindow window(DesktopMode, "gra", sf::Style::Fullscreen);

restart:
	//stale i zmienne
	const float r = 30.0f;
	float kat = 0.0f;
	bool n = 0;
	sf::Clock animation, lifeTime;
	sf::Vector2f predkosc = sf::Vector2f(0.0f, 0.0f);
	bool pause = true;
	srand(time(NULL));
	float lifebarWidth = 1000.0f;
	float lifebarHeight = 50.0f;
	int iloscPunktow = -1;

	//napisy
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
		return EXIT_FAILURE;
	sf::Text text("PRESS S TO START", font, 50);
	text.setPosition(sf::Vector2f(DesktopMode.width / 3 + 80, 160));

	sf::Text hpText("100% hp", font, 30);
	hpText.setPosition(sf::Vector2f(50, 10));

	sf::Text punkty("ilosc punktow: 0", font, 50);
	punkty.setPosition(sf::Vector2f(DesktopMode.width * 0.8, 30));

	//textury i sprity
	sf::Texture texture[2];
	if (!texture[0].loadFromFile("character1.png"))
		return EXIT_FAILURE;
	if (!texture[1].loadFromFile("character2.png"))
		return EXIT_FAILURE;
	sf::Vector2f sizeCharacter = static_cast<sf::Vector2f > (texture[0].getSize());
	sf::Sprite character(texture[0]);
	character.setOrigin(sf::Vector2f(sizeCharacter.x / 2, sizeCharacter.y / 2));
	character.setPosition(960.0f, 540.0f);


	//punkty
	sf::CircleShape point(r);
	point.setOrigin(r, r);
	point.setFillColor(sf::Color(230, 200, 60));
	point.setPosition(960.0f, 540.0f);

	//tabela
	sf::RectangleShape table;
	table.setSize(sf::Vector2f(DesktopMode.width, 125));
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
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S && pause == true)
			{
				pause = false;
			}

		}
		
		if (!pause)
		{
			//koordynaty myszki i obiektu sprite oraz odleglosc miedzy nimi i kat
			sf::Vector2f coordsMouse = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
			sf::Vector2f coordsCharacter = character.getPosition();
			sf::Vector2f distanceVector = sf::Vector2f(abs(coordsMouse.x - coordsCharacter.x), abs(coordsMouse.y - coordsCharacter.y));
			float distance = sqrt(pow(distanceVector.x, 2.0f) + pow(distanceVector.y, 2.0f));
			kat = (180.0f / M_PI) * atan(distanceVector.y / distanceVector.x);

			//predkosc obiektu

			//podazania obiektu za myszka
			if (coordsMouse.x >= coordsCharacter.x)
			{
				if (coordsMouse.y <= coordsCharacter.y)
					kat = 360 - kat;
				else
				{
				}
				character.setScale(1.0f, 1.0f);
			}
			else
			{
				if (coordsMouse.y <= coordsCharacter.y)
					kat = 180 + kat;
				else
					kat = 180 - kat;
				character.setScale(1.0f, -1.0f);
			}
			float v = distance / 500;
			character.setRotation(kat);
			predkosc.x = v * cos(kat);
			predkosc.y = v * sin(kat);
			character.move(predkosc);

			if (coordsCharacter.y < 200.0f)
			{
				character.move(0.0f, 200.0f - coordsCharacter.y);
			}


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
				point.setPosition(rand() % (DesktopMode.width - 201) + 100, rand() % (DesktopMode.height - 276) + 200);
				lifeTime.restart();
				iloscPunktow++;

				std::string sIloscPunktow = std::to_string(iloscPunktow);
				punkty.setString("ilosc punktow: " + sIloscPunktow);

			}

			//spadek zycia
			float lifebarRealTime = lifebarWidth - 1000 * lifeTime.getElapsedTime().asMilliseconds() / lifebarWidth;
			lifebar.setSize(sf::Vector2f(lifebarRealTime, lifebarHeight));
			if (lifebarRealTime <= 0)
				goto restart;
			int hp = round(100 * lifebarRealTime / lifebarWidth);
			
			std::string sHp = std::to_string(hp);
			std::cout << sHp << std::endl;
			hpText.setString(sHp + "% hp");
		}

		window.clear(sf::Color(10,10,10));
		window.draw(point);
		window.draw(character);
		window.draw(table);
		window.draw(lifebar2);
		window.draw(lifebar);
		window.draw(hpText);
		window.draw(punkty);
		if (pause)
			window.draw(text);

		window.display();
	}
	return 0;
}