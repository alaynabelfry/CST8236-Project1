#ifndef  _WINDMILL_H
#define _WINDMILL_H
#include <SFML/Graphics.hpp>
#include "TransformNode.h"
class Windmill {
public:

	Windmill(const sf::RectangleShape* towerToCopy, const const sf::RectangleShape* vaneToCopy);
	
	void SetPosition(float x, float y);
	sf::Vector2f getPosition();
	void RotateSelf(float deltaX, float screenX);
	void RotateAround(float deltaY, float screenY, sf::Vector2f origin);
	void RotateVanes(float angle);
	void Draw(sf::RenderTarget *target);

protected:
	sf::RectangleShape copy(const sf::RectangleShape* shapeToCopy);

	const float PI = 3.1415927;
	sf::RectangleShape towerShape;
	sf::RectangleShape vaneShapes[4];
	TransformNode tower;
	TransformNode* vanes[4];
	float lastangle = 0;
	bool hasDistance = false;
	float distance;

};

#endif // ! _WINDMILL_H


