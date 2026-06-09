#include <SFML/Graphics.hpp> // graphics library being used, requires libsfml-dev package to compile
#include <complex>
#include <vector>
#include <thread>

const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_ITERATIONS = 1000;

double minReal = -2.5;
double maxReal = 1.0;
double minImag = -1.25;
double maxImag = 1.25;

// maps a given pixel coordinate onto the complex plane
std::complex<double> pixelToComplex(int x, int y)
{
    double re = minReal + (x * (maxReal - minReal) / WIDTH);
    double im = minImag + (y * (maxImag - minImag) / HEIGHT);

    return std::complex<double> (re, im);
}

// computes mandelbrot escape velocity for a given point on the complex plane
int computeMandelbrot(std::complex<double> c, int exp = 2)
{
    std::complex<double> z {};

    for (int i = 0; i < MAX_ITERATIONS; ++i)
    {
        z = pow(z, exp) + c;

        if (z.real() * z.real() * z.imag() * z.imag() > 4)
            return i;
    }

    return MAX_ITERATIONS;
}

// renders a single horizontal band of pixels (for multi-threading)
void renderBand(sf::Image& image, int startY, int endY)
{
    for (int y = startY; y < endY; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            std::complex<double> c = pixelToComplex(x, y);
            int iter = computeMandelbrot(c);

            if (iter == MAX_ITERATIONS)
            {
                image.setPixel(x, y, sf::Color::Black);
            }
            else
            {
                sf::Uint8 r = static_cast<sf::Uint8>((iter * 5) % 256);
                sf::Uint8 g = static_cast<sf::Uint8>((iter * 9) % 256);
                sf::Uint8 b = static_cast<sf::Uint8>((iter * 14) % 256);

                image.setPixel(x, y, sf::Color(r, g, b));
            }
        }
    }
}

// manages threads to render the entire image
void renderFractal(sf::Image& image)
{
    unsigned int threadsCount = std::thread::hardware_concurrency();
    if (threadsCount == 0) threadsCount = 4; // if the system is unable to provide the number of available cpus, assign 4

    std::vector<std::thread> threads;
    int rowsPerThread = HEIGHT / threadsCount;

    for (unsigned int i = 0; i < threadsCount; ++i)
    {
        int startY = i * rowsPerThread;
        int endY = (i == threadsCount - 1) ? HEIGHT : startY + rowsPerThread;

        threads.push_back(std::thread(renderBand, std::ref(image), startY, endY));
    }

    for (auto &th : threads)
    {
        th.join();
    }
}

/*
void zoom(int x, int y, int zoomFactor)
{
    std::complex<double> mousePos;
}
*/

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set");

    sf::Image image;
    image.create(WIDTH, HEIGHT, sf::Color::Black);

    sf::Texture texture;
    sf::Sprite sprite;

    renderFractal(image);
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    while (window.isOpen())
    {
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}