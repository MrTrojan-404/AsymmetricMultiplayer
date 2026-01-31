// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACBaseCharacter.h"
#include "AsymmetricCoop/Interface/InteractionInterface.h"
#include "AsymmetricCoop/Interface/PickupInterface.h"
#include "ACHumanCharacter.generated.h"

UCLASS()
class ASYMMETRICCOOP_API AACHumanCharacter : public AACBaseCharacter,
public IPickupInterface,public IInteractionInterface
{
	GENERATED_BODY()

public:
	AACHumanCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnPickupBegin_Implementation(AActor* PickupActor) override;
	virtual void OnPickupEnd_Implementation(AActor* PickupActor) override;

#pragma region CombatInterface

	virtual void PlayEquippedSwapMontage_Implementation() override;
	virtual  void OnSwapEquippedFinished_Implementation() override;
	
#pragma endregion CombatInterface

	
#pragma region AnimInterface

	
#pragma endregion AnimInterface

	
protected:
	virtual void BeginPlay() override;

#pragma region Input

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* HumanMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SwapWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* DropAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* FireAction;

	void InteractButtonPressed();
	void DropButtonPressed();
	void SwapButtonPressed();
	
#pragma endregion Input

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UInteractionComponent* InteractionComponent;
	
#pragma region Interaction
	
	void UpdateInteractionFocus();
	void SetFocusedItem(AItem* NewItem);
	void ClearFocusedItem();
	virtual void OnEquipItem_Implementation(AItem* Item) override;
	
#pragma endregion Interaction

#pragma region AnimationMontages

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* SwapMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;
	
#pragma endregion AnimationMontage

};
