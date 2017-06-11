#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <map>
#include <string>

class ResourceManager
{
	public:
		static ResourceManager & getInstance()
        {
            static ResourceManager instance;
            return instance;
        }

		const sf::Texture & getTexture(const std::string & id) const;
		const sf::Font & getFont(const std::string & id) const;
		const sf::Uint8 * getIconPtr() const;

		bool init();
		void loadTexture(const std::string & filepath, const std::string & id);
		void loadFont(const std::string & filepath, const std::string & id);

	private:
		ResourceManager() { }

		std::map<std::string, sf::Texture> textures;
		std::map<std::string, sf::Font> fonts;
		sf::Image appIcon;

		ResourceManager(const ResourceManager &) = delete;
		void operator=(const ResourceManager &) = delete;
};

#endif