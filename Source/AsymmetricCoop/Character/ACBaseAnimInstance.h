// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AsymmetricCoop/Interface/AnimInterface.h"
#include "ACBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ASYMMETRICCOOP_API UACBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = Character)
	EEquipState EquipState;

private:
	UPROPERTY()
	TScriptInterface<IAnimInterface> AnimOwner;
};
