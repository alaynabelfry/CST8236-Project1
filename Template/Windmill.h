#ifndef  _WINDMILL_H
#define _WINDMILL_H
#include <SFML/Graphics.hpp>
#include "TransformNode.h"
class Windmill {
public:

	Windmill(const sf::RectangleShape* towerToCopy, const sf::RectangleShape* vaneToCopy);
	
	void SetPosition(float x, float y);
	sf::Vector2f getPosition();
	void RotateSelf(float deltaX, float screenX);
	void RotateAround(float deltaY, float screenY, sf::Vector2f origin);
	void RotateVanes(float angle);
	void Draw(sf::RenderTarget *target);
	void SetRotatePoint(sf::Vector2f origin);

protected:
	sf::RectangleShape copy(const sf::RectangleShape* shapeToCopy);

	const float PI = 3.1415927f;
	sf::RectangleShape towerShape;
	sf::RectangleShape vaneShapes[4];
	TransformNode tower;
	TransformNode* vanes[4];
	float lastangle = 0;
	float distance=-1;

};

#endif // ! _WINDMILL_H


