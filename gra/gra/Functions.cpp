#include "include.h"

void follow(sf::Sprite &character, sf::Vector2f coordsMouse, sf::Vector2f coordsCharacter)
{
	//koordynaty myszki i obiektu sprite oraz odleglosc miedzy nimi i kat
	sf::Vector2f distanceVector = sf::Vector2f(abs(coordsMouse.x - coordsCharacter.x), abs(coordsMouse.y - coordsCharacter.y));
	float distance = sqrt(pow(distanceVector.x, 2.0f) + pow(distanceVector.y, 2.0f));
	float kat = (180.0f / M_PI) * std::atan2f(distanceVector.y, distanceVector.x);

	//podazania obiektu za myszka
	if (coordsMouse.x >= coordsCharacter.x)
	{
		if (coordsMouse.y <= coordsCharacter.y)
			kat = 360 - kat;

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
	//float v = 1;
	character.setRotation(kat);
	sf::Vector2f predkosc;
	predkosc.x = v * cos(kat);
	predkosc.y = v * sin(kat);

	if (coordsCharacter.y < 200.0f)
		character.move(0.0f, 201.0f - coordsCharacter.y);
	else
		character.move(predkosc);
}

void follow_v2(sf::Sprite &character, sf::Vector2f coordsMouse, sf::Vector2f coordsCharacter)
{
	character.setPosition(coordsMouse);
}


