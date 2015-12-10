#ifndef SFMLTILEX_H
#define SFMLTILEX_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "NLTmxMap.hpp"

//enum class tileshape {SKY=8,UPPERPORTAL=12, LOWERPORTAL=16, LEFTBLOCK=1, MIDDLEBLOCK=2, RIGHTBLOCK=3, START = 15, LAVA =5, DIGBLOCK=6, MIDDLEBREAK_1 = 6, MIDDLEBREAK_2 = 10, SPIKE_UP = 17, SPIKE_DOWN = 18}; //Enumeration der verschiedenen Tiles
enum class tileshape { SKY = 10, UPPERPORTAL = 16, LOWERPORTAL = 22, LEFTBLOCK = 1, MIDDLEBLOCK = 2, RIGHTBLOCK = 3, START = 21, LAVA = 7, DIGBLOCK = 15, MIDDLEBREAK_1 = 8, MIDDLEBREAK_2 = 14, SPIKE_UP = 25, SPIKE_DOWN = 26 }; //Enumeration der verschiedenen Tiles


struct tilePos{
	tileshape type;  //Art des Tileshapes
	sf::ConvexShape tilepos; //Tileshap an der richtigen Position
};

class SFMLtilex {

public: 

	/* Map */
	char* cFilename;  //Filename der tmx map
	NLTmxMap* cMap; //tilex-Map
	sf::Vector2f startPoint;  //Position des Startpunkts
	sf::Texture cTexture; // Speichert die Textur der Tiles
	bool textureLoaded; //Zum �berpr�fen ob die Textur schon geladen wurde
	//Sprites neu laden
	void refreshSprites(); 
	std::vector<sf::Sprite> cSprites; //Speichert die Sprites der einzelnen Tiles
	sf::Vector2f bounds; //gr��e der Karte
	void updateMap(int x, int y, tileshape newshape) { cLayer->data[mapWidth*y+x] = (int)newshape; } //Gibt einer Kachel einen neuenshape

	/* Das Tileset (wie die Textur auszulesen ist) */
	NLTmxMapTileset* cTileset; //Das Tileset (wie die Textur auszulesen ist)
	int tWidth; //Breite einer Tile in px
	int tHeight; //H�he einer Tile in px

	/* Der Layer (Wie die Map gezeichnet wird) */
	NLTmxMapLayer* cLayer; //Data-Komponente sind die Tile-Werte in einem int-array
	int mapWidth; // Anzahl der Tiles in der Breite
	int mapHeight; //Anzahl der Tiles in der H�he

	/* Kollision */
	/////////////////////////////////////////////////////////////////////
	///Vector von Kollisions-Tile-Arten mit denen Kollidiert werden kann 
	///Position im Array ist die Enum-Nummer                             
	///Statisch, kennt nur hardcodete tileshapes                         
	/////////////////////////////////////////////////////////////////////
	std::vector<sf::ConvexShape> tileshapes; 
	//Kollisions-Tile-Arten in Vector laden (hartcoded)
	void  loadTileshapes(); 
	std::vector<tilePos> cTileColMap; //Kollisons-Map
	bool debugColmap; //Kollisons-Map anzeigen zum debuggen
	//Kollisons-Map laden und Startpunkt berechen
	void calcStart(); 

public:
	SFMLtilex(char* filename);  //Constructor, l�dt gleich die tilemap aus einer Datei
	//colMap anzeigen an/aus
	void changeColMap(){debugColmap = (!debugColmap);};  //colMap anzeigen an/aus
	//Gesamtgr��e der Karte zur�ckgeben
	sf::Vector2f getBounds(){return bounds;};  
	//Position des Startpunktes 
	sf::Vector2f getStart(){return startPoint;}; 
	//Lifert die Kollition-Map in einem tilePos vector
	std::vector<tilePos>  getTilePos(){return cTileColMap;};
	//layer ausw�hlen
	void selectLayer(unsigned short layer); 
	//tileset ausw�hlen
	void selectTileset(unsigned short tileset); 
	//Breite des Spielfeldes zur�ckgeben in px
	int getTotalWidth(){return tWidth*mapWidth;}; 

	//H�he des Spielfeldes zur�ckgeben in px
	int getTotalHeight(){return tHeight*mapHeight;}; 
	//Inhalt der XML ausgeben
	void showMapContent(); 
	//Tilemap neu aus datei laden 
	void reloadMap(); 
	//Textur laden
	int loadTexture();
	//Map Zeichen
	void draw(sf::RenderWindow& window); 
	void digBlock(int x, int y, tileshape newshape) { updateMap( x,  y,  newshape); }
	//Breite eines tiles in px
	const int getTileWidth() { return tWidth; }
	//H�he eines tiles in px
	const int getTileHeight() { return tHeight; }
	//Anzahl der Tiles in X Richtung
	const int getMapWidth() { return mapWidth; }
	//Anzahl der Tiles in Y Richtung
	const int getMapHeight() { return mapHeight; }
};

#endif