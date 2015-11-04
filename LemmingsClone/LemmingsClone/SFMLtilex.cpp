#include "SFMLtilex.h"

static void* loadMyFile(const char * filename, bool appendNull) {

	FILE* f;
	int error = fopen_s(&f,filename, "r");
	if (!f) {
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
	}

	return buffer;
}


NLTmxMap* loadMyMap(char* filename){

	char * xml = (char*)loadMyFile(filename, true);
	return NLLoadTmxMap(xml);

}


SFMLtilex::SFMLtilex(char* filename){
	
	cMap = loadMyMap(filename);
	selectLayer(0);
	selectTileset(0);
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

	
		sf::Vector2u v = cTexture.getSize();
		//Ausrechnen wie viel Tiles es in X und Y richtung gibt
		xTiles=v.x/tWidth;
		yTiles=v.y/tHeight;


		textureLoaded = true;

	return 0;
}

void SFMLtilex::refreshSprites(){
		
		sf::Sprite sprite;
		int counter = 0;

		for(int i = 0; i < yTiles; i++){
			for(int j = 0; j < xTiles; j++){
				sf::Sprite sprite;
				sprite.setTextureRect(sf::IntRect(j*tWidth, i*tHeight, tWidth, tHeight));
				sprite.setTexture(cTexture);
				cSprites.push_back(sprite);
			}
		}
}

void SFMLtilex::draw(sf::RenderWindow& window){

	if(!textureLoaded){
		cout << "Cannot draw, you have to load the Texture first" << endl;
		return;
	}
	
	refreshSprites();
	sf::RectangleShape rectangle(sf::Vector2f((float)tWidth, (float)tHeight));
	
	rectangle.setFillColor(sf::Color::Red);
	sf::Vector2f position;
	
	//cout << "C Sprites: " << cSprites.size() << endl;


	for(int i = 0; i < mapHeight; ++i){
		for(int j = 0; j < mapWidth; ++j){
			position.x = (float) j* tWidth;
			position.y = (float) i * tHeight;
	
			sf::Sprite realsp;
			int Nr = i*mapWidth+j;
			int sNr = cLayer->data[Nr];
		
			realsp = cSprites.at(sNr-1);

			realsp.setPosition(position);
			window.draw(realsp);
		   }
	   }
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