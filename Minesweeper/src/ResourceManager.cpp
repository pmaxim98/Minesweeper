#include "ResourceManager.h"

#include <fstream>

bool ResourceManager::init()
{
	std::ofstream fout("log.txt");

	if (!appIcon.loadFromFile("res/icon.png")) {
		fout << "ResourceManager: Game's icon is missing!\n";
		return false;
	}

	try {
		loadFont("PressStart2P.ttf", "Start");
		loadTexture("squares.png", "Squares");
		loadTexture("mine.png", "Mine");
		loadTexture("clock.png", "Clock");
		loadTexture("like.png", "Like");
	}
	catch (std::exception & e) {
		fout << e.what() << '\n';
		return false;
	}

	return true;
}

void ResourceManager::loadTexture(const std::string & filepath, const std::string & id)
{
	sf::Texture tempTexture;

	if (!tempTexture.loadFromFile("res/" + filepath))
		throw std::runtime_error("ResourceManager: Couldn't load the texture at :'" + filepath + "'.\n");

	tempTexture.setSmooth(true);
	textures.insert({id, tempTexture});
}

void ResourceManager::loadFont(const std::string & filename, const std::string & id)
{
	sf::Font tempFont;

	if (!tempFont.loadFromFile("res/" + filename))
		throw std::runtime_error("ResourceManager: Couldn't load the font from '" + filename + "'.\n");

	fonts.insert({id, tempFont});
}

const sf::Texture & ResourceManager::getTexture(const std::string & id) const
{
	const auto found = textures.find(id);

	if (found != textures.end())
		return found -> second;

	throw std::runtime_error("ResourceManager: Texture with id '" + id + "' not found!\n");
}

const sf::Font & ResourceManager::getFont(const std::string & id) const
{
	const auto found = fonts.find(id);

	if (found != fonts.end())
		return found -> second;

	throw std::runtime_error("Font with id '" + id + "' not found!\n");
}

const sf::Uint8 * ResourceManager::getIconPtr() const
{
	return appIcon.getPixelsPtr();
}
