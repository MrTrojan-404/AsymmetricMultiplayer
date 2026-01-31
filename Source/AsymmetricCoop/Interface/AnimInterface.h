// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsymmetricCoop/CombatState.h"
#include "UObject/Interface.h"
#include "AnimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASYMMETRICCOOP_API IAnimInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Animation")
	float GetGroundSpeed() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Animation")
	bool IsFalling() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Animation")
	EEquipState GetEquipState() const;
};
