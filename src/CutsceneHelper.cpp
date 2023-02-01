#include "pch.h"
#include "CutsceneHelper.h"
#include "Features.h"
#include "PlayerInfo.h"

CutsceneHelper::CutsceneHelper(const nlohmann::json& JsonObject):
	m_Scene(0),
	m_JsonObject(JsonObject)
{
	if (m_JsonObject["id"].is_null())
	{
		LOG_TXT("%s: JsonObject.id is null!", __FUNCTION__);
		return;
	}

	const char* animDict = m_JsonObject["id"].get_ref<const std::string&>().c_str();
	const char* playbackListName = "MultiStart";
	if (!m_JsonObject["playback_id"].is_null())
		playbackListName = m_JsonObject["playback_id"].get_ref<const std::string&>().c_str();
	
	m_Scene = ANIMSCENE::_CREATE_ANIM_SCENE(animDict, 0, playbackListName, false, true);
}

void CutsceneHelper::AddPedExisting(Ped Handle, const char* entityName)
{
	ANIMSCENE::SET_ANIM_SCENE_ENTITY(m_Scene, entityName, Handle, 0);
	m_Peds.push_back(Handle);
}

void CutsceneHelper::AddPedNew(Hash Model, const char* entityName)
{
	Ped Handle = Features::SpawnPed(Model);
	AddPedExisting(Handle, entityName);
}

void CutsceneHelper::AddPeds()
{
	AddLocalPlayer();

	if (m_JsonObject["peds"].is_null())
		return;

	for (const auto& j : m_JsonObject["peds"])
	{
		if (j["model"].is_null() || j["name"].is_null())
			continue;

		LOG_TXT("%s: Adding ped %s.", __FUNCTION__, j["model"].get_ref<const std::string&>().c_str());
		AddPedNew(joaat(j["model"].get_ref<const std::string&>()), j["name"].get_ref<const std::string&>().c_str());
	}
}

void CutsceneHelper::AddLocalPlayer()
{
	const bool b_playerArthur = g_LocalPlayer.m_Model == PLAYER_ZERO;
	ANIMSCENE::SET_ANIM_SCENE_BOOL(m_Scene, "b_playerArthur", b_playerArthur, false);
	AddPedExisting(g_LocalPlayer.m_Entity, (b_playerArthur ? "ARTHUR" : "JOHN"));
}

bool CutsceneHelper::IsCutsceneValid()
{
	if (m_Scene == 0 || (int)m_Scene == -1)
		return false;

	return true;
}

void CutsceneHelper::TeleportToOrigin()
{
	Vector3 position, rotation;
	ANIMSCENE::GET_ANIM_SCENE_ORIGIN(m_Scene, &position, &rotation, 2);
	LOG_TXT("%s: Teleporting to origin.", __FUNCTION__);
	Features::Teleport(position);
}

void CutsceneHelper::LoadCutscene()
{
	if (m_Loaded)
		return;

	LOG_TXT("%s: Loading cutscene.", __FUNCTION__);

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
	LOG_TXT("%s: Playing cutscene.", __FUNCTION__);

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
	LOG_TXT("%s: Cleaning up cutscene.", __FUNCTION__);

	for (const auto& p : m_Peds)
		Features::DeletePed(p);
	m_Peds.clear();

	ANIMSCENE::_DELETE_ANIM_SCENE(m_Scene);
}

void CutsceneHelper::PlayAutomatically()
{
	if (!IsCutsceneValid())
	{
		LOG_TXT("%s: Cutscene is invalid (%d)!", __FUNCTION__, (int)m_Scene);
		return;
	}

	TRY
	{
		LoadCutscene();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	TRY
	{
		TeleportToOrigin();
	}
	EXCEPT{ LOG_EXCEPTION(); }
		
	TRY
	{
		AddPeds();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	TRY
	{
		PlayCutscene();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	TRY
	{
		WaitForCutsceneEnd();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	TRY
	{
		CleanupCutscene();
	}
	EXCEPT{ LOG_EXCEPTION(); }
}
