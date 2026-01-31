// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class AWeapon;
class AItem;
// This class does not need to be modified.
UINTERFACE()
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASYMMETRICCOOP_API ICombatInterface
{
	GENERATED_BODY()

public:
	// Where should weapons be attached?
	UFUNCTION(BlueprintNativeEvent, Category="Combat")
	USkeletalMeshComponent* GetCombatMesh() const;

	// Who owns the weapon after equip?
	UFUNCTION(BlueprintNativeEvent, Category="Combat")
	AActor* GetCombatOwner() const;

	UFUNCTION(BlueprintNativeEvent, Category="Combat")
	void OnEquipItem(AItem* Item);

	UFUNCTION(BlueprintNativeEvent, Category="Combat")
	void AttachToSpringArm(AWeapon* WeaponToAttach);
	
};
