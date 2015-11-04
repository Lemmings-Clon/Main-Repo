#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "NLTmxMap.h"

class SFMLtilex {
private: 
	NLTmxMap* cMap;

	NLTmxMapTileset* cTileset;
	int tWidth;
	int tHeight;

	NLTmxMapLayer* cLayer;
	int mapWidth;
	int mapHeight;

	bool textureLoaded;
	sf::Texture cTexture;

	//Anzahl der Tiles in X und Y richtung
	int xTiles;
	int yTiles;

	std::vector<sf::Sprite> cSprites;
		
	void refreshSprites();
public:
	SFMLtilex(char* filename);

	void selectLayer(unsigned short layer); //layer auswählen
	void selectTileset(unsigned short tileset); //tileset auswählen

	int getTotalWidth(){return tWidth*mapWidth;};
	int getTotalHeight(){return tHeight*mapHeight;};
	void showMapContent(); //Inhalt der XML ausgeben

	int loadTexture();
	void draw(sf::RenderWindow& window); //Map Zeichen



};