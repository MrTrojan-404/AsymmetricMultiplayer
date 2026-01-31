// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsymmetricCoop/CombatState.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class AWeapon;
class AItem;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ASYMMETRICCOOP_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void EquipItem(AItem* ItemToEquip);
	void EquipWeapon(AWeapon* WeaponToEquip);

	void DropEquippedWeapon();
	void HandleEquippedSwap();
	void EquippedSwap();
	void PlayLocalEquippedSwap();
	void OnEquippedSwapFinished();

#pragma region RPC
	UFUNCTION(Server, Reliable)
	void ServerEquip(AItem* ItemToEquip);
	
	UFUNCTION(Server, Reliable)
	void ServerDropWeapon();

	UFUNCTION(Server, Reliable)
	void ServerEquippedSwapWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastEquippedSwapWeapon();

#pragma endregion RPC
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AActor* OwnerActor;

	UPROPERTY(ReplicatedUsing = OnRep_PrimaryEquippedWeapon)
	AWeapon* PrimaryEquippedWeapon;
	
	UPROPERTY(ReplicatedUsing = OnRep_SecondaryEquippedWeapon)
	AWeapon* SecondaryEquippedWeapon;

	UPROPERTY(Replicated)
	EPlayerWeaponState PlayerWeaponState = EPlayerWeaponState::EPWS_HasNoWeapon;
	
	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
	void OnRep_CombatState();

	UFUNCTION()
	void OnRep_PrimaryEquippedWeapon();

	UFUNCTION()
	void OnRep_SecondaryEquippedWeapon();

	EPlayerWeaponState GetPlayerWeaponState() const;

	bool bPredictedSwap = false;

public:
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE AWeapon* GetPrimaryEquippedWeapon() const {return  PrimaryEquippedWeapon;};

};
