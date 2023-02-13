#include "pch.h"
#include "CutsceneHelper.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "Lists.h"
#include "Menu.h"

CutsceneHelper::CutsceneHelper(const nlohmann::json& JsonObject):
	m_Scene(0),
	m_JsonObject(JsonObject)
{
	assert(m_JsonObject.contains("id") && "Invalid cutscene!");
	
	m_Scene = CreateCutscene();
}

CutsceneHelper::CutsceneHelper(const char* animDict):
	m_Scene(0)
{
	for (const auto& cs : g_Cutscenes)
	{
		if (cs["id"].get_ref<const std::string&>() != std::string(animDict))
			continue;
		
		m_JsonObject = cs;
		
		m_Scene = CreateCutscene();
		return;
	}

	// m_Scene will be 0 at this point
	assert(m_Scene && "Unknown cutscene!");
}

AnimScene CutsceneHelper::CreateCutscene()
{
	const char* animDict = m_JsonObject["id"].get_ref<const std::string&>().c_str();
	constexpr int flags = ASF_NONE /* | ASF_BLOCK_SKIPPING | ASF_ENABLE_LETTER_BOX */;
	const char* playbackListName = "MultiStart";
	if (m_JsonObject.contains("playback_id"))
		playbackListName = m_JsonObject["playback_id"].get_ref<const std::string&>().c_str();
	constexpr BOOL p3 = false;
	constexpr BOOL p4 = true;

	return ANIMSCENE::_CREATE_ANIM_SCENE(animDict, flags, playbackListName, p3, p4);
}

void CutsceneHelper::AddPedExisting(Ped Handle, const char* entityName)
{
	ANIMSCENE::SET_ANIM_SCENE_ENTITY(m_Scene, entityName, Handle, 0);
	m_Peds.push_back(Handle);
}

Ped CutsceneHelper::AddPedNew(Hash Model, const char* entityName)
{
	Ped Handle = Features::SpawnPed(Model);
	AddPedExisting(Handle, entityName);
	
	return Handle;
}

void CutsceneHelper::AddPedFromPedJson(const nlohmann::json& PedJsonObject)
{
	Ped Handle = AddPedNew(joaat(PedJsonObject["model"].get_ref<const std::string&>()),
		PedJsonObject["name"].get_ref<const std::string&>().c_str());

	if (PedJsonObject.contains("outfit_preset"))
		PED::_EQUIP_META_PED_OUTFIT_PRESET(Handle, PedJsonObject["outfit_preset"].get<int>(), false);

	if (PedJsonObject.contains("metaped_outfit"))
		Features::SetMetapedOutfit(Handle, PedJsonObject["metaped_outfit"].get<Hash>());
}

void CutsceneHelper::AddPeds()
{
	AddLocalPlayer();

	if (!m_JsonObject.contains("peds"))
		return;

	for (const auto& j : m_JsonObject["peds"])
	{
		if (!j.contains("model") || !j.contains("name"))
			continue;

		AddPedFromPedJson(j);
	}
}

void CutsceneHelper::AddLocalPlayer()
{
	if (m_JsonObject.contains("player_model"))
	{
		// Known ped model (from JSON)
		const auto& PlayerModel = m_JsonObject["player_model"].get_ref<const std::string&>();
		const bool b_PlayerArthur = PlayerModel == "player_zero";
		const char* entityName = (b_PlayerArthur ? "ARTHUR" : "JOHN");
		ANIMSCENE::SET_ANIM_SCENE_BOOL(m_Scene, "b_PlayerArthur", b_PlayerArthur, false);

		if (g_LocalPlayer.m_Model == joaat(PlayerModel))
		{
			if (m_JsonObject.contains("player_outfit_preset"))
				PED::_EQUIP_META_PED_OUTFIT_PRESET(g_LocalPlayer.m_Entity, m_JsonObject["player_outfit_preset"].get<int>(), false);
			ANIMSCENE::SET_ANIM_SCENE_ENTITY(m_Scene, entityName, g_LocalPlayer.m_Entity, 0);
		}
		else
		{
			Ped Handle = Features::SpawnPed(joaat(PlayerModel));
			if (m_JsonObject.contains("player_outfit_preset"))
				PED::_EQUIP_META_PED_OUTFIT_PRESET(Handle, m_JsonObject["player_outfit_preset"].get<int>(), false);
			else
				PED::_EQUIP_META_PED_OUTFIT_PRESET(Handle, (b_PlayerArthur ? 3 : 26), false);
			AddPedExisting(Handle, entityName);
		}
	}
	else
	{
		// Unknown ped model (use current)
		const bool b_PlayerArthur = !Features::IsJohnModel(); // YSC script style check
		ANIMSCENE::SET_ANIM_SCENE_BOOL(m_Scene, "b_PlayerArthur", b_PlayerArthur, false);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(m_Scene, (b_PlayerArthur ? "ARTHUR" : "JOHN"), g_LocalPlayer.m_Entity, 0);
	}
}

void CutsceneHelper::AddObjectExisting(Object Handle, const char* entityName)
{
	ANIMSCENE::SET_ANIM_SCENE_ENTITY(m_Scene, entityName, Handle, 0);
	m_Objects.push_back(Handle);
}

void CutsceneHelper::AddObjectNew(Hash Model, const char* entityName)
{
	Object Handle = Features::SpawnObject(Model);
	AddObjectExisting(Handle, entityName);
}

void CutsceneHelper::AddObjects()
{
	if (!m_JsonObject.contains("objects"))
		return;

	for (const auto& j : m_JsonObject["objects"])
	{
		if (!j.contains("model") || !j.contains("name"))
			continue;

		AddObjectNew(joaat(j["model"].get_ref<const std::string&>()), j["name"].get_ref<const std::string&>().c_str());
	}
}

void CutsceneHelper::AddVehicleExisting(Vehicle Handle, const char* entityName)
{
	ANIMSCENE::SET_ANIM_SCENE_ENTITY(m_Scene, entityName, Handle, 0);
	m_Vehicles.push_back(Handle);
}

void CutsceneHelper::AddVehicleFromVehicleJson(const nlohmann::json& VehicleJsonObject)
{
	Vehicle Handle = Features::SpawnVehicle(joaat(VehicleJsonObject["model"].get_ref<const std::string&>()), false);
	
	if (VehicleJsonObject.contains("extras"))
	{
		for (const auto& e : VehicleJsonObject["extras"])
			VEHICLE::SET_VEHICLE_EXTRA(Handle, e.get<int>(), false);
	}
	
	AddVehicleExisting(Handle, VehicleJsonObject["name"].get_ref<const std::string&>().c_str());
}

void CutsceneHelper::AddVehicleNew(Hash Model, const char* entityName)
{
	Vehicle Handle = Features::SpawnVehicle(Model, false);
	AddVehicleExisting(Handle, entityName);
}

void CutsceneHelper::AddVehicles()
{
	if (!m_JsonObject.contains("vehicles"))
		return;

	for (const auto& j : m_JsonObject["vehicles"])
	{
		if (!j.contains("model") || !j.contains("name"))
			continue;

		AddVehicleFromVehicleJson(j);
	}
}

bool CutsceneHelper::IsCutsceneValid() const
{
	if (m_Scene == 0 || (int)m_Scene == -1)
		return false;

	return true;
}

void CutsceneHelper::TeleportToOrigin()
{
	Vector3 position, rotation;
	ANIMSCENE::GET_ANIM_SCENE_ORIGIN(m_Scene, &position, &rotation, 2);
	Features::Teleport(position);
	Features::YieldThread(1000); // Try to load more of the map
}

void CutsceneHelper::LoadCutscene()
{
	if (m_Loaded)
		return;

	while (true)
	{
		ANIMSCENE::LOAD_ANIM_SCENE(m_Scene);
		if (ANIMSCENE::IS_ANIM_SCENE_LOADED(m_Scene, true, false))
			return;

		Features::YieldThread();
	}
}

void CutsceneHelper::PlayCutscene()
{
	LoadCutscene();
	ANIMSCENE::START_ANIM_SCENE(m_Scene);
}

void CutsceneHelper::SkipCutscene()
{
	if (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(m_Scene, false))
		ANIMSCENE::TRIGGER_ANIM_SCENE_SKIP(m_Scene);
}

void CutsceneHelper::WaitForCutsceneEnd()
{
	while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(m_Scene, false))
		Features::YieldThread();
}

void CutsceneHelper::CleanupCutscene()
{
	for (const auto& o : m_Objects)
		Features::DeleteObject(o);
	m_Objects.clear();

	for (const auto& p : m_Peds)
		Features::DeletePed(p);
	m_Peds.clear();

	for (const auto& v : m_Vehicles)
		Features::DeleteVehicle(v);
	m_Vehicles.clear();

	ANIMSCENE::_DELETE_ANIM_SCENE(m_Scene);
}

void CutsceneHelper::PlayAutomatically()
{
	if (!IsCutsceneValid())
		return;

	TRY
	{
		AddObjects();
		Features::YieldThread();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	TRY
	{
		AddPeds();
		Features::YieldThread();
	}
	EXCEPT{ LOG_EXCEPTION(); }
	
	TRY
	{
		AddVehicles();
		Features::YieldThread();
	}
	EXCEPT{ LOG_EXCEPTION(); }
	
	TRY
	{
		LoadCutscene();
		Features::YieldThread();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	TRY
	{
		TeleportToOrigin();
		Features::YieldThread();
	}
	EXCEPT{ LOG_EXCEPTION(); }
		
	TRY
	{
		PlayCutscene();
		Features::YieldThread();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	TRY
	{
		WaitForCutsceneEnd();
		Features::YieldThread();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	TRY
	{
		CleanupCutscene();
		Features::YieldThread();
	}
	EXCEPT{ LOG_EXCEPTION(); }
}
