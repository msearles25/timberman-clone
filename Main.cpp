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

	// Preparing the player sprite
	sf::Texture playerTexture;
	playerTexture.loadFromFile("graphics/player.png");
	sf::Sprite playerSprite;
	playerSprite.setTexture(playerTexture);
	playerSprite.setPosition(580, 720);

	// the player will start on the left side
	side playerSide{ side::LEFT };

	// Preparing the gravestone
	sf::Texture graveTexture;
	graveTexture.loadFromFile("graphics/rip.png");
	sf::Sprite graveSprite;
	graveSprite.setTexture(graveTexture);
	graveSprite.setPosition(600, 860);

	// Preparing the axe
	sf::Texture axeTexture;
	axeTexture.loadFromFile("graphics/axe.png");
	sf::Sprite axeSprite;
	axeSprite.setTexture(axeTexture);
	axeSprite.setPosition(700, 830);

	// Line the axe up with the tree
	const float AXE_POSITION_LEFT{ 700 };
	const float AXE_POSITION_RIGHT{ 1075 };

	// Preparing the flying log
	sf::Texture logTexture;
	logTexture.loadFromFile("graphics/log.png");
	sf::Sprite logSprite;
	logSprite.setTexture(logTexture);
	logSprite.setPosition(810, 720);

	bool logActive{ false };
	float logSpeedX{ 1000 };
	float logSpeedY{ -1500 };

	// Control the player input
	bool acceptInput{ false };

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyReleased && !paused)
			{
				// listen for the key press again
				acceptInput = true;

				// hide the axe
				axeSprite.setPosition(2000, axeSprite.getPosition().y);
			}
		}

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

			// Make all the branches disappear
			// starting in the second position
			for (int i{ 1 }; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			// Make sure the gravestone is hidden
			graveSprite.setPosition(675, 2000);

			// Move the player into position
			playerSprite.setPosition(580, 720);

			acceptInput = true;
		}

		// Make sure we are accepting inputs from the player
		if (acceptInput)
		{
			// Hanlde pressing the right cursor key
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				playerSide = side::RIGHT;
				score++;

				// Add to the time remaining
				timeRemaining += (2 / score) + .15f;
				
				axeSprite.setPosition(AXE_POSITION_RIGHT, axeSprite.getPosition().y);

				playerSprite.setPosition(1200, 720);

				// update the branches
				updateBranches(score);

				// Set the log flying to the left
				logSprite.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;
			}

			// Handle pressing the left key
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				// make sure the player is on the left
				playerSide = side::LEFT;
				score++;

				timeRemaining += (2 / score) + .15f;

				axeSprite.setPosition(AXE_POSITION_LEFT, axeSprite.getPosition().y);

				playerSprite.setPosition(580, 720);

				updateBranches(score);

				logSprite.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
			}
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
				float height{ (float)(i * 150) };

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

			// Handle the flying log
			if (logActive)
			{
				logSprite.setPosition(logSprite.getPosition().x +
					(logSpeedX * dt.asSeconds()),

					logSprite.getPosition().y +
					(logSpeedY * dt.asSeconds()));

				// Has the log reached the edge?
				if (logSprite.getPosition().x < -100 ||
					logSprite.getPosition().x > 2000)
				{
					// set it up to be used for a new log
					logActive = false;
					logSprite.setPosition(810, 720);
				}
			}

			// Handle squashing the player
			if (branchPositions[5] == playerSide)
			{
				// death
				paused = true;
				acceptInput = false;

				// Draw the gravestone
				graveSprite.setPosition(525, 760);

				// Hide the player
				playerSprite.setPosition(2000, 660);

				// Change the message
				messageText.setString("SQUISHED!");

				// center on the screen
				sf::FloatRect textRect{ messageText.getLocalBounds() };

				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
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

		// Draw the player
		window.draw(playerSprite);

		// Draw the axe
		window.draw(axeSprite);

		// Draw the flying log
		window.draw(logSprite);

		// Draw the gravestone
		window.draw(graveSprite);

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

void updateBranches(int seed)
{
	// Move all branches down one place
	for (int i{ NUM_BRANCHES - 1 }; i > 0; i--)
	{
		branchPositions[i] = branchPositions[i - 1];
	}

	// Spawn a new branch at position 0
	std::srand((int)time(0) + seed);
	int r{ rand() % 5 };

	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}