#ifndef __VISUAL
#define __VISUAL
#include <SFML/Graphics.hpp>
#include "Mandelbrot_set.hpp"



////////////////////////////////////////////////////////////
///
/// Initializes the Mandelbrot_set object and starts the main loop to handle user's iputs
///
////////////////////////////////////////////////////////////

void initWindow();

////////////////////////////////////////////////////////////
///
/// Initializes an array of size (WIDTH * HEIGHT) containing all the vertices that represent each pixel of the screen
///
/// \return List of initialized vertices
/// 
////////////////////////////////////////////////////////////
sf::Vertex* initVertices(int width, int height);

#endif // __VISUAL
