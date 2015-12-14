//**********************************************************************************************************************
#pragma once

//**********************************************************************************************************************
/*
       NW
   W   |   N
     \ | /
SW ----*---- NE    x
     / | \
   S   |   E
       SE

       y

*/

enum ObjectOrientation
{
		NorthEast = 0,
		East,
		SouthEast,
		South,
		SouthWest,
		West,
		NorthWest,
		North,

		count,

};

//**********************************************************************************************************************
static const char* strObjectOrientation[ObjectOrientation::count] = { "ne", "e", "se", "s", "sw", "w", "nw", "n" };

//**********************************************************************************************************************
static ObjectOrientation GetObjectOrientation(sf::Vector2f const& orientation)
{
	static const float anglesOrientation[ObjectOrientation::count] = { 0, 30, 90, 150, 180, 210, 270, 330 };

	ObjectOrientation result = NorthEast;

	float cos2 = uu::maths::Vector2Dot(orientation, sf::Vector2f(1,0));
	float sin2 = uu::maths::Vector2Dot(orientation, sf::Vector2f(0,1));

	float angle;
	if (sin2 > 0)
	{
		angle = acos(cos2);
	}
	else
	{
		angle = uu::maths::TwoPi - acos(cos2);
	}
	angle = Radians2Degrees(angle);

	for (int i=ObjectOrientation::count-1; i >= 0 ; i--)
	{
		result = (ObjectOrientation)i;
		if (angle >= anglesOrientation[i])
			break;
	}

	//printf("ObjectOrientation  x=%f   y=%f  angle=%f deg -> %s\n", cos2, sin2, angle, strObjectOrientation[result]);

	return result;
}
