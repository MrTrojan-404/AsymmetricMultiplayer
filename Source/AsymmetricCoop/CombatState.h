#pragma once

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	ECS_SwappingWeapons UMETA(DisplayName = "Swapping Weapons"),

	EAS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EEquipState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequippied"),
	ECS_EquippedPrimary UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedSecondary UMETA(DisplayName = "Equipped Two-Handed Weapon"),
	
	ECS_MAX UMETA(DisplayName = "DefaultMAX")

};