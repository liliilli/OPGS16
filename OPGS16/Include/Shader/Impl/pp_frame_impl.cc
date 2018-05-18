#include "pp_frame_impl.h"

namespace shading {

constexpr std::array<unsigned, 6> quad_indices = { 0, 1, 2, 2, 3, 0 };
constexpr std::array<float, 32> quad_info = {
	// Vertex       //Normal        // TexCoord
	1.f, 1.f, 0.f,  0.f, 0.f, 1.f,  1.f, 1.f,
	1.f,-1.f, 0.f,  0.f, 0.f, 1.f,  1.f, 0.f,
	-1.f,-1.f, 0.f,  0.f, 0.f, 1.f,  0.f, 0.f,
	-1.f, 1.f, 0.f,  0.f, 0.f, 1.f,  0.f, 1.f
};



}