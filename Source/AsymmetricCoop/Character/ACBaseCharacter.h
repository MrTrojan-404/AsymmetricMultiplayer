// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsymmetricCoop/CombatState.h"
#include "AsymmetricCoop/Interface/AnimInterface.h"
#include "AsymmetricCoop/Interface/CombatInterface.h"
#include "GameFramework/Character.h"
#include "ACBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCombatComponent;
class UInteractionComponent;

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	ECT_Human UMETA(DisplayName = "Human"),
	ECT_Monster UMETA(DisplayName = "Monster"),
	
	ECT_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class ASYMMETRICCOOP_API AACBaseCharacter : public ACharacter, public ICombatInterface, public IAnimInterface
{
	GENERATED_BODY()

public:
	AACBaseCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region CombatInterface

	virtual USkeletalMeshComponent* GetCombatMesh_Implementation() const override;
	virtual AActor* GetCombatOwner_Implementation() const override;
	
#pragma endregion CombatInterface

#pragma region AnimInterface
	
	virtual float GetGroundSpeed_Implementation() const override;
	virtual bool IsFalling_Implementation() const override;
	
#pragma endregion AnimInterface

	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	ECharacterType CharacterType;
		
#pragma region  Input
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;
	
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	
	void SetMovementRotationModel(bool bUseControllerYaw);

#pragma endregion Input

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;
	
};
