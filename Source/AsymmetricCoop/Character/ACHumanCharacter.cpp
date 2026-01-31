// Fill out your copyright notice in the Description page of Project Settings.


#include "ACHumanCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AsymmetricCoop/Actor/Item.h"
#include "AsymmetricCoop/Component/CombatComponent.h"
#include "AsymmetricCoop/Component/InteractionComponent.h"


AACHumanCharacter::AACHumanCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterType = ECharacterType::ECT_Human;

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

}

void AACHumanCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocallyControlled()) return;

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (auto* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(HumanMappingContext, 1);
		}
	}
}



void AACHumanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InteractionComponent && InteractionComponent->NearbyItem)
	{
		UpdateInteractionFocus();
	}
}

#pragma region PickupInterface

void AACHumanCharacter::OnPickupBegin_Implementation(AActor* PickupActor)
{
	AItem* Item = Cast<AItem>(PickupActor);
	if (!Item || !CombatComponent) return;

	InteractionComponent->NearbyItem = Item;
}

void AACHumanCharacter::OnPickupEnd_Implementation(AActor* PickupActor)
{
	AItem* Item = Cast<AItem>(PickupActor);
	if (!Item) return;

	if (InteractionComponent->NearbyItem == Item)
	{
		InteractionComponent->NearbyItem = nullptr;
	}

	if (InteractionComponent->FocusedItem == Item)
	{
		ClearFocusedItem();
	}
}

#pragma endregion PickupInterface

#pragma region CombatInterface


void AACHumanCharacter::PlayEquippedSwapMontage_Implementation()
{
	if (CombatComponent == nullptr || CombatComponent->GetPrimaryEquippedWeapon() == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		if (SwapMontage)
		{
			AnimInstance->Montage_Play(SwapMontage);
		}
	}
}

void AACHumanCharacter::OnSwapEquippedFinished_Implementation()
{
	ICombatInterface::OnSwapEquippedFinished_Implementation();

	if (CombatComponent)
	{
		CombatComponent->OnEquippedSwapFinished();
	}
}

#pragma endregion CombatInterface

#pragma region Interaction

void AACHumanCharacter::UpdateInteractionFocus()
{
	if (!IsLocallyControlled()) return;

	// If nothing nearby, don't trace at all
	if (!InteractionComponent || !InteractionComponent->NearbyItem)
	{
		ClearFocusedItem();
		return;
	}
	
	AController* SlashController = GetController();
	if (!SlashController) return;

	FVector Start;
	FRotator Rot;
	SlashController->GetPlayerViewPoint(Start, Rot);

	FVector End = Start + Rot.Vector() * 500.f;

	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		FColor::Green,
		false,
		0.1f,
		0,
		1.5f
	);
	
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			0.f,
			FColor::Yellow,
			FString::Printf(TEXT("Trace Hit: %s"),
				bHit ? *GetNameSafe(Hit.GetActor()) : TEXT("NONE"))
		);
	}
	// Only accept if we hit the focused item
	AItem* HitItem = bHit ? Cast<AItem>(Hit.GetActor()) : nullptr;

	// Only focus if the hit item is the one we're near
	if (HitItem && HitItem == InteractionComponent->NearbyItem)
	{
		SetFocusedItem(HitItem);
	}
	else
	{
		ClearFocusedItem();
	}
}

void AACHumanCharacter::SetFocusedItem(AItem* NewItem)
{
	if (InteractionComponent->FocusedItem == NewItem) return;
	
	ClearFocusedItem();

	InteractionComponent->FocusedItem = NewItem;

	if (InteractionComponent->FocusedItem)
	{
		InteractionComponent->FocusedItem->ShowPickupWidget(true);
	}
}

void AACHumanCharacter::ClearFocusedItem()
{
	if (InteractionComponent->FocusedItem)
	{
		InteractionComponent->FocusedItem->ShowPickupWidget(false);
	}

	InteractionComponent->FocusedItem = nullptr;
}

void AACHumanCharacter::OnEquipItem_Implementation(AItem* Item)
{
	if (!CombatComponent || !Item) return;

	CombatComponent->ServerEquip(Item);
}

#pragma endregion Interaction

#pragma endregion Input

void AACHumanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AACHumanCharacter::InteractButtonPressed);
		EnhancedInputComponent->BindAction(SwapWeaponAction, ETriggerEvent::Triggered, this, &AACHumanCharacter::SwapButtonPressed);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &AACHumanCharacter::DropButtonPressed);
	}
}



void AACHumanCharacter::InteractButtonPressed()
{
	if (InteractionComponent)
	{
		InteractionComponent->TryInteract();
	}
}

void AACHumanCharacter::DropButtonPressed()
{
	if (CombatComponent)
	{
		CombatComponent->ServerDropWeapon();
	}
}

void AACHumanCharacter::SwapButtonPressed()
{
	if (CombatComponent)
	{
		CombatComponent->EquippedSwap();
	}
}

#pragma endregion Input