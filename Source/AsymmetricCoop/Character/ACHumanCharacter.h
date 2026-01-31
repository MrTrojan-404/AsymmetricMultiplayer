// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACBaseCharacter.h"
#include "ACHumanCharacter.generated.h"

UCLASS()
class ASYMMETRICCOOP_API AACHumanCharacter : public AACBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AACHumanCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
