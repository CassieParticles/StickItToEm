#include <glm.hpp>

class MathsFunctions
{
public:
	static float normalise(float a, float b, float f);

	static glm::vec2 getMidPtX(glm::vec2 A, glm::vec2 B, float a, float b);
	static glm::vec2 getMidPtY(glm::vec2 A, glm::vec2 B, float a, float b);
};