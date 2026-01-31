// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_EquippedSecondary UMETA(DisplayName = "Equipped Secondary"),
	EWS_Dropped UMETA(DisplayName = "Unequipped"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};


UENUM(BlueprintType)
enum class EFireType : uint8
{
	EFT_HitScan UMETA(DisplayName = "Hit Scan Weapon"),
	EFT_Projectile UMETA(DisplayName = "Projectile Weapon"),
	EFT_Shotgun UMETA(DisplayName = "Shotgun Weapon"),

	EFT_MAX UMETA(DisplayName = "DefaultMAX")
};


UCLASS()
class ASYMMETRICCOOP_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SetWeaponState(EWeaponState WeaponStatetoSet);
	void Dropped();

protected:


private:
	UPROPERTY(ReplicatedUsing= OnRep_WeaponState)
	EWeaponState WeaponState = EWeaponState::EWS_Initial;

	UFUNCTION()
	void OnRep_WeaponState();

	void OnWeaponStateSet();
	void OnEquipped();
	void OnDropped();
	void OnEquippedSecondary();

	void AttachToOwnerHand();
	void AttachToOwnerBack();
};
