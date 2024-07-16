// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

enum class EItemState : uint8
{
	EIS_Normal,
	EIS_Equipped
};

UCLASS()
class ACTIONRPG_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;
	float Runningtime;
	UPROPERTY(EditAnywhere)
		float Amplitude = 0.25f;
	UPROPERTY(EditInstanceOnly)
	float TimeConstant = 5.f;

	template<typename T>
	T Avg(T First, T Second);
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SpawnPickUpSystem();
	virtual void SpawnPickUpSound();

	EItemState ItemState = EItemState::EIS_Normal;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ItemEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* PickUpSound;
private:

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* PickUpEffect;
	
};

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First+Second) / 2;
}
