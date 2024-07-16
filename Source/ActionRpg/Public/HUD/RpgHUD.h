// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RpgHUD.generated.h"
class URpgOverlay;

/**
 * 
 */
UCLASS()
class ACTIONRPG_API ARpgHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditDefaultsOnly, Category = RPG)
	TSubclassOf<URpgOverlay> RpgOverlayClass;

	UPROPERTY()
	URpgOverlay* RpgOverlay;

public:
	FORCEINLINE URpgOverlay* GetRpgOverlay() const { return RpgOverlay; }
};
