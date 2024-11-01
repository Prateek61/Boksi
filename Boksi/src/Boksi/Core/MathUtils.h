#pragma once

#include <glm/glm.hpp>

namespace Boksi::Maths
{
	glm::vec2 OctahedralEncode(glm::vec3 vec);
	glm::vec3 OctahedralDecode(glm::vec2 vec);
}