#include "SFMLtilex.hpp"

using namespace::std;

static void* loadMyFile(const char * filename, bool appendNull) {

	FILE* f;
	int error = fopen_s(&f,filename, "r");
	if (!f) {
		cout << "cannot open file!!" << endl;
		return 0;
	}

	fseek(f, 0, SEEK_END);
	auto length = ftell(f) + appendNull;
	fseek(f, 0, SEEK_SET);

	void* buffer = malloc(length);
	fread(buffer, length, 1, f);
	fclose(f);

	if (appendNull) {
		((char*)buffer)[length - 1] = 0;




	/*Null an die richtige stelle setzen damit debug modus funktioniert*/
	std::string str((char*)buffer);
	std::size_t found = str.find("/map")+5;

	((char*)buffer)[found] =0;
	
	}
	return buffer;
}

NLTmxMap* loadMyMap(char* filename){

	char * xml = (char*)loadMyFile(filename, true);
	return NLLoadTmxMap(xml);

}

SFMLtilex::SFMLtilex(char* filename){
	cFilename = filename;
	debugColmap = false;
	loadTileshapes();
	reloadMap();
}

void SFMLtilex::reloadMap(){
	cMap = loadMyMap(cFilename);
	selectLayer(0);					
	selectTileset(0);
	calcStart();
	bounds.x = (float)getTotalWidth();
	bounds.y = (float)getTotalHeight();
	textureLoaded = false;
}

void SFMLtilex::selectLayer(unsigned short layer){

	if(layer>cMap->layers.size())
		return;

	cLayer = cMap->layers.at(layer);
	mapHeight = cLayer->height;
	mapWidth = cLayer->width;
	textureLoaded = false;
}

void SFMLtilex::selectTileset(unsigned short layer){

	if(layer>cMap->tilesets.size())
		return;

	cTileset = cMap->tilesets.at(layer);
	tWidth = cTileset->tileWidth;
	tHeight = cTileset->tileHeight;

	textureLoaded = false;
}

int SFMLtilex::loadTexture(){
	if(textureLoaded)
		return 1;

	if (!cTexture.loadFromFile(cTileset->filename))
	{
		return 5;
	}

	textureLoaded = true;

	refreshSprites();

	return 0;
}

void SFMLtilex::refreshSprites(){

	//Ausrechnen wie viel Tiles es in X und Y auf der Textur gibt

	sf::Vector2u v = cTexture.getSize();
	int xTiles=v.x/tWidth;
	int yTiles=v.y/tHeight;

	cSprites.clear();
	cSprites.reserve(1+(yTiles*xTiles));

	for(int i = 0; i < yTiles; i++){
		for(int j = 0; j < xTiles; j++){
			sf::Sprite sprite;
			sprite.setTextureRect(sf::IntRect(j*tWidth, i*tHeight, tWidth, tHeight));
			sprite.setTexture(cTexture);
			cSprites.push_back(sprite);
		}
	}
}

void SFMLtilex::calcStart(){

	int const * p = cLayer->data;  //Pointer auf die aktuelle Tile im xml
	cTileColMap.clear();
	cTileColMap.reserve(cLayer->height*cLayer->width);
	sf::Vector2f vec; //hier wird die Position der aktuellen kollisions-tile gespeichert

	for(int i = 0; i < mapHeight; ++i) {
		for(int j = 0; j < mapWidth; ++j){

			vec.x = (float)j*tWidth;
			vec.y = (float)i*tHeight;

			sf::ConvexShape sh;

			if(tileshapes[*p].getPointCount()){
				sh = tileshapes[*p];
				sh.setPosition(vec);
				sh.setFillColor(sf::Color::Red);

				tilePos tp;
				tp.type = (tileshape)*p;
				tp.tilepos = sh;

				cTileColMap.push_back(tp);
			}

			if(*p++ == (int)tileshape::START)  {
				startPoint.x= vec.x;
				startPoint.y= vec.y;
				//cout << "Start Point: " << *p << " " << startPoint.x << " " << startPoint.y << endl;
			}
		}
	}
	//cout << "shapes: " << cTileColMap.tilePos.size() << endl;
}

void SFMLtilex::draw(sf::RenderWindow& window){
	


	if(!textureLoaded){
		cout << "Cannot draw, you have to load the Texture first" << endl;
		return;
	}


	int Nr;  //Counter der zu zeichnenden Tiles
	int sNr; //spritenummer des zu zeichnenden Tiles
	sf::Vector2f position; //Position des zu zeichenden TIles

	for(int i = 0; i < mapHeight; ++i){
		for(int j = 0; j < mapWidth; ++j){
			position.x = (float) j* tWidth;
			position.y = (float) i * tHeight;

			sf::Sprite drawSprite;
			Nr = i*mapWidth+j;
			sNr = cLayer->data[Nr];

			drawSprite =cSprites.at(sNr-1);
			drawSprite.setPosition(position);

			window.draw(drawSprite);
		}
	}

	if(debugColmap)
		for(int i = 0; i < cTileColMap.size(); ++i)
			window.draw(cTileColMap[i].tilepos);
}

void SFMLtilex::loadTileshapes(){
	tileshapes.clear();
	tileshapes.resize(40);

	/* LOWER PORTAL */

	sf::ConvexShape shapeLowerportal;
	shapeLowerportal.setPointCount(4);

	shapeLowerportal.setPoint(0, sf::Vector2f(5,0));
	shapeLowerportal.setPoint(1, sf::Vector2f(27,0));
	shapeLowerportal.setPoint(2, sf::Vector2f(27,32));
	shapeLowerportal.setPoint(3, sf::Vector2f(5,32));

	tileshapes.insert (tileshapes.begin() + (int)tileshape::LOWERPORTAL, shapeLowerportal);
	tileshapes.erase (tileshapes.begin()+ (int)tileshape::LOWERPORTAL +1);


	/* MIDDLE BLOCK */

	sf::ConvexShape shapeMiddleBlock;
	shapeMiddleBlock.setPointCount(4);

	shapeMiddleBlock.setPoint(0, sf::Vector2f(0,0));
	shapeMiddleBlock.setPoint(1, sf::Vector2f(32,0));
	shapeMiddleBlock.setPoint(2, sf::Vector2f(32,32));
	shapeMiddleBlock.setPoint(3, sf::Vector2f(0,32));

	tileshapes.insert (tileshapes.begin() + (int)tileshape::MIDDLEBLOCK, shapeMiddleBlock);
	tileshapes.erase (tileshapes.begin()+ (int)tileshape::MIDDLEBLOCK +1);
	tileshapes.insert(tileshapes.begin() + (int)tileshape::MIDDLEBREAK_1, shapeMiddleBlock);
	tileshapes.erase(tileshapes.begin() + (int)tileshape::MIDDLEBREAK_1 + 1);
	tileshapes.insert(tileshapes.begin() + (int)tileshape::MIDDLEBREAK_2, shapeMiddleBlock);
	tileshapes.erase(tileshapes.begin() + (int)tileshape::MIDDLEBREAK_2 + 1);


	/* UPPER PORTAL */

	sf::ConvexShape shapeUpperportal;
	shapeUpperportal.setPointCount(4);

	shapeUpperportal.setPoint(3, sf::Vector2f(5,32));
	shapeUpperportal.setPoint(2, sf::Vector2f(11,21));
	shapeUpperportal.setPoint(1, sf::Vector2f(25,21));
	shapeUpperportal.setPoint(0, sf::Vector2f(27,32));


	tileshapes.insert (tileshapes.begin() + (int)tileshape::UPPERPORTAL, shapeUpperportal);
	tileshapes.erase (tileshapes.begin()+ (int)tileshape::UPPERPORTAL +1);

	/* LEFT SHAPE */
	sf::ConvexShape leftShape;
	leftShape.setPointCount(4);

	leftShape.setPoint(0, sf::Vector2f(2,3));
	leftShape.setPoint(1, sf::Vector2f(32,0));
	leftShape.setPoint(2, sf::Vector2f(32,32));
	leftShape.setPoint(3, sf::Vector2f(2,26));

	tileshapes.insert (tileshapes.begin() + (int)tileshape::LEFTBLOCK, leftShape);
	tileshapes.erase (tileshapes.begin()+ (int)tileshape::LEFTBLOCK +1);

	/* RIGHT SHAPE */

	sf::ConvexShape rightShape;
	rightShape.setPointCount(4);

	rightShape.setPoint(0, sf::Vector2f(0,0));
	rightShape.setPoint(1, sf::Vector2f(30,3));
	rightShape.setPoint(2, sf::Vector2f(30,26));
	rightShape.setPoint(3, sf::Vector2f(0,32));


	tileshapes.insert (tileshapes.begin() + (int)tileshape::RIGHTBLOCK, rightShape);
	tileshapes.erase (tileshapes.begin()+ (int)tileshape::RIGHTBLOCK +1);

	/* LAVA */

	sf::ConvexShape shapeLava;
	shapeLava.setPointCount(4);

	shapeLava.setPoint(0, sf::Vector2f(3,3));
	shapeLava.setPoint(1, sf::Vector2f(29,3));
	shapeLava.setPoint(2, sf::Vector2f(29,29));
	shapeLava.setPoint(3, sf::Vector2f(3,29));


	tileshapes.insert (tileshapes.begin() + (int)tileshape::LAVA, shapeLava);
	tileshapes.erase (tileshapes.begin()+ (int)tileshape::LAVA +1);

	/* DIG*/

	sf::ConvexShape shapedigblock;
	shapedigblock.setPointCount(4);

	shapedigblock.setPoint(0, sf::Vector2f(0, 0));
	shapedigblock.setPoint(1, sf::Vector2f(33, 0));
	shapedigblock.setPoint(2, sf::Vector2f(33, 32));
	shapedigblock.setPoint(3, sf::Vector2f(0, 32));

	tileshapes.insert(tileshapes.begin() + (int)tileshape::DIGBLOCK, shapedigblock);
	tileshapes.erase(tileshapes.begin() + (int)tileshape::DIGBLOCK + 1);

	/*SPIKES_UP*/

	sf::ConvexShape shapeSpikeUP;
	shapeSpikeUP.setPointCount(4);

	shapeSpikeUP.setPoint(0, sf::Vector2f(3, 3));
	shapeSpikeUP.setPoint(1, sf::Vector2f(29, 3));
	shapeSpikeUP.setPoint(2, sf::Vector2f(29, 29));
	shapeSpikeUP.setPoint(3, sf::Vector2f(3, 29));


	tileshapes.insert(tileshapes.begin() + (int)tileshape::SPIKE_UP, shapeSpikeUP);
	tileshapes.erase(tileshapes.begin() + (int)tileshape::SPIKE_UP + 1);

	/*SPIKES_DOWN*/

	sf::ConvexShape shapeSpikeDOWN;
	shapeSpikeDOWN.setPointCount(4);

	shapeSpikeDOWN.setPoint(0, sf::Vector2f(3, 3));
	shapeSpikeDOWN.setPoint(1, sf::Vector2f(29, 3));
	shapeSpikeDOWN.setPoint(2, sf::Vector2f(29, 29));
	shapeSpikeDOWN.setPoint(3, sf::Vector2f(3, 29));


	tileshapes.insert(tileshapes.begin() + (int)tileshape::SPIKE_DOWN, shapeSpikeDOWN);
	tileshapes.erase(tileshapes.begin() + (int)tileshape::SPIKE_DOWN + 1);
}

void SFMLtilex::showMapContent(){
	NLTmxMap* map = cMap;
	int i = 0;


	cout << endl << "Tilesets :" << map->tilesets.size() << endl;
	for ( auto ts : map->tilesets ) {
		cout << "NUMMER: " << ++i << " "<<
			"FILNENAME: " << ts->filename << " " <<
			"FIRST GRID: " << ts->firstGid << " " <<
			"NAME: " << ts->name << " " <<
			"TILE HEIGHT: " << ts->tileHeight << " " <<
			"TILE WIDTH: "<< ts->tileWidth << endl;
	}

	i=0;
	cout << endl << "Map layer :" << map->layers.size() << endl;
	for ( auto ts : map->layers) {
		cout << "NUMMER: " << ++i << " "<<
			"HEIGHT: " << ts->height << " " <<
			"NAME: " << ts->name << " " <<
			"WIDTH: " << ts->width << " " << 

			"DATA: " << endl;

		int* j = ts->data;

		for(int k = 1; k <= ts->height * ts->width; k++){
			cout << *j++ << " ";

			if((k%ts->width) == 0)
				cout << endl;
		}
		cout << endl << endl;
	}
	i=0;
	cout << endl << endl << endl << "Object groups :" << map->groups.size() << endl;
	for ( auto ts : map->groups) {
		cout << "NUMMER: " << ++i << " "<<
			"HEIGHT: " << ts->height<< " " <<
			"NAME: " << ts->name << " " <<
			"VISIBLE: " << ts->visible << " " <<
			"WIDTH: " << ts->width << " " << 
			"OBJECTS:" <<
			endl;
	}


	return;

}