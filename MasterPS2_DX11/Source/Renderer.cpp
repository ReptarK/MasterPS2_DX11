#include "Renderer.h"

#include "ImGui/imgui_internal.h"
#include "ImGui/Rubik.h"

#include <sstream>

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Initialize()
{
	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 14.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	m_pFont = io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 32.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
}

void Renderer::BeginScene()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin("##Backbuffer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
}

void Renderer::EndScene()
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->PushClipRectFullScreen();

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
}

float Renderer::RenderText(const std::string& text, const ImVec2& position, float size, uint32_t color, bool center)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = m_pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());

			window->DrawList->AddText(m_pFont, size, { position.x - textSize.x / 2.0f, position.y + textSize.y * i }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), line.c_str());
		}
		else
		{
			window->DrawList->AddText(m_pFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());

			window->DrawList->AddText(m_pFont, size, { position.x, position.y + textSize.y * i }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), line.c_str());
		}

		y = position.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}

void Renderer::RenderLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddLine(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), thickness);
}

void Renderer::RenderCircle(const ImVec2& position, float radius, uint32_t color, float thickness, uint32_t segments)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddCircle(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), segments, thickness);
}

void Renderer::RenderCircleFilled(const ImVec2& position, float radius, uint32_t color, uint32_t segments)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddCircleFilled(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), segments);
}

void Renderer::RenderRect(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xFF;
	float r = (color >> 16) & 0xFF;
	float g = (color >> 8) & 0xFF;
	float b = (color) & 0xFF;

	window->DrawList->AddRect(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags, thickness);
}

void Renderer::RenderRectFilled(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xFF;
	float r = (color >> 16) & 0xFF;
	float g = (color >> 8) & 0xFF;
	float b = (color) & 0xFF;

	window->DrawList->AddRectFilled(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags);
}

void Renderer::RenderImage(ID3D11ShaderResourceView* pTexture, const ImVec2& from, const ImVec2& to, uint32_t color)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xFF;
	float r = (color >> 16) & 0xFF;
	float g = (color >> 8) & 0xFF;
	float b = (color) & 0xFF;

	window->DrawList->AddImage(pTexture, from, to, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }));
}

void Renderer::RenderImageRounded(ID3D11ShaderResourceView* pTexture, const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xFF;
	float r = (color >> 16) & 0xFF;
	float g = (color >> 8) & 0xFF;
	float b = (color) & 0xFF;

	window->DrawList->AddImageRounded(pTexture, from, to, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags);
}


//#include "stdafx.h"
//#include "Renderer.h"
//
//#include "ImGui/Rubik.h"
//
//#include "SDK\CGame.h"
//
//#include <sstream>
//
//ImFont* Renderer::m_defaultFont = nullptr;
//float Renderer::m_defaultFontSize = 16.f;
//
//int Renderer::m_width;
//int Renderer::m_height;
//
//void Renderer::Initialize()
//{
//	ImGuiIO& io = ImGui::GetIO();
//	//m_defaultFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 16.0f);
//	//m_defaultFont = io.Fonts->AddFontDefault();
//	m_defaultFont = io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
//
//	m_width = CGameInstance::Get()->m_graphics->m_width;
//	m_width = CGameInstance::Get()->m_graphics->m_height;
//}
//
//void Renderer::DrawLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness)
//{
//	ImGuiWindow* window = ImGui::GetCurrentWindow();
//
//	int a = (color >> 24) & 0xff; //#AARRGGBB
//	int r = (color >> 16) & 0xff;
//	int g = (color >> 8) & 0xff;
//	int b = (color) & 0xff;
//
//	window->DrawList->AddLine(from, to, ImGui::GetColorU32(ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f)), thickness);
//}
//
//void Renderer::DrawBox(const ImVec2& feet, const ImVec2& headPosition, uint32_t color)
//{
//	float height = headPosition.y - feet.y;
//	float width = height * 0.35;
//
//	//Draw box
//	DrawLine(ImVec2(feet.x - width, feet.y), ImVec2(feet.x + width, feet.y), 0x00000000, 2.0f);
//	DrawLine(ImVec2(feet.x - width, headPosition.y), ImVec2(feet.x - width, feet.y), 0x00000000, 2.0f);
//
//	DrawLine(ImVec2(feet.x + width, headPosition.y), ImVec2(feet.x + width, feet.y), 0x00000000, 2.0f);
//	DrawLine(ImVec2(feet.x - width, headPosition.y), ImVec2(feet.x + width, headPosition.y), 0x00000000, 2.0f);
//
//	//Draw box
//	DrawLine(ImVec2(feet.x - width, feet.y), ImVec2(feet.x + width, feet.y), color, 1.5f);
//	DrawLine(ImVec2(feet.x - width, headPosition.y), ImVec2(feet.x - width, feet.y), color, 1.5f);
//
//	DrawLine(ImVec2(feet.x + width, headPosition.y), ImVec2(feet.x + width, feet.y), color, 1.5f);
//	DrawLine(ImVec2(feet.x - width, headPosition.y), ImVec2(feet.x + width, headPosition.y), color, 1.5f);
//}
//
//void Renderer::DrawCircle(const ImVec2& position, float radius, uint32_t color, float thickness)
//{
//	ImGuiWindow* window = ImGui::GetCurrentWindow();
//
//	int a = (color >> 24) & 0xff; //#AARRGGBB
//	int r = (color >> 16) & 0xff;
//	int g = (color >> 8) & 0xff;
//	int b = (color) & 0xff;
//
//	window->DrawList->AddCircle(position, radius, ImGui::GetColorU32(ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f)), 24, thickness);
//}
//
//float Renderer::DrawMessage(ImFont* pFont, const std::string& text, const ImVec2& position, float size, uint32_t color, bool center)
//{
//	if (!pFont)
//		return 0;
//
//	ImGuiWindow* window = ImGui::GetCurrentWindow();
//
//	int a = (color >> 24) & 0xff; //#AARRGGBB
//	int r = (color >> 16) & 0xff;
//	int g = (color >> 8) & 0xff;
//	int b = (color) & 0xff;
//
//	std::stringstream steam(text);
//	std::string line;
//	float y = 0.0f;
//	int i = 0;
//
//	while (std::getline(steam, line))
//	{
//		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
//		if (center)
//		{
//			window->DrawList->AddText(
//				m_defaultFont,
//				m_defaultFontSize,
//				ImVec2(position.x - textSize.x / 2.0f, position.y + textSize.y * i),
//				ImGui::GetColorU32(ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f)),
//				line.c_str());
//		}
//		else
//		{
//			window->DrawList->AddText(
//				m_defaultFont,
//				m_defaultFontSize,
//				ImVec2(position.x, position.y + textSize.y * i),
//				ImGui::GetColorU32(ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f)),
//				line.c_str());
//		}
//
//		y = position.y + textSize.y * (i + 1);
//		i++;
//	}
//
//	return y;
//}
