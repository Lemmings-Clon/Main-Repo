#ifndef SFMLTILEX_H
#define SFMLTILEX_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "NLTmxMap.hpp"

enum class tileshape {SKY=8,UPPERPORTAL=12, LOWERPORTAL=16, LEFTBLOCK=1, MIDDLEBLOCK=2, RIGHTBLOCK=3, START = 15, LAVA =5, DIGBLOCK=6}; //Enumeration der verschiedenen Tiles

struct tilePos{
	tileshape type;  //Art des Tileshapes
	sf::ConvexShape tilepos; //Tileshap an der richtigen Position
};

class SFMLtilex {

private: 

	/* Map */
	char* cFilename;  //Filename der tmx map
	NLTmxMap* cMap; //tilex-Map
	sf::Vector2f startPoint;  //Position des Startpunkts
	sf::Texture cTexture; // Speichert die Textur der Tiles
	bool textureLoaded; //Zum überprüfen ob die Textur schon geladen wurde
	//Sprites neu laden
	void refreshSprites(); 
	std::vector<sf::Sprite> cSprites; //Speichert die Sprites der einzelnen Tiles
	sf::Vector2f bounds; //größe der Karte

	/* Das Tileset (wie die Textur auszulesen ist) */
	NLTmxMapTileset* cTileset; //Das Tileset (wie die Textur auszulesen ist)
	int tWidth; //Breite einer Tile in px
	int tHeight; //Höhe einer Tile in px

	/* Der Layer (Wie die Map gezeichnet wird) */
	NLTmxMapLayer* cLayer; //Data-Komponente sind die Tile-Werte in einem int-array
	int mapWidth; // Anzahl der Tiles in der Breite
	int mapHeight; //Anzahl der Tiles in der Höhe

	/* Kollision */
	/////////////////////////////////////////////////////////////////////
	///Vector von Kollisions-Tile-Arten mit denen Kollidiert werden kann 
	///Position im Array ist die Enum-Nummer                             
	///Statisch, kennt nur hardcodete tileshapes                         
	/////////////////////////////////////////////////////////////////////
	std::vector<sf::ConvexShape> tileshapes; 
	//Kollisions-Tile-Arten in Vector laden (hartcoded)
	void  loadTileshapes(); 
	std::vector<tilePos> cTilePos; //Kollisons-Map
	bool debugColmap; //Kollisons-Map anzeigen zum debuggen
	//Kollisons-Map laden und Startpunkt berechen
	void calcStart(); 

public:
	SFMLtilex(char* filename);  //Constructor, lädt gleich die tilemap aus einer Datei
	//colMap anzeigen an/aus
	void changeColMap(){debugColmap = (!debugColmap);};  //colMap anzeigen an/aus
	//Gesamtgröße der Karte zurückgeben
	sf::Vector2f getBounds(){return bounds;};  
	//Position des Startpunktes 
	sf::Vector2f getStart(){return startPoint;}; 
	//Lifert die Kollition-Map in einem tilePos vector
	std::vector<tilePos>  getTilePos(){return cTilePos;};
	//layer auswählen
	void selectLayer(unsigned short layer); 
	//tileset auswählen
	void selectTileset(unsigned short tileset); 
	//Breite des Spielfeldes zurückgeben in px
	int getTotalWidth(){return tWidth*mapWidth;}; 

	//Höhe des Spielfeldes zurückgeben in px
	int getTotalHeight(){return tHeight*mapHeight;}; 
	//Inhalt der XML ausgeben
	void showMapContent(); 
	//Tilemap neu aus datei laden 
	void reloadMap(); 
	//Textur laden
	int loadTexture();
	//Map Zeichen
	void draw(sf::RenderWindow& window); 
};

#endif