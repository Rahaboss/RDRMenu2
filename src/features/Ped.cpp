#include "pch.h"
#include "Ped.h"
#include "JobQueue.h"

namespace Features
{
	void CleanPed(Ped ped)
	{
		QUEUE_JOB(ped)
		{
			PED::CLEAR_PED_BLOOD_DAMAGE(ped);
			PED::CLEAR_PED_WETNESS(ped);
			PED::CLEAR_PED_ENV_DIRT(ped);
			PED::CLEAR_PED_DECORATIONS(ped);
			PED::_SET_PED_DIRT_CLEANED(ped, 0.0f, -1, TRUE, TRUE);
			for (int i = 0; i < 10; i++)
			{
				PED::_CLEAR_PED_BLOOD_DAMAGE_FACIAL(ped, i);
				PED::CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(ped, i);
				PED::CLEAR_PED_DAMAGE_DECAL_BY_ZONE(ped, i, "ALL");
			}
		}
		END_JOB()
	}
}
