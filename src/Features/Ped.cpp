#include "pch.h"
#include "Ped.h"
#include "JobQueue.h"
#include "Features.h"

namespace Features
{
	void CleanPed(Ped ped)
	{
		QUEUE_JOB(=)
		{
			PED::CLEAR_PED_BLOOD_DAMAGE(ped);
			PED::CLEAR_PED_WETNESS(ped);
			PED::CLEAR_PED_ENV_DIRT(ped);
			PED::CLEAR_PED_DECORATIONS(ped);
			PED::_SET_PED_DIRT_CLEANED(ped, 0.0f, -1, true, true);
			for (int i = 0; i < 10; i++)
			{
				PED::_CLEAR_PED_BLOOD_DAMAGE_FACIAL(ped, i);
				PED::CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(ped, i);
				PED::CLEAR_PED_DAMAGE_DECAL_BY_ZONE(ped, i, "ALL");
			}
		}
		END_JOB()
	}

	void PrintPedAttributes(Ped ped)
	{
		constexpr const char* AttributeNames[]{
			"PA_HEALTH", "PA_STAMINA", "PA_SPECIALABILITY", "PA_COURAGE", "PA_AGILITY", "PA_SPEED",
			"PA_ACCELERATION", "PA_BONDING", "SA_HUNGER", "SA_FATIGUED", "SA_INEBRIATED", "SA_POISONED",
			"SA_BODYHEAT", "SA_BODYWEIGHT", "SA_OVERFED", "SA_SICKNESS", "SA_DIRTINESS", "SA_DIRTINESSHAT",
			"MTR_STRENGTH", "MTR_GRIT", "MTR_INSTINCT", "PA_UNRULINESS", "SA_DIRTINESSSKIN",
		};
		static_assert(ARRAYSIZE(AttributeNames) == MAX_ATTRIBUTES);

		TRY
		{
			//LOG_TO_CONSOLE("Attr,Rank,BaseRank,BonusRank,MaxRank,DefaultRank,DefaultMaxRank,Points,MaxPoints\n");
			for (int i = 0; i < MAX_ATTRIBUTES; i++)
			{
				const char* Attr = AttributeNames[i];
				const int Rank = ATTRIBUTE::GET_ATTRIBUTE_RANK(ped, i);
				const int BaseRank = ATTRIBUTE::GET_ATTRIBUTE_BASE_RANK(ped, i);
				const int BonusRank = ATTRIBUTE::GET_ATTRIBUTE_BONUS_RANK(ped, i);
				const int MaxRank = ATTRIBUTE::GET_MAX_ATTRIBUTE_RANK(ped, i);
				const int DefaultRank = ATTRIBUTE::GET_DEFAULT_ATTRIBUTE_RANK(ped, i);
				const int DefaultMaxRank = ATTRIBUTE::GET_DEFAULT_MAX_ATTRIBUTE_RANK(ped, i);
				const int Points = ATTRIBUTE::GET_ATTRIBUTE_POINTS(ped, i);
				const int MaxPoints = ATTRIBUTE::GET_MAX_ATTRIBUTE_POINTS(ped, i);
				
				//LOG_TO_CONSOLE("%s,%d,%d,%d,%d,%d,%d,%d,%d\n", Attr, Rank, BaseRank, BonusRank, MaxRank, DefaultRank, DefaultMaxRank, Points, MaxPoints);
				LOG_TO_CONSOLE("%s (%d/%d)\n", Attr, Points, MaxPoints);
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
	
	void GiveGoldCores(Ped ped)
	{
		QUEUE_JOB(=)
		{
			for (int i = 0; i < 3; i++)
			{
				constexpr float Duration = 10000.0f; // seconds
				constexpr int CoreValue = 100; // 0 - 100
				constexpr BOOL Sound = false;

				ATTRIBUTE::DISABLE_ATTRIBUTE_OVERPOWER(ped, i);
				ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(ped, i, CoreValue);
				ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(ped, i, Duration, Sound);
				ATTRIBUTE::ENABLE_ATTRIBUTE_OVERPOWER(ped, i, Duration, Sound);
			}
		}
		END_JOB()
	}

	void SetMetapedOutfit(Ped ped, Hash outfit)
	{
		TRY
		{
			int requestId = PED::_REQUEST_META_PED_OUTFIT(ENTITY::GET_ENTITY_MODEL(ped), outfit);
			while (!PED::_HAS_META_PED_OUTFIT_LOADED(requestId))
				YieldThread();
			PED::_APPLY_PED_META_PED_OUTFIT(requestId, ped, true, false);
			PED::_RELEASE_META_PED_OUTFIT_REQUEST(requestId);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
	
	void RemoveAllPedWeapons(Ped ped)
	{
		QUEUE_JOB(=)
		{
			WEAPON::REMOVE_ALL_PED_WEAPONS(ped, true, true);
		}
		END_JOB()
	}
}
