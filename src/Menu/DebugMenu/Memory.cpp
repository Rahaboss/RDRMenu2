#include "pch.h"
#include "Memory.h"
#include "Renderer/ImGuiExtras.h"
#include "Rage/natives.h"
#include "Rage/ScriptGlobal.h"
#include "Script/PlayerInfo.h"
#include "Script/Network.h"

static void RenderNativeDebug()
{
	static uint64_t s_NativeInput = 0xEC9A1261BF0CE510;
	ImGui::InputU64("Native Hash", &s_NativeInput, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);

	const uintptr_t NativeHandler = TO_IDA(NativeInvoker::GetHandler(s_NativeInput));
	ImGui::Text("IDA Address: 0x%llX", NativeHandler);
	ImGui::SameLine();
	if (ImGui::SmallButton("Copy##copy_ida_native"))
	{
		std::stringstream NativeAddress;
		NativeAddress << std::hex << std::uppercase << NativeHandler;
		ImGui::SetClipboardText(NativeAddress.str().c_str());
	}
}

static void RenderGlobalDebug()
{
	//Hash* Hair = ScriptGlobal(1946054).At(1497).At(1).At(1, 3).Get<Hash*>();
	Hash* Hair = ScriptGlobal(40).At(7748).At(2).Get<Hash*>();
	
	ImGui::SetNextItemWidth(200);
	ImGui::InputU32("Hair Type", Hair);
	
	Hash* Length = ScriptGlobal(40).At(7748).At(1).Get<Hash*>();

	ImGui::SetNextItemWidth(200);
	ImGui::InputU32("Hair Length", Length);

	ImGui::Separator();

	static std::vector<int> s_Offsets{ 1946054, 1497, 1, 1, 3 };

	std::string GlobalString = "Global";
	GlobalString += '_';
	GlobalString += std::to_string(s_Offsets[0]);

	for (size_t i = 1; i < s_Offsets.size(); i++)
	{
		GlobalString += ".f_";
		GlobalString += std::to_string(s_Offsets[i]);
	}

	int offset = 0;
	for (size_t i = 0; i < s_Offsets.size(); i++)
		offset += s_Offsets[i];
	
	if (void* mem = ScriptGlobal(offset).Get<void*>())
	{
		ImGui::TextUnformatted(GlobalString.c_str());

		const int int_value = *reinterpret_cast<int*>(mem);

		ImGui::Text("int: %d", int_value);
		ImGui::SameLine();
		if (ImGui::SmallButton((std::string{ "Copy##int_global" } + std::to_string(reinterpret_cast<uintptr_t>(mem))).c_str()))
			ImGui::SetClipboardText(std::to_string(int_value).c_str());

		const float float_value = *reinterpret_cast<float*>(mem);

		ImGui::Text("float: %f", float_value);
		ImGui::SameLine();
		if (ImGui::SmallButton((std::string{ "Copy##float_global" } + std::to_string(reinterpret_cast<uintptr_t>(mem))).c_str()))
			ImGui::SetClipboardText(std::to_string(float_value).c_str());
	}

	int i = 0;
	for (auto it = s_Offsets.begin(); it < s_Offsets.cend(); it++)
	{
		ImGui::SetNextItemWidth(300);
		ImGui::InputInt((std::string{ "##input_global" } + std::to_string(i)).c_str(), &(*it));
		if (s_Offsets.size() != 1)
		{
			ImGui::SameLine();
			if (ImGui::Button((std::string{ "Remove##global" } + std::to_string(i)).c_str()))
				it = s_Offsets.erase(it);
		}

		i++;
	}

	if (ImGui::Button("Add Offset"))
		s_Offsets.push_back(0);

	ImGui::Text("Pointers::ScriptGlobals: 0x%llX", Pointers::ScriptGlobals);
	ImGui::SameLine();
	if (ImGui::SmallButton("Copy##Pointers::ScriptGlobals"))
	{
		std::stringstream ss;
		ss << std::hex << std::uppercase << (uintptr_t)Pointers::ScriptGlobals;
		ImGui::SetClipboardText(ss.str().c_str());
	}
}

static void RenderNetworkDebug()
{
	if (const rage::netPlayerData* NetPlayerData = Script::GetNetPlayerData(g_LocalPlayer.m_Index))
	{
		ImGui::Text("Name: %s", NetPlayerData->m_Name);

		ImGui::Text("Rockstar ID: %llu", NetPlayerData->m_RockstarID);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##Rockstar ID"))
		{
			std::stringstream ss;
			ss << NetPlayerData->m_RockstarID;
			ImGui::SetClipboardText(ss.str().c_str());
		}

		ImGui::Text("Internal IP: %u.%u.%u.%u", NetPlayerData->m_InternalIP.m_Field1, NetPlayerData->m_InternalIP.m_Field2,
			NetPlayerData->m_InternalIP.m_Field3, NetPlayerData->m_InternalIP.m_Field4);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##Internal IP"))
			Script::CopyIP(NetPlayerData->m_InternalIP);

		ImGui::Text("External IP: %u.%u.%u.%u", NetPlayerData->m_ExternalIP.m_Field1, NetPlayerData->m_ExternalIP.m_Field2,
			NetPlayerData->m_ExternalIP.m_Field3, NetPlayerData->m_ExternalIP.m_Field4);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##External IP"))
			Script::CopyIP(NetPlayerData->m_ExternalIP);
	}
}

void DebugMenu::RenderMemoryTab()
{
	if (!ImGui::BeginTabItem("Memory##debug"))
		return;

	TRY
	{
		ImGui::SeparatorText("Native Debug");
		RenderNativeDebug();

		ImGui::SeparatorText("Global Debug");
		RenderGlobalDebug();

		ImGui::SeparatorText("Network Debug");
		RenderNetworkDebug();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndTabItem();
}
