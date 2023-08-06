#include "pch.h"
#include "CutsceneHelper.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "Lists.h"
#include "Menu.h"

// DON'T USE OR CALL FUNCTIONS THAT USE THE JOB QUEUE !!! (unless you know what you are doing)

CutsceneHelper::CutsceneHelper(const json& JsonObject):
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
	int flags = ASF_NONE /* | ASF_BLOCK_SKIPPING | ASF_ENABLE_LETTER_BOX */;
	if (m_JsonObject.contains("flags"))
		flags = m_JsonObject["flags"].get<uint32_t>();
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

void CutsceneHelper::AddPedFromPedJson(const json& PedJsonObject)
{
	Ped Handle = AddPedNew(Features::GetHashFromJson(PedJsonObject["model"]), PedJsonObject["name"].get_ref<const std::string&>().c_str());
	Features::YieldThread();

	if (PedJsonObject.contains("outfit_preset"))
		PED::_EQUIP_META_PED_OUTFIT_PRESET(Handle, PedJsonObject["outfit_preset"].get<int>(), false);

	// https://github.com/femga/rdr3_discoveries/blob/master/clothes/metaped_outfits.lua
	if (PedJsonObject.contains("meta_ped_outfit"))
		Features::SetMetaPedOutfit(Handle, Features::GetHashFromJson(PedJsonObject["meta_ped_outfit"]));

	if (PedJsonObject.contains("meta_ped_wearable"))
		Features::SetMetaPedOutfit(Handle, Features::GetHashFromJson(PedJsonObject["meta_ped_wearable"]));

	if (PedJsonObject.contains("remove_weapons") && PedJsonObject["remove_weapons"].get<bool>())
		WEAPON::REMOVE_ALL_PED_WEAPONS(Handle, true, true);
}

void CutsceneHelper::AddPeds()
{
	if (!m_JsonObject.contains("peds"))
		return;

	for (const auto& j : m_JsonObject["peds"])
	{
		// Don't add if invalid
		if (!j.contains("model") || !j.contains("name"))
			continue;

		// Don't add if already added
		if (ENTITY::DOES_ENTITY_EXIST(ANIMSCENE::_GET_ANIM_SCENE_PED(m_Scene, j["name"].get_ref<const std::string&>().c_str(), FALSE)))
			continue;

		AddPedFromPedJson(j);
	}
}

void CutsceneHelper::AddLocalPlayer()
{
	if (m_JsonObject.contains("player_model"))
	{
		// Known ped model (from JSON)
		const Hash PlayerModel = rage::joaat(m_JsonObject["player_model"].get_ref<const std::string&>());
		const bool b_PlayerArthur = PlayerModel == RAGE_JOAAT("player_zero");
		const char* entityName = (b_PlayerArthur ? "ARTHUR" : "JOHN");
		ANIMSCENE::SET_ANIM_SCENE_BOOL(m_Scene, "b_PlayerArthur", b_PlayerArthur, false);

		Ped Handle;
		if (g_LocalPlayer.m_Model == PlayerModel)
		{
			Handle = g_LocalPlayer.m_Entity;
			ANIMSCENE::SET_ANIM_SCENE_ENTITY(m_Scene, entityName, g_LocalPlayer.m_Entity, 0);
		}
		else
		{
			Handle = Features::SpawnPed(PlayerModel);
			AddPedExisting(Handle, entityName);
			// Apply default outfit
			if (!m_JsonObject.contains("player_outfit_preset") && !m_JsonObject.contains("player_meta_ped_outfit"))
				PED::_EQUIP_META_PED_OUTFIT_PRESET(Handle, (b_PlayerArthur ? 3 : 26), false);
		}

		if (m_JsonObject.contains("player_outfit_preset"))
			PED::_EQUIP_META_PED_OUTFIT_PRESET(Handle, m_JsonObject["player_outfit_preset"].get<int>(), false);
		if (m_JsonObject.contains("player_meta_ped_outfit"))
			Features::SetMetaPedOutfit(Handle, Features::GetHashFromJson(m_JsonObject["player_meta_ped_outfit"]));
		if (m_JsonObject.contains("player_meta_ped_wearable"))
			Features::SetMetaPedOutfit(Handle, Features::GetHashFromJson(m_JsonObject["player_meta_ped_wearable"]));
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
		// Don't add if invalid
		if (!j.contains("model") || !j.contains("name"))
			continue;

		AddObjectNew(Features::GetHashFromJson(j["model"]), j["name"].get_ref<const std::string&>().c_str());
	}
}

void CutsceneHelper::AddVehicleExisting(Vehicle Handle, const char* entityName)
{
	ANIMSCENE::SET_ANIM_SCENE_ENTITY(m_Scene, entityName, Handle, 0);
	m_Vehicles.push_back(Handle);
}

void CutsceneHelper::AddVehicleFromVehicleJson(const json& VehicleJsonObject)
{
	Vehicle Handle = Features::SpawnVehicle(Features::GetHashFromJson(VehicleJsonObject["model"]), false);
	
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
		// Don't add if invalid
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
	if (m_JsonObject.contains("disable_tp_to_origin") && m_JsonObject["disable_tp_to_origin"].get<bool>())
	{
		ANIMSCENE::SET_ANIM_SCENE_ORIGIN(m_Scene, g_LocalPlayer.m_Pos.x, g_LocalPlayer.m_Pos.y, g_LocalPlayer.m_Pos.z, 0, 0, 0, 0);
		return;
	}

	Vector3 position, rotation;
	ANIMSCENE::GET_ANIM_SCENE_ORIGIN(m_Scene, &position, &rotation, 2);
	Features::LoadGround(position);
	Features::Teleport(position);
}

void CutsceneHelper::LoadCutscene()
{
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
	ANIMSCENE::START_ANIM_SCENE(m_Scene);
}

void CutsceneHelper::SkipCutscene()
{
	if (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(m_Scene, false))
		ANIMSCENE::TRIGGER_ANIM_SCENE_SKIP(m_Scene);
}

void CutsceneHelper::WaitForCutsceneEnd()
{
	uint32_t Time = (uint32_t)(ANIMSCENE::_GET_ANIM_SCENE_DURATION(m_Scene) * 1000.0f);
	uint64_t StartTime = GetTickCount64();

	while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(m_Scene, false) && GetTickCount64() < (StartTime + Time))
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

	if (m_Scene)
		ANIMSCENE::_DELETE_ANIM_SCENE(m_Scene);
}

void CutsceneHelper::PlayAutomatically()
{
	if (!IsCutsceneValid())
		return;

	TRY
	{
		LoadCutscene();
		TeleportToOrigin();

		AddLocalPlayer();
		AddPeds();
		AddObjects();
		AddVehicles();
		
		Features::YieldThread(1000); // Try to load more of the map

		PlayCutscene();
		WaitForCutsceneEnd();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	TRY
	{
		CleanupCutscene();
	}
	EXCEPT{ LOG_EXCEPTION(); }
}
