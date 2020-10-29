#include <SFML/Graphics.hpp>

int main()
{
	// Create a video mode object
	sf::VideoMode vm(1920, 1080);
	// Create and open a window for the game
	sf::RenderWindow window(vm, "Timber", sf::Style::Fullscreen);

	// Create a texture to hold a graphic on the GPU's memory
	sf::Texture backgroundTexture;
	// Load the graphic into the texture
	backgroundTexture.loadFromFile("graphics/background.png");

	// Create a sprite
	sf::Sprite backgroundSprite;
	// Attach the texture to the sprite
	backgroundSprite.setTexture(backgroundTexture);

	// Set the background to cover the screen
	backgroundSprite.setPosition(0, 0);

	while (window.isOpen())
	{
		// handle the players input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}
		// update the scene

		// clear everything from the last frame
		window.clear();
		
		// Draw the game scene
		window.draw(backgroundSprite);

		// Show everything we just drew
		window.display();
	}

	return 0;
}