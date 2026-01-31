// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMonsterCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AACMonsterCharacter::AACMonsterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator::ZeroRotator;

	SpringArm->bUsePawnControlRotation = true;
	
}

// Called when the game starts or when spawned
void AACMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

