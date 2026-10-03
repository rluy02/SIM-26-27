#pragma once
namespace simulation {
	// Paso fijo de la simulación 60 fps
	constexpr double fixedTimestep = 1.0 / 60.0; // s
	constexpr double worldGravityY = -9.8; // m/s^2
	constexpr double zeroGravity = 0.0; // m/s^2
}