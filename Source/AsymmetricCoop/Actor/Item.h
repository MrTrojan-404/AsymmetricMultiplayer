// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsymmetricCoop/Interface/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UPickupComponent;

UCLASS()
class ASYMMETRICCOOP_API AItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AItem();
	
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractionText_Implementation() const override;
	
	void ShowPickupWidget(bool bShow);
	void EnableOutline(bool bEnable);

	void EnableCustomDepth(bool bEnable);

	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return ItemMesh; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPickupComponent* PickupComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UWidgetComponent* PickupWidgetComponent;

	UPROPERTY(VisibleAnywhere, Category="Interaction")
	class UBoxComponent* TraceBox;

private:
};
