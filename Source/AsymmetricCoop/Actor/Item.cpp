// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "AsymmetricCoop/WeaponTypes.h"
#include "AsymmetricCoop/Component/PickupComponent.h"
#include "AsymmetricCoop/Interface/CombatInterface.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"


AItem::AItem()
{

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);


	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemMesh->SetGenerateOverlapEvents(false);


	EnableCustomDepth(true);
	ItemMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	ItemMesh->MarkRenderStateDirty();
	
	PickupComponent = CreateDefaultSubobject<UPickupComponent>(TEXT("PickupComponent"));

	PickupWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidgetComponent->SetupAttachment(RootComponent);
	PickupWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	PickupWidgetComponent->SetVisibility(false);

	TraceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TraceBox"));
	TraceBox->SetupAttachment(RootComponent);

	// Make it cover the weapon shape
	TraceBox->SetBoxExtent(FVector(20.f, 20.f, 70.f));
	TraceBox->SetRelativeLocation(FVector(0.f, 0.f, 40.f));

	TraceBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TraceBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TraceBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AItem::EnableCustomDepth(bool bEnable)
{
	if (ItemMesh)
	{
		ItemMesh->SetRenderCustomDepth(bEnable);
	}
}

void AItem::Interact_Implementation(AActor* Interactor)
{
	if (!Interactor) return;

	// Does the interactor support combat?
	if (Interactor->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
	{
		ICombatInterface::Execute_OnEquipItem(Interactor, this);
	}
}

FText AItem::GetInteractionText_Implementation() const
{
	return FText::FromString("Pick Up");
}

void AItem::ShowPickupWidget(bool bShow)
{
	if (PickupWidgetComponent)
	{
		PickupWidgetComponent->SetVisibility(bShow);
	}
}

void AItem::EnableOutline(bool bEnable)
{
	ItemMesh->SetRenderCustomDepth(bEnable);
}
