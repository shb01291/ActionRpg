// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface //unreal reflection�� ���Ǵ� class
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONRPG_API IHitInterface // ���� ����� ���� ���Ǵ� class, �Լ� �����ϴ� class
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent) //c++, BP event �Ѵ� Ȱ�밡��
	void GetHit(const FVector& ImpactPoint, AActor* Hitter);
	
	//virtual void GetHit(const FVector& ImpactPoint) = 0; //pure virtual
};
