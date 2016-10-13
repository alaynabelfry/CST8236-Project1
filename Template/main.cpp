#include <SFML/Graphics.hpp>
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

	sf::Texture towerTexture;
	towerTexture.loadFromFile("res/towertexture.png");

	sf::Texture vaneTexture;
	vaneTexture.loadFromFile("res/vanetexture.png");

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
	Windmill* windmills[3];

	for (int i = 0; i < 3; i++) {
		windmills[i] = new Windmill(&towerShape, &vaneShape);
		windmills[i]->SetPosition(window.getSize().x*0.25f*(i + 1), window.getSize().y*0.5f);
	}

	sf::Event evt;
	sf::Clock appTimer;

	float angleAmount = 90.0f;

	sf::Vector2<int>prevMove(0, 0);
	int keySelection = 1;

	sf::Vector2f rotationOrigins[3];
	rotationOrigins[0] = sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	rotationOrigins[1] = windmills[0]->getPosition();
	rotationOrigins[2] = windmills[1]->getPosition();

	while (window.isOpen()) {
		float deltaTime = appTimer.restart().asSeconds();

		while (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
			}
			if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.code == sf::Keyboard::Num1) {
					keySelection = 0;
				}
				else if (evt.key.code == sf::Keyboard::Num2) {
					keySelection = 1;
				}
				else if (evt.key.code == sf::Keyboard::Num3) {
					keySelection = 2;
				}
				else if (evt.key.code == sf::Keyboard::Num4 || evt.key.code == sf::Keyboard::A) {
					keySelection = 3;
					//reset positions
					for (int i = 0; i < 3; i++) {
						windmills[i]->SetPosition(window.getSize().x*0.25f*(i + 1), window.getSize().y*0.5f);
					}
				}
			}

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
					float deltaY = (float)evt.mouseMove.y - prevMove.y;
					if (keySelection != 3) {
						if (keySelection == 1)
							rotationOrigins[1] = windmills[0]->getPosition();
						else if (keySelection == 1)
							rotationOrigins[2] = windmills[1]->getPosition();
						windmills[keySelection]->RotateAround(deltaY, window.getSize().y, rotationOrigins[keySelection]);
					}
					else {
						for (int i = 0; i < 3; i++) {
							if (i == 1){
								rotationOrigins[1] = windmills[0]->getPosition();
								printf("windmill2 rotates around %f,%f\n", windmills[0]->getPosition().x, windmills[0]->getPosition().y);
							}
							else if (i == 2){
								rotationOrigins[2] = windmills[1]->getPosition();
								printf("windmill3 rotates around %f,%f\n", windmills[1]->getPosition().x, windmills[0]->getPosition().y);
							}
							windmills[i]->RotateAround(deltaY, window.getSize().y, rotationOrigins[i]);
						}
					}
					prevMove.y = evt.mouseMove.y;
				}
			}
		}

		for (int i = 0; i < 3; i++) {
			windmills[i]->RotateVanes(deltaTime*angleAmount);
		}
		// do stuff.
		window.clear();

		//draw 
		for (int i = 0; i < 3; i++) {
			windmills[i]->Draw(&window);
		}

		window.display();
	}



	return 0;


}