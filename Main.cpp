#include <stdlib.h> // srand & rand
#include <sstream>
#include <time.h> // time
#include <SFML/Graphics.hpp>

void updateBranches(int seed);

const int NUM_BRANCHES{ 6 };
sf::Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
	// Create a video mode object
	sf::VideoMode vm(1920, 1080);
	// Create and open a window for the game
	//sf::RenderWindow window(vm, "Timber", sf::Style::Fullscreen);
	sf::RenderWindow window(vm, "Timber");

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
	beeSprite.setPosition(0, 800);

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

	sf::Clock clock;

	// Time bar
	sf::RectangleShape timeBar;
	float timeBarStartWidth{ 400.0f };
	float timeBarHeight{ 80.0f };
	timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(sf::Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	sf::Time gameTimeTotal;
	float timeRemaining{ 6.0f };
	float timeBarWidthPerSecond{ timeBarStartWidth / timeRemaining };

	bool paused{ true };

	// Draw some text
	int score{ 0 };

	sf::Text messageText;
	sf::Text scoreText;

	// Choosing a font
	sf::Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// Set the font of our message
	messageText.setFont(font);
	scoreText.setFont(font);

	// Assign the actual messages
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score: 0");
	
	// Make the text big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	// Set the color of the text
	messageText.setFillColor(sf::Color::White);
	scoreText.setFillColor(sf::Color::White);

	// Position the text on screen
	sf::FloatRect textRect{ messageText.getLocalBounds() };

	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	scoreText.setPosition(20, 20);

	sf::Texture branchTexture;
	branchTexture.loadFromFile("graphics/branch.png");

	// set the texture for each branch sprite
	for (int i{ 0 }; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(branchTexture);
		branches[i].setPosition(-2000, -2000);

		// set the branches to center
		branches[i].setOrigin(220, 20);
	}

	while (window.isOpen())
	{
		// handle the players input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}

		// Start the game
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			paused = false;

			// Reset the time and the score
			score = 0;
			timeRemaining = 6;
		}

		if (!paused)
		{
			// update the scene

			sf::Time dt{ clock.restart() };

			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			// set the size of the bar
			timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				// Pause the game
				paused = true;

				// Change the message shown 
				messageText.setString("Out of time!");

				// Reposition the text based on new size
				sf::FloatRect textRect{ messageText.getLocalBounds() };
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}

			// Manage the bee
			if (!beeActive)
			{
				// How fast the bee will be
				std::srand((int)time(0)); // seeding the random number generator with current time
				beeSpeed = (std::rand() % 200) + 200.0f;

				// How high the bee is
				std::srand((int)time(0) * 10);
				float height{ (std::rand() % 500) + 500.0f };
				beeSprite.setPosition(2000, height);
			
				beeActive = true;
			}
			else // move the bee
			{
				beeSprite.setPosition(beeSprite.getPosition().x - 
					(beeSpeed * dt.asSeconds()), 
					beeSprite.getPosition().y);

				// has the bee reached the left-hand edge of the screen?
				if (beeSprite.getPosition().x < -100)
				{
					// Set it up as a new bee next frame
					beeActive = false;
				}
			}

			// Manage the clouds
			// cloud 1
			if (!cloud1Active)
			{
				// How fast is the cloud moving
				std::srand((int)time(0) * 10);
				cloud1Speed = (float)(std::rand() % 200);

				// How high is the cloud
				std::srand((int)time(0) * 10);
				float height{ (float)(std::rand() % 150) };
				cloudSprite1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				cloudSprite1.setPosition(
					cloudSprite1.getPosition().x +
					(cloud1Speed * dt.asSeconds()),
					cloudSprite1.getPosition().y);

				// has the cloud reached the edge of the screen
				if (cloudSprite1.getPosition().x > 1920)
				{
					// set it up ass a new cloud
					cloud1Active = false;
				}
			}

			// cloud 2
			if (!cloud2Active)
			{
				std::srand((int)time(0) * 20);
				cloud2Speed = (float)(std::rand() % 200);

				std::srand((int)time(0) * 20);
				float height{ (std::rand() % 300) - 150.0f };
				cloudSprite2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				cloudSprite2.setPosition(
					cloudSprite2.getPosition().x +
					(cloud2Speed * dt.asSeconds()),
					cloudSprite2.getPosition().y);

				if (cloudSprite2.getPosition().x > 1920)
				{
					cloud2Active = false;
				}
			}

			// cloud 3
			if (!cloud3Active)
			{
				std::srand((int)time(0) * 30);
				cloud3Speed = (float)(std::rand() % 200);

				std::srand((int)time(0) * 30);
				float height{ (std::rand() % 450) - 150.0f };
				cloudSprite3.setPosition(-200, height);
				cloud3Active = true;
			}
			else
			{
				cloudSprite3.setPosition(
					cloudSprite3.getPosition().x +
					(cloud3Speed * dt.asSeconds()),
					cloudSprite3.getPosition().y);

				if (cloudSprite3.getPosition().x > 1920)
				{
					cloud3Active = false;
				}
			}

			// update the score 
			std::stringstream ss;
			ss << "Score: " << score;
			scoreText.setString(ss.str());

			// update the branch sprites
			for (int i{ 0 }; i < NUM_BRANCHES; i++)
			{
				float height{ i * 150 };

				if (branchPositions[i] == side::LEFT)
				{
					// Move the sprites to the left side
					branches[i].setPosition(610, height);
					
					// Flip the sprite around the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);
				}
				else
				{
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
			}
		}

		// clear everything from the last frame
		window.clear();
		
		// Draw the game scene
		window.draw(backgroundSprite);
		
		// Draw the clouds
		window.draw(cloudSprite1);
		window.draw(cloudSprite2);
		window.draw(cloudSprite3);

		for (int i{ 0 }; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}

		// Draw the tree
		window.draw(treeSprite);

		// Draw the bee
		window.draw(beeSprite);

		// Draw the score
		window.draw(scoreText);

		window.draw(timeBar);

		if (paused)
		{
			// Draw our start message
			window.draw(messageText);
		}

		// Show everything we just drew
		window.display();
	}

	return 0;
}