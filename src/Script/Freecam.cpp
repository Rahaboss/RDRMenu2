#include "pch.h"
#include "Freecam.h"
#include "Rage/natives.h"
#include "Rage/enums.h"
#include "PlayerInfo.h"
#include "Math.h"
#include "Util/Input.h"
#include "Player.h"
#include "Menu/Menu.h"

static Cam CamEntity = 0;
static Vector3 s_Position{};
static Vector3 s_Rotation{};
void Script::CreateFreecam()
{
	if (CamEntity)
		return;

	CamEntity = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 0);

	s_Position = CAM::GET_GAMEPLAY_CAM_COORD();
	s_Rotation = CAM::GET_GAMEPLAY_CAM_ROT(2);

	CAM::SET_CAM_COORD(CamEntity, s_Position.x, s_Position.y, s_Position.z);
	CAM::SET_CAM_ROT(CamEntity, s_Rotation.x, s_Rotation.y, s_Rotation.z, 2);
	CAM::SET_CAM_ACTIVE(CamEntity, true);
	CAM::RENDER_SCRIPT_CAMS(true, true, 500, true, true, 0);
}

static void UpdateFreecamPosition()
{
	if (Menu::IsOpen)
		return;

	Vector3 PosChange{};
	static float Speed = 0.5f;
	static float Accel = 0.0f;

	if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_JUMP)) { PosChange.z += Speed / 2; } // Left Shift
	if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_SPRINT)) { PosChange.z -= Speed / 2; } // Left Control
	if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_MOVE_UP_ONLY)) { PosChange.y += Speed; } // Forward
	if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_MOVE_DOWN_ONLY)) { PosChange.y -= Speed; } // Backward
	if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_MOVE_LEFT_ONLY)) { PosChange.x -= Speed; } // Left
	if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_MOVE_RIGHT_ONLY)) { PosChange.x += Speed; } // Right

	if (PosChange.x == 0.0f && PosChange.y == 0.0f && PosChange.z == 0.0f) { Accel = 0.0f; }
	else if (Accel < 10) { Accel += 0.15f; }

	Vector3 Rot = CAM::GET_CAM_ROT(CamEntity, 2);
	float Yaw = Script::DegreeToRadian(Rot.z);

	s_Position.x += (PosChange.x * cos(Yaw) - PosChange.y * sin(Yaw)) * Accel;
	s_Position.y += (PosChange.x * sin(Yaw) + PosChange.y * cos(Yaw)) * Accel;
	s_Position.z += PosChange.z * Accel;

	CAM::SET_CAM_COORD(CamEntity, s_Position.x, s_Position.y, s_Position.z);
	STREAMING::SET_FOCUS_POS_AND_VEL(s_Position.x, s_Position.y, s_Position.z, 0.0f, 0.0f, 0.0f);
}

void Script::TickFreecam()
{
	CreateFreecam();
	UpdateFreecamPosition();

	s_Rotation = CAM::GET_GAMEPLAY_CAM_ROT(2);
	CAM::SET_CAM_ROT(CamEntity, s_Rotation.x, s_Rotation.y, s_Rotation.z, 2);

	TASK::CLEAR_PED_TASKS(g_LocalPlayer.m_Entity, false, true);
	TASK::CLEAR_PED_SECONDARY_TASK(g_LocalPlayer.m_Entity);
	TASK::CLEAR_PED_TASKS_IMMEDIATELY(g_LocalPlayer.m_Entity, true, true);
	ENTITY::FREEZE_ENTITY_POSITION(g_LocalPlayer.m_Entity, true);

	if (!Menu::IsOpen)
	{
		static Hash Controls[]{
			INPUT_LOOK_LR,
			INPUT_LOOK_UD,
			INPUT_LOOK_UP_ONLY,
			INPUT_LOOK_DOWN_ONLY,
			INPUT_LOOK_LEFT_ONLY,
			INPUT_LOOK_RIGHT_ONLY,
		};

		PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
		for (Hash c : Controls)
		{
			PAD::ENABLE_CONTROL_ACTION(0, c, true);
			//PAD::ENABLE_CONTROL_ACTION(1, c, true);
		}

		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_ACCEPT))
			Teleport(s_Position);
	}
}

void Script::DestroyFreecam()
{
	if (!CamEntity)
		return;

	CAM::SET_CAM_ACTIVE(CamEntity, false);
	CAM::RENDER_SCRIPT_CAMS(false, true, 500, true, true, 0);
	CAM::DESTROY_CAM(CamEntity, false);
	STREAMING::SET_FOCUS_ENTITY(g_LocalPlayer.m_Entity);

	ENTITY::FREEZE_ENTITY_POSITION(g_LocalPlayer.m_Entity, false);

	CamEntity = 0;
}
