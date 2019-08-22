/* 
 * Simple app to draw colored hollow rectangles with the mouse.
 * Press C to clear the screen
 * Press Q or send the close signal to quit
 */

#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main(void)
{
	// would be cool if this took the user's display into account instead of hardcoding 1080
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Draw Rectangle", sf::Style::Fullscreen);
	window.clear();
	window.display();

	std::vector<sf::VertexArray> rectangles;

	bool isDrawing{false};
	sf::VertexArray current;
	sf::Color currentColor;
	sf::Mouse::Button currentButton;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					{
						window.close();
						break;
					}
				case sf::Event::KeyPressed:
					{
						if (event.key.code == sf::Keyboard::Key::C)
						{
							rectangles.clear();
							window.clear();
							window.display();
						}
						if (event.key.code == sf::Keyboard::Key::Q)
							window.close();
						break;
					}
				case sf::Event::MouseButtonPressed:
					{
						// don't let two buttons draw at once
						if (isDrawing) break;

						// enter drawing state
						isDrawing = true;

						// change color based on mouse button
						if (event.mouseButton.button == sf::Mouse::Button::Left)
							currentColor = sf::Color::Red;
						if (event.mouseButton.button == sf::Mouse::Button::Right)
							currentColor = sf::Color::Green;
						if (event.mouseButton.button == sf::Mouse::Button::Middle)
							currentColor = sf::Color::Green;

						// keep track of mouse button
						currentButton = event.mouseButton.button;

						// initialize vertex array and add the first vertex
						current = sf::VertexArray(sf::LineStrip, 5);
						current[0] = sf::Vertex(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), currentColor);
						break;
					}
				case sf::Event::MouseButtonReleased:
					{
						// don't cancel another button's drawing
						if (event.mouseButton.button != currentButton) break;

						// copy variables for readability
						int x{event.mouseButton.x}, y{event.mouseButton.y};
						sf::Vector2f origin = current[0].position;

						// create remaining vertices
						current[1] = sf::Vertex(sf::Vector2f(x, origin.y), currentColor);
						current[2] = sf::Vertex(sf::Vector2f(x, y), currentColor);
						current[3] = sf::Vertex(sf::Vector2f(origin.x, y), currentColor);
						current[4] = sf::Vertex(sf::Vector2f(origin.x, origin.y), currentColor);

						// add current rectangle to our set of rectangles
						rectangles.push_back(current);

						// redraw
						window.clear();
						for (auto &r : rectangles)
							window.draw(r);
						window.display();

						// exit drawing state
						isDrawing = false;
						break;
					}
				case sf::Event::MouseMoved:
					{
						if (!isDrawing) break;
						// copy variables for readability
						int x{event.mouseMove.x}, y{event.mouseMove.y};

						// create preview vertex array
						sf::VertexArray preview(sf::LineStrip, 5);
						preview[0] = current[0];

						sf::Vector2f origin = current[0].position;
						
						preview[1] = sf::Vertex(sf::Vector2f(x, origin.y), currentColor);
						preview[2] = sf::Vertex(sf::Vector2f(x, y), currentColor);
						preview[3] = sf::Vertex(sf::Vector2f(origin.x, y), currentColor);
						preview[4] = sf::Vertex(sf::Vector2f(origin.x, origin.y), currentColor);

						// redraw
						window.clear();
						for (auto &r : rectangles)
							window.draw(r);
						window.draw(preview);
						window.display();
						break;
					}
			}
		}
	}
}
