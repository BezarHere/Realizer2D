#include <SFML/Graphics.hpp>

#ifndef R2D_TITLE
// funy
#define R2D_TITLE "Dude, define 'R2D_TITLE' for a window title!"
#endif // !R2D_TITLE

#ifndef R2D_CLEAR_CLR
#define R2D_CLEAR_CLR sf::Color(12U, 12U, 12U, 0U)
#endif // !R2D_CLEAR_CLR

using namespace R2D;

void endApplication()
{

	exit(0);
}


int main(int argc, const char** argv)
{
	MainWindow = new sf::RenderWindow(
		sf::VideoMode(512u, 512u, 8u),
		R2D_TITLE,
		7u,
	sf::ContextSettings(8U, 4U, 0U));

	MyPath = _R2D_ string_t(*argv);


	sf::Event window_event;
	while (MainWindow->isOpen())
	{
		MainWindow->clear(R2D_CLEAR_CLR);
		// check all events
		while (MainWindow->pollEvent(window_event)){

			switch (window_event.type)
			{
				case sf::Event::Closed:
					endApplication();
					break;
				default:
					break;
			}
		}


		MainWindow->display();
	}
	return EXIT_SUCCESS;
}

