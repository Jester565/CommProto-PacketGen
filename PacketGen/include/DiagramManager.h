#pragma once
#include <string>
#include <allegro5/allegro_font.h>

static const int BOX_FONT_SIZE = 20;
static const int BITMAP_H = 87;
static const int TITLE_FONT_SIZE = 20;
static const int BIT_FONT_SIZE = 16;
static const int TITLE_Y = 5;
static const int BOX_H = 40;
static const int LINE_W = 2;
static const int BORDER_W = 20;
static const float IMG_SCALE = 2;
static const int BOX_Y = BITMAP_H - BOX_H - LINE_W;
static const int DIAGRAM_BOX_OFF = 5;
static const std::string FONT_NAME = "./Calibri.ttf";

class Message;

class DiagramManager
{
public:
		DiagramManager();

		bool init();

		bool makeDiagram(Message* msg, const std::string& outputDir, const std::string& fileEnding);

		~DiagramManager();

private:
		ALLEGRO_FONT* bitFont;
		ALLEGRO_FONT* boxFont;
		ALLEGRO_FONT* titleFont;
};
