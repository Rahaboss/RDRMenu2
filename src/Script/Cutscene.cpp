#include "pch.h"
#include "Cutscene.h"
#include "Config/Lists.h"
#include "Rage/enums.h"
#include "Rage/natives.h"
#include "Script/Spawning.h"
#include "Thread/Thread.h"
#include "Script/Entity.h"
#include "Script/Ped.h"
#include "Script/PlayerInfo.h"
#include "Script/World.h"
#include "Script/Player.h"
#include "Config/Settings.h"
#include "Util/String.h"

Script::CutsceneHelper::CutsceneHelper(const json& JsonObject):
	m_Scene(0),
	m_JsonObject(JsonObject)
{
	assert(m_JsonObject.contains("id"));
	
	m_Scene = CreateCutscene();
}

Script::CutsceneHelper::CutsceneHelper(const char* animDict):
	m_Scene(0)
{
	const auto it = Lists::GetCutscene(animDict);
	assert(it != Lists::CutsceneList.end());
	m_JsonObject = *it;
	m_Scene = CreateCutscene();
}

AnimScene Script::CutsceneHelper::CreateCutscene()
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

void Script::CutsceneHelper::AddPedExisting(Ped Handle, const char* entityName)
{
	ANIMSCENE::SET_ANIM_SCENE_ENTITY(m_Scene, entityName, Handle, 0);
	m_Peds.push_back(Handle);
}

Ped Script::CutsceneHelper::AddPedNew(Hash Model, const char* entityName)
{
	Ped Handle = Script::SpawnPed(Model);
	AddPedExisting(Handle, entityName);
	
	return Handle;
}

void Script::CutsceneHelper::AddPedFromPedJson(const json& PedJsonObject)
{
	Ped Handle = AddPedNew(Lists::GetHashFromJSON(PedJsonObject["model"]), PedJsonObject["name"].get_ref<const std::string&>().c_str());
	if (!Handle)
		return;

	if (PedJsonObject.contains("outfit_preset"))
		Script::SetPedOutfitPreset(Handle, PedJsonObject["outfit_preset"].get<int>());

	// https://github.com/femga/rdr3_discoveries/blob/master/clothes/metaped_outfits.lua
	if (PedJsonObject.contains("meta_ped_outfit"))
		Script::SetMetaPedOutfit(Handle, Lists::GetHashFromJSON(PedJsonObject["meta_ped_outfit"]));

	if (PedJsonObject.contains("meta_ped_wearable"))
		Script::SetMetaPedOutfit(Handle, Lists::GetHashFromJSON(PedJsonObject["meta_ped_wearable"]));

	if (PedJsonObject.contains("remove_weapons") && PedJsonObject["remove_weapons"].get<bool>())
	{
		WEAPON::_SET_PED_WEAPON_ATTACH_POINT_VISIBILITY(Handle, WEAPON_ATTACH_POINT_RIFLE, false);
		WEAPON::_SET_PED_WEAPON_ATTACH_POINT_VISIBILITY(Handle, WEAPON_ATTACH_POINT_RIFLE_ALTERNATE, false);
	}
}

void Script::CutsceneHelper::AddPeds()
{
	if (!m_JsonObject.contains("peds"))
		return;

	for (const auto& j : m_JsonObject["peds"])
	{
		// Don't add if invalid
		if (!j.contains("model") || !j.contains("name"))
			continue;

		AddPedFromPedJson(j);
	}
}

void Script::CutsceneHelper::AddLocalPlayer()
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
			Handle = AddPedNew(PlayerModel, entityName);
		
			// Apply default outfit
			if (!m_JsonObject.contains("player_outfit_preset") && !m_JsonObject.contains("player_meta_ped_outfit"))
				Script::SetMetaPedOutfit(Handle, (b_PlayerArthur ? RAGE_JOAAT("META_OUTFIT_COOL_WEATHER") : RAGE_JOAAT("META_OUTFIT_GUNSLINGER")));
		}

		if (m_JsonObject.contains("player_outfit_preset"))
			Script::SetMetaPedOutfit(Handle, m_JsonObject["player_outfit_preset"].get<int>());
		if (m_JsonObject.contains("player_meta_ped_outfit"))
			Script::SetMetaPedOutfit(Handle, Lists::GetHashFromJSON(m_JsonObject["player_meta_ped_outfit"]));
		if (m_JsonObject.contains("player_meta_ped_wearable"))
			Script::SetMetaPedOutfit(Handle, Lists::GetHashFromJSON(m_JsonObject["player_meta_ped_wearable"]));
	}
	else
	{
		// Unknown ped model (use current)
		const bool b_PlayerArthur = g_LocalPlayer.m_Model != RAGE_JOAAT("PLAYER_THREE"); // YSC script style check
		ANIMSCENE::SET_ANIM_SCENE_BOOL(m_Scene, "b_PlayerArthur", b_PlayerArthur, false);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(m_Scene, (b_PlayerArthur ? "ARTHUR" : "JOHN"), g_LocalPlayer.m_Entity, 0);
	}
}

void Script::CutsceneHelper::AddObjectExisting(Object Handle, const char* entityName)
{
	ANIMSCENE::SET_ANIM_SCENE_ENTITY(m_Scene, entityName, Handle, 0);
	m_Objects.push_back(Handle);
}

void Script::CutsceneHelper::AddObjectNew(Hash Model, const char* entityName)
{
	Object Handle = Script::SpawnObject(Model);
	AddObjectExisting(Handle, entityName);
}

void Script::CutsceneHelper::AddObjects()
{
	if (!m_JsonObject.contains("objects"))
		return;

	for (const auto& j : m_JsonObject["objects"])
	{
		// Don't add if invalid
		if (!j.contains("model") || !j.contains("name"))
			continue;

		AddObjectNew(Lists::GetHashFromJSON(j["model"]), j["name"].get_ref<const std::string&>().c_str());
	}
}

void Script::CutsceneHelper::AddVehicleExisting(Vehicle Handle, const char* entityName)
{
	ANIMSCENE::SET_ANIM_SCENE_ENTITY(m_Scene, entityName, Handle, 0);
	m_Vehicles.push_back(Handle);
}

void Script::CutsceneHelper::AddVehicleFromVehicleJson(const json& VehicleJsonObject)
{
	Vehicle Handle = Script::SpawnVehicle(Lists::GetHashFromJSON(VehicleJsonObject["model"]), false);
	
	if (VehicleJsonObject.contains("extras"))
	{
		for (const auto& e : VehicleJsonObject["extras"])
			VEHICLE::SET_VEHICLE_EXTRA(Handle, e.get<int>(), false);
	}
	
	AddVehicleExisting(Handle, VehicleJsonObject["name"].get_ref<const std::string&>().c_str());
}

void Script::CutsceneHelper::AddVehicleNew(Hash Model, const char* entityName)
{
	Vehicle Handle = Script::SpawnVehicle(Model, false);
	AddVehicleExisting(Handle, entityName);
}

void Script::CutsceneHelper::AddVehicles()
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

bool Script::CutsceneHelper::IsCutsceneValid() const
{
	if (m_Scene == 0 || (int)m_Scene == -1)
		return false;

	return true;
}

void Script::CutsceneHelper::TeleportToOrigin()
{
	if (m_JsonObject.contains("disable_tp_to_origin") && m_JsonObject["disable_tp_to_origin"].get<bool>())
	{
		ANIMSCENE::SET_ANIM_SCENE_ORIGIN(m_Scene, g_LocalPlayer.m_Pos.x, g_LocalPlayer.m_Pos.y, g_LocalPlayer.m_Pos.z, 0, 0, 0, 0);
		return;
	}

	Vector3 Position, Rotation;
	ANIMSCENE::GET_ANIM_SCENE_ORIGIN(m_Scene, &Position, &Rotation, 2);
	Script::LoadGround(Position);
	Script::Teleport(Position);

	if (PATHFIND::GET_NTH_CLOSEST_VEHICLE_NODE(Position.x, Position.y, Position.z, 1, &Position, 1, 3.0f, 0))
		Script::TeleportOnGround(Position);
}

void Script::CutsceneHelper::LoadCutscene()
{
	while (true)
	{
		ANIMSCENE::LOAD_ANIM_SCENE(m_Scene);
		if (ANIMSCENE::IS_ANIM_SCENE_LOADED(m_Scene, true, false))
			return;

		Thread::YieldThread();
	}
}

void Script::CutsceneHelper::PlayCutscene()
{
	ANIMSCENE::START_ANIM_SCENE(m_Scene);
}

void Script::CutsceneHelper::SkipCutscene()
{
	if (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(m_Scene, false))
		ANIMSCENE::TRIGGER_ANIM_SCENE_SKIP(m_Scene);
}

void Script::CutsceneHelper::WaitForCutsceneEnd()
{
	uint32_t Time = (uint32_t)(ANIMSCENE::_GET_ANIM_SCENE_DURATION(m_Scene) * 1000.0f);
	uint64_t StartTime = GetTickCount64();

	while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(m_Scene, false) && GetTickCount64() < (StartTime + Time))
		Thread::YieldThread();
}

void Script::CutsceneHelper::CleanupCutscene()
{
	for (const auto& o : m_Objects)
		Script::DeleteEntity(o);
	m_Objects.clear();

	for (const auto& p : m_Peds)
		Script::DeleteEntity(p);
	m_Peds.clear();

	for (const auto& v : m_Vehicles)
		Script::DeleteEntity(v);
	m_Vehicles.clear();

	if (m_Scene)
		ANIMSCENE::_DELETE_ANIM_SCENE(m_Scene);
}

void Script::CutsceneHelper::PlayAutomatically()
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
		
		Thread::YieldThread(1s); // Try to load more of the map
		
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

AnimScene Script::CutsceneHelper::GetCutscene() const
{
	return m_Scene;
}

void Script::AddEntityToCutscene(const char* CutsceneName, Entity ent, const char* EntityName)
{
	if (!g_Settings["add_cutscene_info_automatically"].get<bool>())
		return;

	if (!ENTITY::DOES_ENTITY_EXIST(ent))
		return;

	const Hash Model = ENTITY::GET_ENTITY_MODEL(ent);
	if (Model == RAGE_JOAAT("PLAYER_ZERO") || Model == RAGE_JOAAT("PLAYER_THREE"))
		return;

	std::string ModelName = Lists::GetHashName(Model);

	if (!Util::IsStringValid(ModelName))
		return;

	Util::StringToLower(ModelName);

	std::string CutsceneNameLower = Util::StringToLowerCopy(CutsceneName);
	const auto it = Lists::GetCutscene(CutsceneNameLower);
	if (it == Lists::CutsceneList.end())
		return;

	auto& Cutscene = *it;

	json j;
	j["name"] = EntityName;
	j["model"] = ModelName;

	std::string EntityNameLower = Util::StringToLowerCopy(EntityName);
	if (ENTITY::IS_ENTITY_A_PED(ent))
	{
		for (const json& ped : Cutscene["peds"])
		{
			if (Util::StringToLowerCopy(ped["name"].get_ref<const std::string&>()) == EntityNameLower)
				return;
		}

		if (Util::StringContains(ModelName, "horse") || Util::StringContains(EntityNameLower, "horse"))
			j["meta_ped_outfit"] = "meta_horse_saddle_only";

		Cutscene["peds"].push_back(j);
	}
	else if (ENTITY::IS_ENTITY_AN_OBJECT(ent))
	{
		for (const json& obj : Cutscene["objects"])
		{
			if (Util::StringToLowerCopy(obj["name"].get_ref<const std::string&>()) == EntityNameLower)
				return;
		}

		Cutscene["objects"].push_back(j);
	}
	else if (ENTITY::IS_ENTITY_A_VEHICLE(ent))
	{
		for (const json& veh : Cutscene["vehicles"])
		{
			if (Util::StringToLowerCopy(veh["name"].get_ref<const std::string&>()) == EntityNameLower)
				return;
		}

		std::vector<int> Extras;

		for (int i = 1; i <= 16; i++)
		{
			if (VEHICLE::DOES_EXTRA_EXIST(ent, i) && VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(ent, i))
				Extras.push_back(i);
		}

		if (!Extras.empty())
			j["extras"] = Extras;

		Cutscene["vehicles"].push_back(j);
	}
	else
	{
		if (g_Settings["log_animscene"].get<bool>())
			LOG_TEXT("Added unknown type of entity \"%s\" (model: %u) to AnimScene \"%s\".", EntityName, Model, CutsceneName);
		return;
	}

	if (g_Settings["log_animscene"].get<bool>())
		LOG_TEXT("Added entity %s (\"%s\") to AnimScene \"%s\" in database.", Lists::GetHashName(Model).c_str(), EntityName, CutsceneName);
}

void Script::AddEntityPlaybackID(const char* CutsceneName, const char* PlaybackID)
{
	if (!Util::IsStringValid(CutsceneName) || !Util::IsStringValid(PlaybackID))
		return;

	const std::string PlaybackIDLower = Util::StringToLowerCopy(PlaybackID);
	if (PlaybackIDLower == "normalstart" || PlaybackIDLower == "multistart")
		return;
	
	const std::string CutsceneNameLower = Util::StringToLowerCopy(CutsceneName);
	const auto it = Lists::GetCutscene(CutsceneNameLower);
	if (it == Lists::CutsceneList.end())
		return;

	json& Cutscene = *it;
	if (Cutscene.contains("playback_id"))
		return;

	Cutscene["playback_id"] = PlaybackID;
	
	if (g_Settings["log_animscene"].get<bool>())
		LOG_TEXT("Added Playback ID \"%s\" to AnimScene \"%s\" in database.", PlaybackID, CutsceneName);
}
