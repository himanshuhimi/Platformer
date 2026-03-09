#include "../map.h"

Map::Map(SDL_Renderer *renderer, string source)
{
    XMLDocument doc;
    doc.LoadFile(source.c_str());
    mapElement = doc.FirstChildElement("map");
    width = mapElement->IntAttribute("width");
    height = mapElement->IntAttribute("height");
    tileWidth = mapElement->IntAttribute("tilewidth");
    tileHeight = mapElement->IntAttribute("tileheight");
    for (
        XMLElement *child = mapElement->FirstChildElement();
        child != nullptr;
        child = child->NextSiblingElement()
    )
    {
        const char *childName = child->Name();
        if (strcmp(childName, "layer")) loadLayer(child);
        else if (strcmp(childName, "tileset")) loadTileset(child);
    }
}