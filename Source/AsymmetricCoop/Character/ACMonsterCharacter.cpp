// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMonsterCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


AACMonsterCharacter::AACMonsterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator::ZeroRotator;

	SpringArm->bUsePawnControlRotation = true;

	CharacterType = ECharacterType::ECT_Monster;
}

void AACMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocallyControlled()) return;

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (auto* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MonsterMappingContext, 1);
		}
	}
	
}

// Called every frame
void AACMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AACMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

