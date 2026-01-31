// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACBaseCharacter.h"
#include "ACMonsterCharacter.generated.h"

UCLASS()
class ASYMMETRICCOOP_API AACMonsterCharacter : public AACBaseCharacter
{
	GENERATED_BODY()

public:
	AACMonsterCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	virtual void BeginPlay() override;

#pragma region Input

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* MonsterMappingContext;
	
#pragma endregion Input

private:

};
