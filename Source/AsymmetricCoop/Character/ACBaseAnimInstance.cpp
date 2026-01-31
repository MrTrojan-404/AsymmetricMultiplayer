// Fill out your copyright notice in the Description page of Project Settings.


#include "ACBaseAnimInstance.h"


void UACBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* PawnOwner = TryGetPawnOwner();
	if (PawnOwner && PawnOwner->GetClass()->ImplementsInterface(UAnimInterface::StaticClass()))
	{
		AnimOwner.SetObject(PawnOwner);
		AnimOwner.SetInterface(Cast<IAnimInterface>(PawnOwner));
	}
}

void UACBaseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!AnimOwner) return;

	GroundSpeed = IAnimInterface::Execute_GetGroundSpeed(AnimOwner.GetObject());
	IsFalling   = IAnimInterface::Execute_IsFalling(AnimOwner.GetObject());
}