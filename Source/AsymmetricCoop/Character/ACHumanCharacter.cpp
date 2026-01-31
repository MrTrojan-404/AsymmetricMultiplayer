// Fill out your copyright notice in the Description page of Project Settings.


#include "ACHumanCharacter.h"


// Sets default values
AACHumanCharacter::AACHumanCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AACHumanCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AACHumanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AACHumanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

