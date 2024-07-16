// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONRPG_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenStamina(float DeltaTime);
protected:
	virtual void BeginPlay() override;

private:
	//현재 체력
	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float Stamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float DodgeCost = 20.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float StaminaRegenRate = 8.f;
public:
	//setter health
	void ReceiveDamage(float Damage);
	void UseStamina(float StaminaCost);
	float GetHealthPercent();
	float GetStaminaPercent();
	bool IsAlive();
	void AddSouls(int32 NumberOfSouls);
	void AddGold(int32 AmountOfGold);
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE int32 GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE int32 GetStamina() const { return Stamina; }


};
