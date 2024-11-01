#include "bkpch.h"

#include "MathUtils.h"

namespace Boksi::Maths
{
	// https://jcgt.org/published/0003/02/01/
	inline glm::vec2 SignNotZero(glm::vec2 vec)
	{
		return { glm::sign(vec.x), glm::sign(vec.y) };
	}

	glm::vec2 OctahedralEncode(glm::vec3 vec)
	{
		glm::vec2 enc = glm::vec2(vec.x, vec.y) * (1.0f / (glm::abs(vec.x) + glm::abs(vec.y) + glm::abs(vec.z)));

		// Reflect the folds of the lower hemisphere over the diagonals
		return vec.z < 0.0f ? (1.0f - glm::abs(glm::vec2(enc.y, enc.x))) * SignNotZero(enc) : enc;
	}

	glm::vec3 OctahedralDecode(glm::vec2 vec)
	{
		glm::vec3 dec = glm::vec3(vec.x, vec.y, 1.0f - glm::abs(vec.x) - glm::abs(vec.y));
		if (dec.z < 0.0f)
		{
			dec.x = (1.0f - glm::abs(vec.y)) * glm::sign(vec.x);
			dec.y = (1.0f - glm::abs(vec.x)) * glm::sign(vec.y);
		}

		return glm::normalize(dec);
	}
}