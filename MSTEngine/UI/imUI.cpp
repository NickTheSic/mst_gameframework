#include "imUI.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "mstgl.h"
#include "mstglm.h"
#include "mstDebug.h"
#include "Renderer/QuadRenderer.h"
#include "Renderer/TextRenderer.h"
#include "Renderer/SpriteSheetRenderer.h"
#include "Core/Engine.h"
#include "Utils.h"

namespace ui
{	
	constexpr int MAX_VERTICES = 100;
	constexpr int START_CHARACTER = 32;
	constexpr int LAST_CHARCTER = 126;
	constexpr int CUSTOM_TEXTURES = 0;
	constexpr int TEXTURE_ARRAY_START_OFFSET = START_CHARACTER - CUSTOM_TEXTURES;
	constexpr int MAX_TEXUTURES = (LAST_CHARCTER - TEXTURE_ARRAY_START_OFFSET);
	constexpr const char* FONT_FILE_PATH = "Data/caviardreamsbold.ttf";
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

	struct TextRenderData
	{
		v2f Pos;
		Color Colour;
		std::string Text;
	};

	struct ButtonRenderData
	{
		v2f bl;
		v2f ur;

		Color col;
	};

	struct RenderState
	{
		mst::TextRenderer TextRenderer;
		mst::QuadRenderer QuadRenderer;
		//mst::SpriteSheetGeneratorRenderer SpriteSheetRenderer;

		std::vector<TextRenderData> AddedText;
		std::vector<ButtonRenderData> AddedButtons;
	};

	// Global State
	RenderState state;

	void InitUI()
	{
		state.TextRenderer.Init(100, FONT_FILE_PATH, 10);
		state.TextRenderer.UseProgram();
		state.TextRenderer.SetUniform("u_CameraPos", mst::Engine::Get()->GetMainCamera().Position);

		state.QuadRenderer.Init(100);
		state.QuadRenderer.UseProgram();
		state.QuadRenderer.SetUniform("u_CameraPos", mst::Engine::Get()->GetMainCamera().Position);
		state.QuadRenderer.SetUniform("u_WorldSize", mst::Engine::Get()->GetScreenSize());
	}

	void AddText(const char* Text, v2f&& Position, const Color& Colour)
	{
		AddText(Text, Position, Colour);
	}

	void AddText(const char* Text, v2f& Position, const Color& Colour)
	{
		TextRenderData trd;
		trd.Text = Text;
		trd.Pos = Position;
		trd.Colour = Colour;
		state.AddedText.push_back(std::move(trd));
	}

	bool AddButton(const char* Label, const v2f& Position, const Color& Colour)
	{
		bool IsClicked = false;

		const int StringLength = (int)strlen(Label);
		const float TextSize = 7 * (StringLength);

		ButtonRenderData brd;
		brd.bl = Position;
		brd.ur = v2i{TextSize, 16};

		v2f TextPos = Position + v2f{ 2,2 };
		AddText(Label, TextPos, {255,255,255});

		mst::Engine* Engine = mst::Engine::Get();

		if (Utils::IsPointInRect(Engine->GetMousePosition(), brd.bl, brd.ur))
		{ 
			if ((Colour.r+Colour.b+Colour.g) > 254)
				brd.col = Color{Colour.r*0.75, Colour.g*0.75, Colour.b*0.75};
			else
				brd.col = Color{ Colour.r + 75, Colour.g + 75, Colour.b + 75 };

			if (Engine->IsMouseButtonReleased(0))
			{
				IsClicked = true;
			}
		}
		else
		{
			brd.col = Colour;
		}

		state.AddedButtons.push_back(std::move(brd));

		return IsClicked;
	}

	void Present()
	{
		//Render quads first
		state.QuadRenderer.StartRender();
		for (auto& button : state.AddedButtons)
		{
			state.QuadRenderer.AddRect(button.bl, button.ur, button.col);
		}
		state.QuadRenderer.EndRender();

		//Render images on top of quads

		//Render text above all
		state.TextRenderer.StartRender();
		for (auto& Text : state.AddedText)
		{
			state.TextRenderer.RenderText(Text.Text, {Text.Pos.x, Text.Pos.y});
		}
		state.TextRenderer.EndRender();

		state.AddedButtons.clear();
		state.AddedText.clear();
	}
}
