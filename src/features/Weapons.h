#pragma once

#include "pch.h"
#include "rage/natives.h"
#include "rage/enums.h"

namespace Features
{
	void DropCurrentWeapon();
	void GiveAmmo(const Hash& ammo_hash);
	void GiveAllAmmo();
	void GiveAllDualWieldWeapons();
	void GiveAllWeapons();
	void GiveBackWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveLeftHandWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveRightHandWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveShoulderWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveWeapon(const Hash& weapon_hash);
}

// According to ingame compendium
// Commented ones are technically ammo not weapons
// No Dualwield means you can't have 2 of them (pistols/revolvers)
constexpr inline std::array g_WeaponList{
	WEAPON_REVOLVER_CATTLEMAN, // Cattleman Revolver
	WEAPON_REVOLVER_CATTLEMAN_PIG, // Granger's Revolver --- No Dualwield
	WEAPON_REVOLVER_CATTLEMAN_MEXICAN, // Flaco's Revolver --- No Dualwield
	WEAPON_REVOLVER_CATTLEMAN_JOHN, // John's Cattleman Revolver
	WEAPON_REVOLVER_DOUBLEACTION, // Double-Action Revolver

	WEAPON_REVOLVER_DOUBLEACTION_EXOTIC, // Algernon's Revolver
	WEAPON_REVOLVER_DOUBLEACTION_MICAH, // Micah's Revolver --- No Dualwield
	WEAPON_REVOLVER_DOUBLEACTION_GAMBLER, // High Roller Revolver --- No Dualwield
	WEAPON_REVOLVER_SCHOFIELD, // Schofield Revolver
	WEAPON_REVOLVER_SCHOFIELD_CALLOWAY, // Calloway's Revolver --- No Dualwield

	WEAPON_REVOLVER_SCHOFIELD_GOLDEN, // Otis Miller's Revolver --- No Dualwield
	WEAPON_REVOLVER_LEMAT, // LeMat Revolver
	WEAPON_PISTOL_VOLCANIC, // Volcanic Pistol
	WEAPON_PISTOL_SEMIAUTO, // Semi-Automatic Pistol
	WEAPON_PISTOL_MAUSER, // Mauser Pistol

	WEAPON_PISTOL_MAUSER_DRUNK, // Midnight's Pistol
	WEAPON_PISTOL_M1899, // M1899 Pistol
	WEAPON_REPEATER_CARBINE, // Carbine Repeater
	WEAPON_REPEATER_WINCHESTER, // Lancaster Repeater
	WEAPON_REPEATER_HENRY, // Litchfield Repeater

	WEAPON_REPEATER_EVANS, // Evans Repeater
	WEAPON_RIFLE_VARMINT, // Varmint Rifle
	WEAPON_RIFLE_SPRINGFIELD, // Springfield Rifle
	WEAPON_RIFLE_BOLTACTION, // Bolt Action Rifle
	WEAPON_SNIPERRIFLE_ROLLINGBLOCK, // Rolling Block Rifle

	WEAPON_SNIPERRIFLE_ROLLINGBLOCK_EXOTIC, // Rare Rolling Block Rifle
	WEAPON_SNIPERRIFLE_CARCANO, // Carcano Rifle
	WEAPON_SHOTGUN_SAWEDOFF, // Sawed-Off Shotgun
	WEAPON_SHOTGUN_DOUBLEBARREL, // Double-Barreled Shotgun
	WEAPON_SHOTGUN_DOUBLEBARREL_EXOTIC, // Rare Shotgun

	WEAPON_SHOTGUN_PUMP, // Pump-Action Shotgun
	WEAPON_SHOTGUN_SEMIAUTO, // Semi-Auto Shotgun
	WEAPON_SHOTGUN_REPEATING, // Repeating Shotgun
	WEAPON_BOW, // Bow
	WEAPON_UNARMED, // Unarmed

	WEAPON_THROWN_MOLOTOV, // Fire Bottle
	//AMMO_MOLOTOV_VOLATILE, // Fire Bottle - Volatile
	WEAPON_THROWN_DYNAMITE, // Dynamite
	//AMMO_DYNAMITE_VOLATILE, // Dynamite - Volatile
	WEAPON_THROWN_THROWING_KNIVES, // Throwing Knife

	//AMMO_THROWING_KNIVES_IMPROVED, // Improved Throwing Knife
	//AMMO_THROWING_KNIVES_POISON, // Poison Throwing Knife
	WEAPON_THROWN_TOMAHAWK, // Tomahawk
	//AMMO_TOMAHAWK_IMPROVED, // Tomahawk - Improved
	//AMMO_TOMAHAWK_HOMING, // Tomahawk - Homing

	WEAPON_THROWN_TOMAHAWK_ANCIENT, // Ancient Tomahawk
	WEAPON_MELEE_CLEAVER, // Cleaver
	WEAPON_MELEE_MACHETE, // Machete
	WEAPON_MELEE_BROKEN_SWORD, // Broken Pirate Sword
	WEAPON_MELEE_KNIFE, // Hunting Knife

	WEAPON_MELEE_KNIFE_JOHN, // John's Knife
	WEAPON_MELEE_KNIFE_BEAR, // Antler Knife
	WEAPON_MELEE_KNIFE_CIVIL_WAR, // Civil War Knife
	WEAPON_MELEE_KNIFE_JAWBONE, // Jawbone Knife
	WEAPON_MELEE_KNIFE_MINER, // Wide-Blade Knife

	WEAPON_MELEE_KNIFE_VAMPIRE, // Ornate Dagger
	WEAPON_MELEE_HATCHET, // Hatchet
	WEAPON_MELEE_HATCHET_HEWING, // Hewing Hatchet
	WEAPON_MELEE_ANCIENT_HATCHET, // Stone Hatchet
	WEAPON_MELEE_HATCHET_VIKING, // Viking Hatchet

	WEAPON_MELEE_HATCHET_HUNTER, // Hunter Hatchet
	WEAPON_MELEE_HATCHET_HUNTER_RUSTED, // Rusted Hunter Hatchet
	WEAPON_MELEE_HATCHET_DOUBLE_BIT, // Double Bit Hatchet
	WEAPON_MELEE_HATCHET_DOUBLE_BIT_RUSTED, // Rusted Double Bit Hatchet
};

constexpr inline std::array g_DualWieldWeaponList{
	WEAPON_REVOLVER_CATTLEMAN, // Cattleman Revolver
	WEAPON_REVOLVER_CATTLEMAN_JOHN, // John's Cattleman Revolver
	WEAPON_REVOLVER_DOUBLEACTION, // Double-Action Revolver
	WEAPON_REVOLVER_DOUBLEACTION_EXOTIC, // Algernon's Revolver
	WEAPON_REVOLVER_SCHOFIELD, // Schofield Revolver
	WEAPON_REVOLVER_LEMAT, // LeMat Revolver
	WEAPON_PISTOL_VOLCANIC, // Volcanic Pistol
	WEAPON_PISTOL_SEMIAUTO, // Semi-Automatic Pistol
	WEAPON_PISTOL_MAUSER, // Mauser Pistol
	WEAPON_PISTOL_MAUSER_DRUNK, // Midnight's Pistol
	WEAPON_PISTOL_M1899, // M1899 Pistol
	WEAPON_SHOTGUN_SAWEDOFF, // Sawed-Off Shotgun
};

constexpr inline std::array g_DualWieldWeaponPickupList{
	PICKUP_WEAPON_REVOLVER_CATTLEMAN, // Cattleman Revolver
	PICKUP_WEAPON_REVOLVER_CATTLEMAN_JOHN, // John's Cattleman Revolver
	PICKUP_WEAPON_REVOLVER_DOUBLEACTION, // Double-Action Revolver
	PICKUP_WEAPON_REVOLVER_DOUBLEACTION_EXOTIC, // Algernon's Revolver
	PICKUP_WEAPON_REVOLVER_SCHOFIELD, // Schofield Revolver
	PICKUP_WEAPON_REVOLVER_LEMAT, // LeMat Revolver
	PICKUP_WEAPON_PISTOL_VOLCANIC, // Volcanic Pistol
	PICKUP_WEAPON_PISTOL_SEMIAUTO, // Semi-Automatic Pistol
	PICKUP_WEAPON_PISTOL_MAUSER, // Mauser Pistol
	PICKUP_WEAPON_PISTOL_MAUSER_DRUNK, // Midnight's Pistol
	PICKUP_WEAPON_PISTOL_M1899, // M1899 Pistol
	PICKUP_WEAPON_SHOTGUN_SAWEDOFF, // Sawed-Off Shotgun
};

constexpr inline std::array g_AmmoList{
	AMMO_22,
	AMMO_22_TRANQUILIZER,
	AMMO_ARROW,
	AMMO_ARROW_DYNAMITE,
	AMMO_ARROW_FIRE,
	AMMO_ARROW_IMPROVED,
	AMMO_ARROW_POISON,
	AMMO_ARROW_SMALL_GAME,
	AMMO_ARROW_TRACKING,
	AMMO_DYNAMITE,
	AMMO_DYNAMITE_VOLATILE,
	AMMO_HATCHET,
	AMMO_HATCHET_ANCIENT,
	AMMO_HATCHET_CLEAVER,
	AMMO_HATCHET_DOUBLE_BIT,
	AMMO_HATCHET_DOUBLE_BIT_RUSTED,
	AMMO_HATCHET_HEWING,
	AMMO_HATCHET_HUNTER,
	AMMO_HATCHET_HUNTER_RUSTED,
	AMMO_HATCHET_VIKING,
	AMMO_LASSO,
	AMMO_MOLOTOV,
	AMMO_MOLOTOV_VOLATILE,
	AMMO_MOONSHINEJUG,
	AMMO_PISTOL,
	AMMO_PISTOL_EXPRESS,
	AMMO_PISTOL_EXPRESS_EXPLOSIVE,
	AMMO_PISTOL_HIGH_VELOCITY,
	AMMO_PISTOL_SPLIT_POINT,
	AMMO_REPEATER,
	AMMO_REPEATER_EXPRESS,
	AMMO_REPEATER_EXPRESS_EXPLOSIVE,
	AMMO_REPEATER_HIGH_VELOCITY,
	AMMO_REPEATER_SPLIT_POINT,
	AMMO_REVOLVER,
	AMMO_REVOLVER_EXPRESS,
	AMMO_REVOLVER_EXPRESS_EXPLOSIVE,
	AMMO_REVOLVER_HIGH_VELOCITY,
	AMMO_REVOLVER_SPLIT_POINT,
	AMMO_RIFLE,
	AMMO_RIFLE_EXPRESS,
	AMMO_RIFLE_EXPRESS_EXPLOSIVE,
	AMMO_RIFLE_HIGH_VELOCITY,
	AMMO_RIFLE_SPLIT_POINT,
	AMMO_SHOTGUN,
	AMMO_SHOTGUN_BUCKSHOT_INCENDIARY,
	AMMO_SHOTGUN_SLUG,
	AMMO_SHOTGUN_SLUG_EXPLOSIVE,
	AMMO_THROWING_KNIVES,
	AMMO_THROWING_KNIVES_IMPROVED,
	AMMO_THROWING_KNIVES_POISON,
	AMMO_TOMAHAWK,
	AMMO_TOMAHAWK_ANCIENT,
	AMMO_TOMAHAWK_HOMING,
	AMMO_TOMAHAWK_IMPROVED,
};

const inline std::map<std::string, Hash> g_WeaponMenuList{
	{ "Algernon's Revolver", WEAPON_REVOLVER_DOUBLEACTION_EXOTIC },
	{ "Ancient Tomahawk", WEAPON_THROWN_TOMAHAWK_ANCIENT },
	{ "Antler Knife", WEAPON_MELEE_KNIFE_BEAR },
	{ "Bill's Bolt Action Rifle", WEAPON_RIFLE_BOLTACTION_BILL },
	{ "Bill's Knife", WEAPON_MELEE_KNIFE_BILL },
	{ "Bill's Schofield Revolver", WEAPON_REVOLVER_SCHOFIELD_BILL },
	{ "Binoculars", WEAPON_KIT_BINOCULARS },
	{ "Bolt Action Rifle", WEAPON_RIFLE_BOLTACTION },
	{ "Bow", WEAPON_BOW },
	{ "Breach Cannon", WEAPON_TURRET_CANNON },
	{ "Broken Pirate Sword", WEAPON_MELEE_BROKEN_SWORD },
	{ "Calloway's Revolver", WEAPON_REVOLVER_SCHOFIELD_CALLOWAY },
	{ "Camera", WEAPON_KIT_CAMERA },
	{ "Carbine Repeater", WEAPON_REPEATER_CARBINE },
	{ "Carcano Rifle", WEAPON_SNIPERRIFLE_CARCANO },
	{ "Cattleman Revolver", WEAPON_REVOLVER_CATTLEMAN },
	{ "Cattleman Revolver", WEAPON_REVOLVER_CATTLEMAN_DUALWIELD },
	{ "Charles' Bow", WEAPON_BOW_CHARLES },
	{ "Charles' Knife", WEAPON_MELEE_KNIFE_CHARLES },
	{ "Charles' Sawed-Off Shotgun", WEAPON_SHOTGUN_SAWEDOFF_CHARLES },
	{ "Civil War Knife", WEAPON_MELEE_KNIFE_CIVIL_WAR },
	{ "Cleaver", WEAPON_MELEE_CLEAVER },
	{ "Double Bit Hatchet", WEAPON_MELEE_HATCHET_DOUBLE_BIT },
	{ "Double-Action Revolver", WEAPON_REVOLVER_DOUBLEACTION },
	{ "Double-Action Revolver", WEAPON_REVOLVER_DOUBLEACTION_DUALWIELD },
	{ "Double-Barreled Shotgun", WEAPON_SHOTGUN_DOUBLEBARREL },
	{ "Dutch's Knife", WEAPON_MELEE_KNIFE_DUTCH },
	{ "Dutch's Schofield Revolver", WEAPON_REVOLVER_SCHOFIELD_DUTCH },
	{ "Dutch's Schofield Revolver", WEAPON_REVOLVER_SCHOFIELD_DUTCH_DUALWIELD },
	{ "Dynamite - Volatile", AMMO_DYNAMITE_VOLATILE },
	{ "Dynamite", WEAPON_THROWN_DYNAMITE },
	{ "Electric Lantern", WEAPON_MELEE_LANTERN_ELECTRIC },
	{ "Electrostatic Detector", WEAPON_KIT_DETECTOR },
	{ "Evans Repeater", WEAPON_REPEATER_EVANS },
	{ "Fire Bottle - Volatile", AMMO_MOLOTOV_VOLATILE },
	{ "Fire Bottle", WEAPON_THROWN_MOLOTOV },
	{ "Fishing Rod", WEAPON_FISHINGROD },
	{ "Flaco's Revolver", WEAPON_REVOLVER_CATTLEMAN_MEXICAN },
	{ "Gatling Gun", WEAPON_TURRET_GATLING },
	{ "Granger's Revolver", WEAPON_REVOLVER_CATTLEMAN_PIG },
	{ "Hatchet (Melee Only)", WEAPON_MELEE_HATCHET_MELEEONLY },
	{ "Hatchet", WEAPON_MELEE_HATCHET },
	{ "Hewing Hatchet", WEAPON_MELEE_HATCHET_HEWING },
	{ "High Roller Revolver", WEAPON_REVOLVER_DOUBLEACTION_GAMBLER },
	{ "Hosea's Cattleman Revolver", WEAPON_REVOLVER_CATTLEMAN_HOSEA },
	{ "Hosea's Cattleman Revolver", WEAPON_REVOLVER_CATTLEMAN_HOSEA_DUALWIELD },
	{ "Hosea's Knife", WEAPON_MELEE_KNIFE_HOSEA },
	{ "Hosea's Semi-Auto Shotgun", WEAPON_SHOTGUN_SEMIAUTO_HOSEA },
	{ "Hunter Hatchet", WEAPON_MELEE_HATCHET_HUNTER },
	{ "Hunting Knife", WEAPON_MELEE_KNIFE },
	{ "Improved Throwing Knife", AMMO_THROWING_KNIVES_IMPROVED },
	{ "Javier's Double-Action Revolver", WEAPON_REVOLVER_DOUBLEACTION_JAVIER },
	{ "Javier's Knife", WEAPON_MELEE_KNIFE_JAVIER },
	{ "Javier's Throwing Knife", WEAPON_THROWN_THROWING_KNIVES_JAVIER },
	{ "Jawbone Knife", WEAPON_MELEE_KNIFE_JAWBONE },
	{ "John's Cattleman Revolver", WEAPON_REVOLVER_CATTLEMAN_JOHN },
	{ "John's Knife", WEAPON_MELEE_KNIFE_JOHN },
	{ "John's Lancaster Repeater", WEAPON_REPEATER_WINCHESTER_JOHN },
	{ "Kieran's Cattleman Revolver", WEAPON_REVOLVER_CATTLEMAN_KIERAN },
	{ "Kieran's Knife", WEAPON_MELEE_KNIFE_KIERAN },
	{ "Lancaster Repeater", WEAPON_REPEATER_WINCHESTER },
	{ "Lantern", WEAPON_MELEE_DAVY_LANTERN },
	{ "Lasso", WEAPON_LASSO },
	{ "LeMat Revolver", WEAPON_REVOLVER_LEMAT },
	{ "Lenny's Cattleman Revolver", WEAPON_REVOLVER_CATTLEMAN_LENNY },
	{ "Lenny's Knife", WEAPON_MELEE_KNIFE_LENNY },
	{ "Lenny's Rolling Block Rifle", WEAPON_SNIPERRIFLE_ROLLINGBLOCK_LENNY },
	{ "Litchfield Repeater", WEAPON_REPEATER_HENRY },
	{ "M1899 Pistol", WEAPON_PISTOL_M1899 },
	{ "Machete", WEAPON_MELEE_MACHETE },
	{ "Mauser Pistol", WEAPON_PISTOL_MAUSER },
	{ "Maxim Gun", WEAPON_TURRET_MAXIM },
	{ "Micah's Knife", WEAPON_MELEE_KNIFE_MICAH },
	{ "Micah's Revolver", WEAPON_REVOLVER_DOUBLEACTION_MICAH },
	{ "Micah's Revolver", WEAPON_REVOLVER_DOUBLEACTION_MICAH_DUALWIELD },
	{ "Midnight's Pistol", WEAPON_PISTOL_MAUSER_DRUNK },
	{ "Moonshine Jug", WEAPON_MOONSHINEJUG },
	{ "Old Lantern", WEAPON_MELEE_LANTERN },
	{ "Ornate Dagger", WEAPON_MELEE_KNIFE_VAMPIRE },
	{ "Otis Miller's Revolver", WEAPON_REVOLVER_SCHOFIELD_GOLDEN },
	{ "Poison Throwing Knife", AMMO_THROWING_KNIVES_POISON },
	{ "Pump-Action Shotgun", WEAPON_SHOTGUN_PUMP },
	{ "Rare Rolling Block Rifle", WEAPON_SNIPERRIFLE_ROLLINGBLOCK_EXOTIC },
	{ "Rare Shotgun", WEAPON_SHOTGUN_DOUBLEBARREL_EXOTIC },
	{ "Repeating Shotgun", WEAPON_SHOTGUN_REPEATING },
	{ "Revolving Cannon", WEAPON_TURRET_REVOLVING_CANNON },
	{ "Rolling Block Rifle", WEAPON_SNIPERRIFLE_ROLLINGBLOCK },
	{ "Rusted Double Bit Hatchet", WEAPON_MELEE_HATCHET_DOUBLE_BIT_RUSTED },
	{ "Rusted Hunter Hatchet", WEAPON_MELEE_HATCHET_HUNTER_RUSTED },
	{ "Sadie's Carbine Repeater", WEAPON_REPEATER_CARBINE_SADIE },
	{ "Sadie's Cattleman Revolver", WEAPON_REVOLVER_CATTLEMAN_SADIE },
	{ "Sadie's Cattleman Revolver", WEAPON_REVOLVER_CATTLEMAN_SADIE_DUALWIELD },
	{ "Sadie's Knife", WEAPON_MELEE_KNIFE_SADIE },
	{ "Sawed-Off Shotgun", WEAPON_SHOTGUN_SAWEDOFF },
	{ "Schofield Revolver", WEAPON_REVOLVER_SCHOFIELD },
	{ "Schofield Revolver", WEAPON_REVOLVER_SCHOFIELD_DUALWIELD },
	{ "Sean's Cattleman Revolver", WEAPON_REVOLVER_CATTLEMAN_SEAN },
	{ "Sean's Knife", WEAPON_MELEE_KNIFE_SEAN },
	{ "Semi-Auto Shotgun", WEAPON_SHOTGUN_SEMIAUTO },
	{ "Semi-Automatic Pistol", WEAPON_PISTOL_SEMIAUTO },
	{ "Springfield Rifle", WEAPON_RIFLE_SPRINGFIELD },
	{ "Stone Hatchet", WEAPON_MELEE_ANCIENT_HATCHET },
	{ "Throwing Knife", WEAPON_THROWN_THROWING_KNIVES },
	{ "Tomahawk (Melee Only)", WEAPON_THROWN_TOMAHAWK_MELEEONLY },
	{ "Tomahawk - Homing", AMMO_TOMAHAWK_HOMING },
	{ "Tomahawk - Improved", AMMO_TOMAHAWK_IMPROVED },
	{ "Tomahawk", WEAPON_THROWN_TOMAHAWK },
	{ "Torch", WEAPON_MELEE_TORCH },
	{ "Torch", WEAPON_MELEE_TORCH_CROWD },
	{ "Unarmed", WEAPON_UNARMED },
	{ "Uncle's Double-Barreled Shotgun", WEAPON_SHOTGUN_DOUBLEBARREL_UNCLE },
	{ "Uncle's Knife", WEAPON_MELEE_KNIFE_UNCLE },
	{ "Uncle's Schofield Revolver", WEAPON_REVOLVER_SCHOFIELD_UNCLE },
	{ "Varmint Rifle", WEAPON_RIFLE_VARMINT },
	{ "Viking Hatchet", WEAPON_MELEE_HATCHET_VIKING },
	{ "Volcanic Pistol", WEAPON_PISTOL_VOLCANIC },
	{ "Wide-Blade Knife", WEAPON_MELEE_KNIFE_MINER },
};
