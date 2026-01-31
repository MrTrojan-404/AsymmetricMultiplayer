// Fill out your copyright notice in the Description page of Project Settings.


#include "ACBaseCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AsymmetricCoop/Component/CombatComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AACBaseCharacter::AACBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->AirControl = 0.7f;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 150.0f;
	SpringArm->SocketOffset.Y = 50.0f;
	SpringArm->bUsePawnControlRotation = true;


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);
	
}

void AACBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	// Input
	if (!IsLocallyControlled()) return;

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AACBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region CombatInterface

USkeletalMeshComponent* AACBaseCharacter::GetCombatMesh_Implementation() const
{
	return GetMesh();
}

AActor* AACBaseCharacter::GetCombatOwner_Implementation() const
{
	return const_cast<AACBaseCharacter*>(this);
}

#pragma region AnimInterface


float AACBaseCharacter::GetGroundSpeed_Implementation() const
{
	return GetVelocity().Size2D();
}

bool AACBaseCharacter::IsFalling_Implementation() const
{
	if (GetCharacterMovement())
	{
		return GetCharacterMovement()->IsFalling();
	}
	return false;
}

/*EEquipState AACBaseCharacter::GetEquipState_Implementation() const
{
	if (CombatComponent)
	{
		return CombatComponent->GetEquipState();
	}
	return EEquipState::ECS_Unequipped;
}*/


#pragma endregion AnimInterface

#pragma endregion CombatInterface

#pragma region Input

void AACBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AACBaseCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AACBaseCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AACBaseCharacter::AttackButtonPressed);

	}
}


void AACBaseCharacter::Move(const FInputActionValue& value)
{
	if (Controller != nullptr)
	{
		// Forward and Backward Movement Input
		const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.0f);
		const FVector ForwardDirection(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(ForwardDirection, value.Get<FVector>().Y);

		// Left and Right Movement
		const FVector RightDirection(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(RightDirection, value.Get<FVector>().X);
	}
}

void AACBaseCharacter::Look(const FInputActionValue& value)
{
	AddControllerPitchInput(value.Get<FVector2d>().Y);
	AddControllerYawInput(value.Get<FVector2d>().X);
}

void AACBaseCharacter::AttackButtonPressed()
{
	
}

#pragma endregion Input

#pragma region Helper Functions

void AACBaseCharacter::SetRotationModel(bool bUseControllerYaw)
{
	bUseControllerRotationYaw = bUseControllerYaw;

	GetCharacterMovement()->bOrientRotationToMovement = !bUseControllerYaw;

	SpringArm->bUsePawnControlRotation = bUseControllerYaw;
}

#pragma endregion Helper Functions