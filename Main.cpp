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

	// Tree sprite
	sf::Texture treeTexture;
	treeTexture.loadFromFile("graphics/tree.png");
	sf::Sprite treeSprite;
	treeSprite.setTexture(treeTexture);
	treeSprite.setPosition(810, 0);

	// Bee sprite
	sf::Texture beeTexture;
	beeTexture.loadFromFile("graphics/bee.png");
	sf::Sprite beeSprite;
	beeSprite.setTexture(beeTexture);

	// it the bee currently moving?
	bool beeActive{ false };
	// speed of the bee
	float beeSpeed{ 0.0f };

	// Cloud Sprites
	sf::Texture cloudTexture;
	cloudTexture.loadFromFile("graphics/cloud.png");
	sf::Sprite cloudSprite1;
	sf::Sprite cloudSprite2;
	sf::Sprite cloudSprite3;
	cloudSprite1.setTexture(cloudTexture);
	cloudSprite2.setTexture(cloudTexture);
	cloudSprite3.setTexture(cloudTexture);

	// Position the clouds on the left of the screen at different heights
	cloudSprite1.setPosition(0, 0);
	cloudSprite2.setPosition(0, 250);
	cloudSprite3.setPosition(0, 500);

	// Are the clouds currently on the screen?
	bool cloud1Active{ false };
	bool cloud2Active{ false };
	bool cloud3Active{ false };

	// Cloud speed
	float cloud1Speed{ 0.0f };
	float cloud2Speed{ 0.0f };
	float cloud3Speed{ 0.0f };

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
		
		// Draw the clouds
		window.draw(cloudSprite1);
		window.draw(cloudSprite2);
		window.draw(cloudSprite3);

		// Draw the tree
		window.draw(treeSprite);

		// Draw the bee
		window.draw(beeSprite);

		// Show everything we just drew
		window.display();
	}

	return 0;
}