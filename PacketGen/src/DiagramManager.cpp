#include "DiagramManager.h"
#include "Message.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

DiagramManager::DiagramManager()
		:bitFont(nullptr), boxFont(nullptr), titleFont(nullptr)
{
		
}

bool DiagramManager::init()
{
		bool failure = false;
		failure |= !al_init();
		failure |= !al_init_image_addon();
		failure |= !al_init_primitives_addon();
		failure |= !al_init_font_addon();
		failure |= !al_init_ttf_addon();
		if (failure)
		{
				std::cerr << "Unable to initialize allegro" << std::endl;
				return false;
		}
		boxFont = al_load_ttf_font(FONT_NAME.c_str(), BOX_FONT_SIZE * IMG_SCALE, 0);

		titleFont = al_load_ttf_font(FONT_NAME.c_str(), TITLE_FONT_SIZE * IMG_SCALE, 0);

		bitFont = al_load_ttf_font(FONT_NAME.c_str(), BIT_FONT_SIZE * IMG_SCALE, 0);
		if (bitFont == nullptr || boxFont == nullptr || titleFont == nullptr)
		{
				std::cerr << "Unable to load font " << FONT_NAME << std::endl;
				return false;
		}

		return true;
}

bool DiagramManager::makeDiagram(Message * msg, const std::string& outputDir, const std::string & fileEnding)
{
		std::vector <Field*>& fields = msg->getFields();
		float maxBoxW = 0;
		for (int i = 0; i < fields.size(); i++)
		{
				Field* field = fields.at(i);
				if (field->type->numBytes <= 0)
				{
						std::cerr << "ERROR: Unable to represent type \"" << field->type->name << "\" in a byte diagram... message \"" << msg->getName() << "\" will not be generated" << std::endl;
						return true;
				}
				float boxW = (float)(al_get_text_width(boxFont, field->name->c_str()) / (float)field->type->numBytes) / IMG_SCALE;
				if (boxW > maxBoxW)
				{
						maxBoxW = boxW;
				}
		}
		float totalBoxW = 0;
		for (int i = 0; i < fields.size(); i++)
		{
				Field* field = fields.at(i);
				totalBoxW += DIAGRAM_BOX_OFF;
				totalBoxW += maxBoxW * field->type->numBytes;
				totalBoxW += DIAGRAM_BOX_OFF;
		}
		float titleW = (al_get_text_width(titleFont, msg->getName().c_str()))/IMG_SCALE;
		float bmpW = titleW > totalBoxW ? titleW : totalBoxW;
		bmpW += BORDER_W * 2;

		ALLEGRO_BITMAP* bmp = al_create_bitmap(bmpW * IMG_SCALE, BITMAP_H * IMG_SCALE);
		if (bmp == nullptr)
		{
				std::cerr << "FATAL ERROR: Could not create bitmap. Stopping all diagram generation." << std::endl;
				return false;
		}
		al_set_target_bitmap(bmp);
		al_clear_to_color(al_map_rgb(255, 255, 255));
		al_draw_text(titleFont, al_map_rgb(0, 0, 0), (bmpW / 2) * IMG_SCALE, TITLE_Y * IMG_SCALE, ALLEGRO_ALIGN_CENTER, msg->getName().c_str());
		float box_X = (bmpW - totalBoxW) / 2;
		int bitCount = 0;
		al_draw_text(bitFont, al_map_rgb(0, 0, 0), box_X * IMG_SCALE, (BOX_Y - BIT_FONT_SIZE) * IMG_SCALE, ALLEGRO_ALIGN_CENTER, std::to_string(bitCount).c_str());
		for (int i = 0; i < fields.size(); i++)
		{
				Field* field = fields.at(i);
				float boxW = maxBoxW * field->type->numBytes + DIAGRAM_BOX_OFF * 2;
				bitCount += field->type->numBytes * 8;
				al_draw_rectangle(box_X * IMG_SCALE, BOX_Y * IMG_SCALE, (box_X + boxW) * IMG_SCALE, (BOX_Y + BOX_H) * IMG_SCALE, al_map_rgb(0, 0, 0), (LINE_W * IMG_SCALE));
				al_draw_text(boxFont, al_map_rgb(0, 0, 0), (box_X + boxW / 2) * IMG_SCALE, (BOX_Y + BOX_H/4) * IMG_SCALE, ALLEGRO_ALIGN_CENTER, field->name->c_str());
				box_X += boxW;
				al_draw_text(bitFont, al_map_rgb(0, 0, 0), box_X * IMG_SCALE, (BOX_Y - BIT_FONT_SIZE) * IMG_SCALE, ALLEGRO_ALIGN_CENTER, std::to_string(bitCount - 1).c_str());
		}
		al_set_target_bitmap(nullptr);
		std::string outPath = outputDir;
		outPath += '/';
		outPath += msg->getName();
		outPath += ".";
		outPath += fileEnding;
		if (!al_save_bitmap(outPath.c_str(), bmp))
		{
				std::cerr << "FATAL ERROR: Could not save diagram of message \"" << msg->getName() << "\". Keep in mind that the file ending matters, png and jpg should work" << std::endl;
				return false;
		}
		al_destroy_bitmap(bmp);
		bmp = nullptr;
		return true;
}

DiagramManager::~DiagramManager()
{
		if (bitFont)
		{
				al_destroy_font(bitFont);
		}
		if (boxFont)
		{
				al_destroy_font(boxFont);
		}
		if (titleFont)
		{
				al_destroy_font(titleFont);
		}
		al_shutdown_font_addon();
		al_shutdown_image_addon();
		al_shutdown_primitives_addon();
		al_shutdown_ttf_addon();
}
