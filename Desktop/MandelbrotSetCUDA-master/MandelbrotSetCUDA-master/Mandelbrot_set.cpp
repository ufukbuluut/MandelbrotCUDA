#include "Mandelbrot_set.hpp"
#include "mandelbrot_sequential.h"
#include <thread>
#include <cuda.h>
#include <cuda_runtime.h>

#include <iostream>
#include <fstream>
#include <chrono>


using namespace ms;

Mandelbrot_set::Mandelbrot_set(sf::RenderWindow* window, int width, int height, sf::Vertex* vertices) {
	this->width = width;
	this->height = height;
	this->window = window;
	this->vertices = vertices;
}

int ms::Mandelbrot_set::calculate_paralell_CUDA()
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	int* result = mendelbrot_kernel(height, width, maxIter, startX, startY, zoom,32,32);
	for (int x = 0; x < (*window).getSize().x; ++x)
	{
		for (int y = 0; y < (*window).getSize().y; ++y)
		{
			int i = x + y * width;

#ifdef __STATIC_MAX_ITER
			vertices[i].color.r = palette[result[i]].r;
			vertices[i].color.g = palette[result[i]].g;
			vertices[i].color.b = palette[result[i]].b;
#else
			double iterationNorm = 360.0 * result[i] / maxIter;
			if (result[i])
				vertices[i].color = hsv2rgb_fast(iterationNorm);
			else
				vertices[i].color = sf::Color::Black;
#endif
		}
	}
	cudaFreeHost(result);

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	int paralell = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	return paralell;
}

int ms::Mandelbrot_set::calculate_sequential_CUDA()
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	int* result = mendelbrot_sequential(height, width, maxIter, startX, startY, zoom);

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	for (int x = 0; x < (*window).getSize().x; ++x) {
		for (int y = 0; y < (*window).getSize().y; ++y) {
			int i = x + y * width;

#ifdef __STATIC_MAX_ITER
			vertices[i].color.r = palette[result[i]].r;
			vertices[i].color.g = palette[result[i]].g;
			vertices[i].color.b = palette[result[i]].b;
#else
			double iterationNorm = 360.0 * result[i] / maxIter;
			if (result[i])
				vertices[i].color = hsv2rgb_fast(iterationNorm);
			else
				vertices[i].color = sf::Color::Black;
#endif
		}
	}

	delete[] result; // Free memory allocated for result

	
	int sequential = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	return sequential;
}

void Mandelbrot_set::show()
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	int* result = mendelbrot_kernel(height, width, maxIter, startX, startY, zoom,32,32);
	for (int x = 0; x < (*window).getSize().x; ++x)
	{
		for (int y = 0; y < (*window).getSize().y; ++y)
		{
			int i = x + y * width;

#ifdef __STATIC_MAX_ITER
			vertices[i].color.r = palette[result[i]].r;
			vertices[i].color.g = palette[result[i]].g;
			vertices[i].color.b = palette[result[i]].b;
#else
			double iterationNorm = 360.0 * result[i] / maxIter;
			if (result[i])
				vertices[i].color = hsv2rgb_fast(iterationNorm);
			else
				vertices[i].color = sf::Color::Black;
#endif
		}
	}
	cudaFreeHost(result);
	(*window).draw(vertices, width * height, sf::Points);
	if (isHUDshown) showHUD();

	(*window).display();

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Paralell Time enlapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[microseconds]" << std::endl;

}

void Mandelbrot_set::show_seq() {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	int* result = mendelbrot_sequential(height, width, maxIter, startX, startY, zoom);

	for (int x = 0; x < (*window).getSize().x; ++x) {
		for (int y = 0; y < (*window).getSize().y; ++y) {
			int i = x + y * width;

#ifdef __STATIC_MAX_ITER
			vertices[i].color.r = palette[result[i]].r;
			vertices[i].color.g = palette[result[i]].g;
			vertices[i].color.b = palette[result[i]].b;
#else
			double iterationNorm = 360.0 * result[i] / maxIter;
			if (result[i])
				vertices[i].color = hsv2rgb_fast(iterationNorm);
			else
				vertices[i].color = sf::Color::Black;
#endif
		}
	}

	delete[] result; // Free memory allocated for result

	(*window).draw(vertices, width * height, sf::Points);
	if (isHUDshown) showHUD();

	(*window).display();

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Sequential Time elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[microseconds]" << std::endl;

}




void Mandelbrot_set::calculate_speedup_efficiency()
{




	std::ofstream outputFile("speedup_efficiency_results.txt"); // Open a file for writing results
	if (!outputFile.is_open())
	{
		std::cerr << "Error opening output file!" << std::endl;
		return;
	}

	for (int i = 16; i <= 1024; i *= 2)
	{
			std::chrono::steady_clock::time_point begin_seq = std::chrono::steady_clock::now();

			int* result_sequential = mendelbrot_sequential(height, width, maxIter, startX, startY, zoom);

			std::chrono::steady_clock::time_point end_seq = std::chrono::steady_clock::now();

			// Time for sequential execution
			double sequentialTime = std::chrono::duration_cast<std::chrono::microseconds>(end_seq - begin_seq).count();
			std::cout << "Sequential Time: " << sequentialTime << " microseconds" << std::endl;

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		int* result = mendelbrot_kernel(height, width, maxIter, startX, startY, zoom, i, i);
		double grid_size = (((width + i - 1) / i) * ((height + i - 1) / i));
		double block_size = i * i;
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		// Calculate parallel execution time
		double parallelTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
		
		// Compute speedup
		double speedup = (sequentialTime) / parallelTime;

		double numThreads = (width / grid_size) * (height / grid_size);
		// Compute efficiency
		double efficiency = speedup / numThreads;

		// Write results to console
		std::cout << "Speedup (" << i << "x" << i << "): " << speedup << std::endl;
		std::cout << "Efficiency (" << i << "x" << i << "): " << efficiency << std::endl;
		std::cout << "Paralell Time: " << parallelTime << " microseconds" << std::endl;
		// Write results to file
		outputFile << i <<" " << speedup <<" "<< efficiency << std::endl;
		//outputFile << "Efficiency (" << i << "x" << i << "): " << efficiency << std::endl;

		// Free memory allocated by CUDA
		cudaFreeHost(result);
}

	outputFile.close(); // Close the output file
	std::cout << "Results written to speedup_efficiency_results.txt" << std::endl;
}



void Mandelbrot_set::showHUD() {
	sf::Font font;
	font.loadFromFile("graphics/font/Roboto-Regular.ttf");

	char str[128];
	snprintf(str, 127, "OffsetX: %.2f   OffsetY: %.2f\nZoom: %.4f\nMax iteration: %d", startX, startY, zoom, maxIter);

	sf::RectangleShape rect(sf::Vector2f(450, 100));

	sf::Text text;
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Black);
	text.setStyle(sf::Text::Bold);
	(*window).draw(rect);
	(*window).draw(text);
}

void Mandelbrot_set::setZoom(double modifier) {
	zoom *= modifier;
	show();
	//show_seq();


}

void Mandelbrot_set::setStartX(double modifier) {
	setPosition(modifier, 0);
}

void Mandelbrot_set::setStartY(double modifier) {
	setPosition(0, modifier);
}

void Mandelbrot_set::setPosition(double modifierX, double modifierY) {
	startX += modifierX * zoom;
	startY += modifierY * zoom;
	show();
	//show_seq();


}

void Mandelbrot_set::setIteration(int modifier) {
	if (modifier < 0 && maxIter > 1)
		maxIter += modifier;
	else if (modifier > 0 && maxIter < 199)
		maxIter += modifier;
	show();
	//show_seq();


}

void Mandelbrot_set::invertHUD() {
	isHUDshown = !isHUDshown;
	show();
	//show_seq();


}

void Mandelbrot_set::reset() {
	startX = startY = 0.0;
	zoom = 1.0;
	maxIter = 100;
	show();
	//show_seq();


}

sf::Color Mandelbrot_set::hsv2rgb(double h, double s, double v) {
	double C, X, Y, Z, H;
	short i;
	sf::Color out;

	h /= 60.0;
	i = (short)(h);
	H = h - i;
	C = v * 255;
	X = C * (1.0 - s);
	Y = C * (1.0 - s * H);
	Z = C * (1.0 - s * (1 - H));

	switch (i)
	{
	case 0:
		out.r = C;
		out.g = Z;
		out.b = X;
		break;
	case 1:
		out.r = Y;
		out.g = C;
		out.b = Z;
		break;
	case 2:
		out.r = X;
		out.g = C;
		out.b = Z;
		break;
	case 3:
		out.r = X;
		out.g = Y;
		out.b = C;
		break;
	case 4:
		out.r = Z;
		out.g = X;
		out.b = C;
		break;
	case 5:
	default:
		out.r = C;
		out.g = X;
		out.b = Y;
		break;
	}
	return out;
}

sf::Color Mandelbrot_set::hsv2rgb_fast(double h) {
	const double C = 255.0;
	double X, Y, Z, H;
	short i;
	sf::Color out;

	h /= 60.0;
	i = (short)(h);
	H = h - i;
	X = 0.0;
	Y = C * (1.0 - H);
	Z = C * H;

	switch (i)
	{
	case 0:
		out.r = C;
		out.g = Z;
		out.b = X;
		break;
	case 1:
		out.r = Y;
		out.g = C;
		out.b = Z;
		break;
	case 2:
		out.r = X;
		out.g = C;
		out.b = Z;
		break;
	case 3:
		out.r = X;
		out.g = Y;
		out.b = C;
		break;
	case 4:
		out.r = Z;
		out.g = X;
		out.b = C;
		break;
	case 5:
	default:
		out.r = C;
		out.g = X;
		out.b = Y;
		break;
	}
	return out;
}

