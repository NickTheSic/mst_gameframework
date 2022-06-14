#include "imUI.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "mstgl.h"
#include "mstDebug.h"

namespace ui
{	
	constexpr int MAX_VERTICES = 100;
	constexpr int START_CHARACTER = 32;
	constexpr int LAST_CHARCTER = 126;
	constexpr int CUSTOM_TEXTURES = 0;
	constexpr int TEXTURE_ARRAY_START_OFFSET = START_CHARACTER - CUSTOM_TEXTURES;
	constexpr int MAX_TEXUTURES = (LAST_CHARCTER - TEXTURE_ARRAY_START_OFFSET);
	constexpr const char* FONT_FILE_PATH = "Data/caviardreams.ttf";
	constexpr int FONT_SIZE = 16;

	struct Texture
	{
		v2f bl_coord;
		v2f ur_coord;
	};

	struct Glyph
	{
		v2i size;
		v2i bearing;
		v2f offset;
		int advance;
	};

	struct State
	{
		~State();

		v2f Coordinates[MAX_VERTICES];
		v2f Vertices[MAX_VERTICES];
		RGBA Colours[MAX_VERTICES];

		Texture Textures[126];

		unsigned int vao, vbo, vert_count;
		unsigned int UITexture;
	};

	// Global State
	State state;

	void Init()
	{
		glGenVertexArrays(1, &state.vao);
		glGenBuffers(1, &state.vbo);
		glGenTextures(1, &state.UITexture);

		unsigned int SheetWidth = 0;
		unsigned int SheetHeight = 0;

		// Init Font Sheet;
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			dbglog("ERROR::FREETYPE: Could not init FreeType Library");
			dbgbreak("ERROR::FREETYPE: Could not init FreeType Library");
			return;
		}

		FT_Face face;
		if (FT_New_Face(ft, FONT_FILE_PATH, 0, &face))
		{
			dbglog("ERROR::FREETYPE: Failed to load font");
			dbgbreak("ERROR::FREETYPE: Failed to load font")
			return;
		}

		FT_Set_Pixel_Sizes(face, 0, FONT_SIZE);

		for (unsigned char c = START_CHARACTER; c < LAST_CHARCTER; c++)
		{
			if (!FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				dbglog("Failed to Load character:" << c);
				continue;
			}

			SheetWidth += face->glyph->face->glyph->bitmap.width;
			SheetHeight = (face->glyph->bitmap.rows > SheetHeight)
				? face->glyph->bitmap.rows
				: SheetHeight;
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, state.UITexture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
#if !defined __EMSCRIPTEN__ || !defined PLATFORM_WEB
			GL_RED,
#else
			GL_ALPHA,
#endif
			SheetWidth,
			SheetHeight,
			0,
#if !defined __EMSCRIPTEN__ || !defined PLATFORM_WEB
			GL_RED,
#else
			GL_ALPHA,
#endif
			GL_UNSIGNED_BYTE,
			new unsigned char[SheetWidth * SheetHeight]
		);


		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}

	void RenderText(const char* Text, v2f& startPos, float textScale)
	{

	}

	// State
	// 
	// Text Renderer \
	//				   -> turn into 1?
	// Quad Renderer /	(May need images)
	//
	// Scrollable/Expandable
	// 
	// DropDownArrow/Header:
	//		- Clickable
	//		- Lable
	//		- On/Off
	// 
	// Button:
	//		- Clickable
	//		- Says Text
	//		- Quad / Image
	// 
	// Checkbox:
	//		- Clickable
	//		- On / Off
	// 
	// Text
	// EditableText
	// 
	// Init()
	// Begin()
	// 
	// Text()
	// Button()
	// Checkbox()
	// 
	// End()

	State::~State()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}

}
