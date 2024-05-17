#include "main.hpp"
#include "Mandelbrot_set.hpp"

//#define WIDTH 1600
//#define HEIGHT 1600
//#define WIDTH 1920
//#define HEIGHT 1080


int main()
{

	initWindow();



	return 0;
}



void initWindow()
{

	//std::vector<float> dataPoints = { 10.f, 20.f, 30.f, 40.f, 50.f }; // Veri noktalarý
	float minValue = 0.f;
	float maxValue = 100.f;

	bool isHUDPressed = false;
	std::vector<std::pair<int, int>> predefinedSizes = {
	{800, 600},
	{1024, 768},
	{1280, 720},
	{1600, 900},
	{1920, 1080}
	};

	int choice;
	std::cout << "Programi Nasil Baslatmak Istediginizi Seciniz:" << std::endl;
	std::cout << "1. Custom Size Degerleri ile Baslatmak icin 1'e Basiniz" << std::endl;
	std::cout << "2. Onceden Belirlenmis Size Boyutlarý ile Baslatmak icin 2'ye Basiniz" << std::endl;
	std::cin >> choice;

	int width, height;

	if (choice == 1) {
		std::cout << "Pencere Genisligini Giriniz: ";
		std::cin >> width;
		std::cout << "Pencere Yuksekligini Giriniz: ";
		std::cin >> height;
	}
	else if (choice == 2) {
		std::cout << "Asagidaki Size Degerlerinden Birini Seciniz:" << std::endl;
		for (size_t i = 0; i < predefinedSizes.size(); ++i) {
			std::cout << i + 1 << ". " << predefinedSizes[i].first << "x" << predefinedSizes[i].second << std::endl;
		}
		int chosenIndex;
		std::cin >> chosenIndex;
		if (chosenIndex >= 1 && chosenIndex <= predefinedSizes.size()) {
			width = predefinedSizes[chosenIndex - 1].first;
			height = predefinedSizes[chosenIndex - 1].second;
		}
		else {
			std::cerr << "Invalid choice. Using default size." << std::endl;
			width = 800;
			height = 600; // Varsayýlan boyutu belirtin
		}
	}
	else {
		std::cerr << "Invalid choice. Using default size." << std::endl;
		width = 800;
		height = 600; // Varsayýlan boyutu belirtin
	}

	sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Set");

	sf::Vertex* vertices = initVertices(width, height);

	ms::Mandelbrot_set mandelbrot(&window, width, height, vertices);


	window.clear();
	
	mandelbrot.calculate_speedup_efficiency();
	mandelbrot.show();
	//mandelbrot.show_seq();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				mandelbrot.setPosition(sf::Mouse::getPosition(window).x - (int)(window.getSize().x >> 1), sf::Mouse::getPosition(window).y - (int)(window.getSize().y >> 1));
			}
			if (event.type == sf::Event::MouseWheelMoved)
			{
				if (event.mouseWheel.delta > 0)
					mandelbrot.setZoom(0.9);
				else
					mandelbrot.setZoom(1.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				mandelbrot.setStartY(-100);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				mandelbrot.setStartY(100);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				mandelbrot.setStartX(-100);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				mandelbrot.setStartX(100);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
				mandelbrot.setZoom(0.9);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
				mandelbrot.setZoom(1.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				mandelbrot.reset();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
				mandelbrot.setIteration(1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
				mandelbrot.setIteration(-1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
				if (!isHUDPressed) {
					isHUDPressed = true;
					mandelbrot.invertHUD();
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
				
			}
			else
				isHUDPressed = false;
		}
		


	}
}

sf::Vertex* initVertices(int width, int height)
{
	sf::Vertex* vertices = new sf::Vertex[width * height];

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			vertices[x + y * width].position = sf::Vector2f(x, y);
			vertices[x + y * width].color = sf::Color::Black; // Initial color
		}
	}

	return vertices;
}

#if __DEBUG
void fractal(sf::RenderWindow& window, sf::Vertex* vertices, int startX, int startY, double zoom) // 15 * 10^5 micro sec
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	sf::Color color;
	double divX = window.getSize().x / 3.5;
	double divY = window.getSize().y / 2.0;
	for (int x = 0; x < window.getSize().x; ++x)
	{
		for (int y = 0; y < window.getSize().y; ++y)
		{
			double internalX = (x * zoom + startX) / divX - 2.5;
			double internalY = (y * zoom + startY) / divY - 1.0;
			double re = internalX;
			double im = internalY;
			int iteration = 1;
			while (re * re + im * im <= 4 && iteration < MAX_ITER)
			{
				double reTemp = re * re - im * im + internalX;
				im = 2 * re * im + internalY;
				re = reTemp;
				iteration++;
			}
			double iterationNorm = iteration / (double)MAX_ITER;
			color = hsv2rgb_s1(iterationNorm * 360.0, iteration < MAX_ITER ? 1.0 : 0.0);
			vertices[x + y * WIDTH].color = color;
		}
	}
	window.draw(vertices, WIDTH * HEIGHT, sf::Points);
	window.display();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time enlapsed CPU: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[microseconds]" << std::endl;
}
#endif