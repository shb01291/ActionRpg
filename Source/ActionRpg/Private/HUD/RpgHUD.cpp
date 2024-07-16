// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RpgHUD.h"
#include "HUD/RpgOverlay.h"

void ARpgHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && RpgOverlayClass)
		{
			RpgOverlay = CreateWidget<URpgOverlay>(Controller, RpgOverlayClass);
			RpgOverlay->AddToViewport();
		}
	}
}
