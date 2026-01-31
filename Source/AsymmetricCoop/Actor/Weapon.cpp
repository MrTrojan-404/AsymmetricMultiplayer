// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "AsymmetricCoop/WeaponTypes.h"
#include "AsymmetricCoop/Component/PickupComponent.h"
#include "AsymmetricCoop/Interface/CombatInterface.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
}


#pragma region Handle Weapon States

void AWeapon::SetWeaponState(EWeaponState WeaponStatetoSet)
{
	WeaponState = WeaponStatetoSet;
	OnWeaponStateSet();
}

void AWeapon::Dropped()
{
	SetWeaponState(EWeaponState::EWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	ItemMesh->DetachFromComponent(DetachRules);
	SetOwner(nullptr);
}

void AWeapon::OnRep_WeaponState()
{
	OnWeaponStateSet();
}

void AWeapon::OnWeaponStateSet()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		OnEquipped();
		break;
	case EWeaponState::EWS_EquippedSecondary:
		OnEquippedSecondary();
		break;
	case EWeaponState::EWS_Dropped:
		OnDropped();
		break;
	}
}

void AWeapon::OnDropped()
{
	PickupComponent->PickupSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetEnableGravity(true);
	SetReplicateMovement(true);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	TraceBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TraceBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TraceBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	ItemMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	ItemMesh->MarkRenderStateDirty();
	EnableCustomDepth(true);

}

void AWeapon::OnEquipped()
{
	ShowPickupWidget(false);
	PickupComponent->PickupSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemMesh->SetSimulatePhysics(false);
	ItemMesh->SetEnableGravity(false);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	TraceBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TraceBox->SetCollisionResponseToAllChannels(ECR_Ignore);

	AttachToOwnerHand();
	EnableCustomDepth(false);
}

void AWeapon::OnEquippedSecondary()
{
	ShowPickupWidget(false);
	PickupComponent->PickupSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemMesh->SetSimulatePhysics(false);
	ItemMesh->SetEnableGravity(false);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	TraceBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TraceBox->SetCollisionResponseToAllChannels(ECR_Ignore);

	AttachToOwnerBack();
	
	EnableCustomDepth(true);
	
	ItemMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_TAN);
	ItemMesh->MarkRenderStateDirty();
}

#pragma  endregion Handle Weapon States


#pragma region Helper Functions

void AWeapon::AttachToOwnerHand()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return;

	ICombatInterface* CombatOwner = Cast<ICombatInterface>(OwnerActor);
	if (!CombatOwner) return;

	USkeletalMeshComponent* Mesh =
		ICombatInterface::Execute_GetCombatMesh(OwnerActor);

	if (!Mesh) return;

	const USkeletalMeshSocket* RightHandSocket =
		Mesh->GetSocketByName("RightHandSocket");

	if (RightHandSocket)
	{
		RightHandSocket->AttachActor(this, Mesh);
	}
}

void AWeapon::AttachToOwnerBack()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return;

	ICombatInterface* CombatOwner = Cast<ICombatInterface>(OwnerActor);
	if (!CombatOwner) return;

	USkeletalMeshComponent* Mesh =
		ICombatInterface::Execute_GetCombatMesh(OwnerActor);

	if (!Mesh) return;

	const USkeletalMeshSocket* BackSocket =
		Mesh->GetSocketByName("BackSocket");

	if (BackSocket)
	{
		BackSocket->AttachActor(this, Mesh);
	}
}

#pragma endregion Helper Functions