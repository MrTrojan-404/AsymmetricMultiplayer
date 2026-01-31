// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "AsymmetricCoop/Actor/Weapon.h"
#include "AsymmetricCoop/Interface/CombatInterface.h"
#include "Net/UnrealNetwork.h"


UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();

}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, PrimaryEquippedWeapon);
	DOREPLIFETIME(UCombatComponent, SecondaryEquippedWeapon);
	DOREPLIFETIME(UCombatComponent, CombatState);
}
#pragma region RepNotify

void UCombatComponent::OnRep_CombatState()
{
	
}

void UCombatComponent::OnRep_PrimaryEquippedWeapon()
{
	if (!PrimaryEquippedWeapon) return;
	
}

void UCombatComponent::OnRep_SecondaryEquippedWeapon()
{
	if (!SecondaryEquippedWeapon) return;
	
}

#pragma endregion RepNotify

// Only For Human Character

#pragma region Equip

void UCombatComponent::EquipItem(AItem* ItemToEquip)
{
	if (ItemToEquip == nullptr) return;

	// If item is a Weapon, Call Equip Weapon
	if (!ItemToEquip) return;

	if (AWeapon* Weapon = Cast<AWeapon>(ItemToEquip))
	{
		EquipWeapon(Weapon);
	}
}


void UCombatComponent::ServerEquip_Implementation(AItem* ItemToEquip)
{
	if (!ItemToEquip) return;

	AActor* ActorOwner = GetOwner();
	if (!ActorOwner || !ActorOwner->HasAuthority()) return;

	// ️Make sure it's a weapon
	AWeapon* Weapon = Cast<AWeapon>(ItemToEquip);
	if (!Weapon) return;

	//Make sure the player is actually near it
	const float Distance = FVector::Dist(
		ActorOwner->GetActorLocation(),
		Weapon->GetActorLocation()
	);

	if (Distance > 200.f) return; // reject cheating / invalid calls

	EquipWeapon(Weapon);
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (!OwnerActor || !WeaponToEquip) return;
	if (!OwnerActor->HasAuthority()) return;

	WeaponToEquip->SetOwner(
		ICombatInterface::Execute_GetCombatOwner(OwnerActor)
	);

	// PRIMARY
	if (PrimaryEquippedWeapon == nullptr)
	{
		PrimaryEquippedWeapon = WeaponToEquip;
		PlayerWeaponState = EPlayerWeaponState::EPWS_HasOneWeapon;
		WeaponToEquip->SetWeaponState(EWeaponState::EWS_Equipped);
		
		return;
	}

	// SECONDARY
	if (SecondaryEquippedWeapon == nullptr)
	{
		SecondaryEquippedWeapon = WeaponToEquip;
		PlayerWeaponState = EPlayerWeaponState::EPWS_HasTwoWeapons;
		WeaponToEquip->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
	}
}
#pragma endregion Equip

#pragma region Inventory

void UCombatComponent::EquippedSwap()
{
	if (GetCombatState() != ECombatState::ECS_Unoccupied ||
	GetPlayerWeaponState() != EPlayerWeaponState::EPWS_HasTwoWeapons) return;

	if (!GetOwner()->HasAuthority())
	{
		bPredictedSwap = true;
		PlayLocalEquippedSwap();
		ServerEquippedSwapWeapon();
	}

	if (true)
	{
		HandleEquippedSwap();
	}
}

void UCombatComponent::PlayLocalEquippedSwap()
{
	if (OwnerActor)
	{
		ICombatInterface::Execute_PlayEquippedSwapMontage(OwnerActor);
	}
}

void UCombatComponent::ServerEquippedSwapWeapon_Implementation()
{
	HandleEquippedSwap();
}


void UCombatComponent::HandleEquippedSwap()
{
	CombatState = ECombatState::ECS_SwappingWeapons;
	MulticastEquippedSwapWeapon();
}

void UCombatComponent::MulticastEquippedSwapWeapon_Implementation()
{
	if (bPredictedSwap)
	{
		bPredictedSwap = false;
		return;
	}
	
	PlayLocalEquippedSwap();	
}


void UCombatComponent::OnEquippedSwapFinished()
{
	if (!GetOwner()->HasAuthority()) return;

	if (!PrimaryEquippedWeapon || !SecondaryEquippedWeapon)
	{
		CombatState = ECombatState::ECS_Unoccupied;
		return;
	}

	CombatState = ECombatState::ECS_Unoccupied;

	AWeapon* Temp = PrimaryEquippedWeapon;
	PrimaryEquippedWeapon = SecondaryEquippedWeapon;
	SecondaryEquippedWeapon = Temp;

	PrimaryEquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	SecondaryEquippedWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
}


void UCombatComponent::ServerDropWeapon_Implementation()
{
	DropEquippedWeapon();
}

void UCombatComponent::DropEquippedWeapon()
{
	if (!GetOwner()->HasAuthority()) return;
	if (!PrimaryEquippedWeapon) return;

	PrimaryEquippedWeapon->Dropped();
	PrimaryEquippedWeapon = nullptr;

	if (SecondaryEquippedWeapon)
	{
		PrimaryEquippedWeapon = SecondaryEquippedWeapon;
		SecondaryEquippedWeapon = nullptr;

		PlayerWeaponState = EPlayerWeaponState::EPWS_HasOneWeapon;

		PrimaryEquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	}
	else
	{
		PlayerWeaponState = EPlayerWeaponState::EPWS_HasNoWeapon;
	}
}

#pragma endregion Inventory

#pragma region Helper Functions

EPlayerWeaponState UCombatComponent::GetPlayerWeaponState() const
{
	if (PrimaryEquippedWeapon && SecondaryEquippedWeapon)
		return EPlayerWeaponState::EPWS_HasTwoWeapons;
	if (PrimaryEquippedWeapon)
		return EPlayerWeaponState::EPWS_HasOneWeapon;
	return EPlayerWeaponState::EPWS_HasNoWeapon;
}

#pragma endregion Helper Functions
