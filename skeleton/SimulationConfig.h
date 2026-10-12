#pragma once
namespace simulation {
	// Paso fijo de la simulación 60 fps
	constexpr double fixedTimestep = 1.0 / 60.0; // s
	constexpr float  fixedTimestepF = static_cast<float>(fixedTimestep);
	constexpr float worldGravityY = -9.8f; // m/s^2
	constexpr float zeroGravity = 0.0f; // m/s^2
	constexpr float systemWorldGravityY = -10.f; // m/s^2
}