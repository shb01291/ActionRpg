// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface //unreal reflection에 사용되는 class
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONRPG_API IHitInterface // 다중 상속을 위해 사용되는 class, 함수 선언하는 class
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent) //c++, BP event 둘다 활용가능
	void GetHit(const FVector& ImpactPoint, AActor* Hitter);
	
	//virtual void GetHit(const FVector& ImpactPoint) = 0; //pure virtual
};
