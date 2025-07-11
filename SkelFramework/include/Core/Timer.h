#pragma once


namespace skel
{
	struct Timer
	{
		Timer() { reset(); }
		float elapsed() const
		{
			std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - start);
			return static_cast<float>(time_span.count());
		}
		void reset() { start = std::chrono::high_resolution_clock::now(); }
		std::chrono::high_resolution_clock::time_point start;
	};
}
