#include "MathsFunctions.h"

float MathsFunctions::normalise(float a, float b, float f)
{
	return (f - a) / (b - a); 
}

glm::vec2 MathsFunctions::getMidPtX(glm::vec2 A, glm::vec2 B, float a, float b)
{
	return glm::vec2(A.x + normalise(a, b, 0) * (B.x - A.x), A.y);
}

glm::vec2 MathsFunctions::getMidPtY(glm::vec2 A, glm::vec2 B, float a, float b)
{
	return glm::vec2(A.x, A.y + normalise(a, b, 0) * (B.y - A.y));
}

float MathsFunctions::getDistSqr(glm::vec2 A, glm::vec2 B)
{
	return (B.x-A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y);
}
