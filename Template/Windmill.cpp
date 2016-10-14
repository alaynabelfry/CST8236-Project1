#include "Windmill.h"



Windmill::Windmill(const sf::RectangleShape* towerToCopy, const sf::RectangleShape* vaneToCopy) {
	towerShape = copy(towerToCopy);
	tower.m_CurrentShape = &towerShape;
	vaneShapes[0] = copy(vaneToCopy);
	vaneShapes[1] = copy(vaneToCopy);
	vaneShapes[2] = copy(vaneToCopy);
	vaneShapes[3] = copy(vaneToCopy);
	for (int i = 0; i < 4; i++) {
		vaneShapes[i].setRotation(90.0f*i);
		vaneShapes[i].setPosition(0, (-towerShape.getLocalBounds().height / 4.0f));
		vanes[i]= new TransformNode(&vaneShapes[i]);
		tower.AddChild(vanes[i]);
	}
}

void Windmill::Draw(sf::RenderTarget * target)
{
	tower.Draw(target);
}

void Windmill::SetRotatePoint(sf::Vector2f origin)
{
	float toRotate = (origin.x / origin.y) * 2 * PI;
	float a = towerShape.getPosition().x - origin.x;
	float b = towerShape.getPosition().y - origin.y;
	distance = sqrtf(a*a + b*b);
	lastangle = toRotate;
}

Windmill::~Windmill()
{
	for (int i = 0; i < 4; i++) {
		delete vanes[i];
	}
}

sf::RectangleShape Windmill::copy(const sf::RectangleShape * shapeToCopy)
{
	sf::RectangleShape newShape(shapeToCopy->getSize());
	newShape.setOrigin(shapeToCopy->getOrigin());
	newShape.setScale(shapeToCopy->getScale());
	newShape.setTexture(shapeToCopy->getTexture());
	return newShape;
}


void Windmill::SetPosition(float x, float y)
{
	towerShape.setPosition(x, y);
}

sf::Vector2f Windmill::getPosition()
{
	return towerShape.getPosition();
}

void Windmill::RotateSelf(float deltaX, float screenX)
{

	towerShape.rotate(360.0f*(deltaX / screenX));
}


void Windmill::RotateAround(float deltaY, float screenY, sf::Vector2f origin)
{
	//convert to radians for trig
	float toRotate = (deltaY / screenY) * 2 * PI;

	lastangle += toRotate;
	
	if(lastangle>=2*PI){
		lastangle -= 2*PI;
	}
	else if (lastangle <= -2 * PI) {
		lastangle += 2 * PI;
	}
	float newX = cosf(lastangle) * (distance) + origin.x;
	float newY = sinf(lastangle) * (distance) + origin.y;
	towerShape.setPosition(newX, newY);
}

void Windmill::RotateVanes(float angle)
{
	for (int i = 0; i < 4; i++) {
		vaneShapes[i].rotate(angle);
	}
}


