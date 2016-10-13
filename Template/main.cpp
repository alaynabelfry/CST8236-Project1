#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "TransformNode.h"
#include "Windmill.h"


sf::RectangleShape copyRect(const sf::RectangleShape* shapeToCopy) {
	sf::RectangleShape newShape(shapeToCopy->getSize());
	newShape.setOrigin(shapeToCopy->getOrigin());
	newShape.setScale(shapeToCopy->getScale());
	newShape.setTexture(shapeToCopy->getTexture());
	return newShape;
}

int main(int argc, char *argv)
{

	sf::Texture cloudTexture;
	cloudTexture.loadFromFile("res/cloud.png");
	cloudTexture.setRepeated(true);
	sf::Sprite tilingBgSprite;
	tilingBgSprite.setTexture(cloudTexture);


	sf::Texture towerTexture;
	towerTexture.loadFromFile("res/towertexture.png");

	sf::Texture vaneTexture;
	vaneTexture.loadFromFile("res/vanetexture.png");

	sf::SoundBuffer buffer;
	buffer.loadFromFile("res/chickadee.ogg");
	sf::Sound sound(buffer);

	sf::RectangleShape towerShape(sf::Vector2<float>(towerTexture.getSize()));
	towerShape.setTexture(&towerTexture);

	sf::FloatRect towerBounds = towerShape.getLocalBounds();

	towerShape.setOrigin(towerBounds.left + towerBounds.width / 2.0f, towerBounds.top + towerBounds.height / 2.0f);
	towerShape.setScale(0.25f, 0.25f);

	sf::RectangleShape vaneShape(sf::Vector2<float>(vaneTexture.getSize()));
	vaneShape.setTexture(&vaneTexture);
	sf::FloatRect vaneBounds = vaneShape.getLocalBounds();
	//long side is x, short is y
	vaneShape.setOrigin(0, vaneBounds.height / 2.0f);
	vaneShape.setScale(0.5f, 0.5f);

	sf::RenderWindow window(sf::VideoMode(800, 800), "Template", sf::Style::Titlebar | sf::Style::Close);

	tilingBgSprite.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));
	sf::IntRect bgRect = tilingBgSprite.getTextureRect();
	float secondsTimer = 0.0f;
	float bgSpeed = 150.0f;

	Windmill* windmills[3];

	for (int i = 0; i < 3; i++) {
		windmills[i] = new Windmill(&towerShape, &vaneShape);
		windmills[i]->SetPosition(window.getSize().x*0.25f + window.getSize().x*0.2f*(i + 1), window.getSize().y*0.5f);
	}

	sf::Vector2f *rotatePoints[3];
	rotatePoints[0] = new sf::Vector2f(windmills[2]->getPosition());
	rotatePoints[1] = new sf::Vector2f(windmills[0]->getPosition());
	rotatePoints[2] = new sf::Vector2f(windmills[1]->getPosition());

	for (int i = 0; i < 3; i++) {
		windmills[i]->SetRotatePoint(*rotatePoints[i]);
	}


	sf::Event evt;
	sf::Clock appTimer;

	float angleAmount = 90.0f;

	sf::Vector2<int>prevMove(0, 0);
	int keySelection = 1;


	sf::Vector2f rotateorigin = sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	bool soundPlaying = false;

	while (window.isOpen()) {
		float deltaTime = appTimer.restart().asSeconds();

		while (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
			}
			if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.code == sf::Keyboard::Num4 || evt.key.code == sf::Keyboard::A) {
					keySelection = 3;
					//reset positions
					for (int i = 0; i < 3; i++) {
						windmills[i]->SetPosition(window.getSize().x*0.25f + window.getSize().x*0.2f*(i + 1), window.getSize().y*0.5f);
					}
					/*windmills[0]->SetRotatePoint(rotateorigin);
					windmills[1]->SetRotatePoint(windmills[0]->getPosition());
					windmills[2]->SetRotatePoint(windmills[1]->getPosition());*/
				}
				else {
					if (evt.key.code == sf::Keyboard::Num1) {
						keySelection = 0;
					}
					else if (evt.key.code == sf::Keyboard::Num2) {
						keySelection = 1;
					}
					else if (evt.key.code == sf::Keyboard::Num3) {
						keySelection = 2;
					}
					windmills[keySelection]->SetRotatePoint(*rotatePoints[keySelection]);
				}



			}

			//windmill rotations
			if (evt.type == sf::Event::MouseMoved) {
				if (evt.mouseMove.x != prevMove.x) {
					float deltaX = (float)evt.mouseMove.x - prevMove.x;
					if (keySelection != 3) {
						windmills[keySelection]->RotateSelf(deltaX, window.getSize().x);
					}
					else {
						for (int i = 0; i < 3; i++)
							windmills[i]->RotateSelf(deltaX, window.getSize().x);
					}
					prevMove.x = evt.mouseMove.x;
				}

				if (evt.mouseMove.y != prevMove.y) {
					sf::Vector2f origin;
					float deltaY = (float)evt.mouseMove.y - prevMove.y;
					if (keySelection != 3) {
						windmills[keySelection]->RotateAround(deltaY, window.getSize().y, *rotatePoints[keySelection]);
					}
					else {
						for (int i = 0; i < 3; i++) {
							if (i == 1) {
								origin = windmills[0]->getPosition();
							}
							else if (i == 2) {
								origin = windmills[1]->getPosition();
							}
							else
								origin = rotateorigin;
							windmills[i]->RotateAround(deltaY, window.getSize().y, origin);
						}
					}
					prevMove.y = evt.mouseMove.y;
				}
			}
		}

		for (int i = 0; i < 3; i++) {
			windmills[i]->RotateVanes(deltaTime*angleAmount);
		}

		if (!soundPlaying) {
			sound.play();
			soundPlaying = true;
		}

		secondsTimer += deltaTime;

		sf::IntRect deltaRect = bgRect;
		deltaRect.left = bgRect.left + (int)(bgSpeed * secondsTimer);
		tilingBgSprite.setTextureRect(deltaRect);

		if (secondsTimer > 1.0f)
		{
			secondsTimer -= 1.0f;
			bgRect.left += (int)bgSpeed;

			if (bgRect.left >= (float)cloudTexture.getSize().x) {
				bgRect.left -= cloudTexture.getSize().x;
			}
		}

		window.clear();

		//draw 
		window.draw(tilingBgSprite);
		for (int i = 0; i < 3; i++) {
			windmills[i]->Draw(&window);
		}

		window.display();
	}



	return 0;


}