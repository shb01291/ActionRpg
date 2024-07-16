// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RpgOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void URpgOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void URpgOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void URpgOverlay::SetGold(int32 Gold)
{
	if (GoldCountText)
	{
		GoldCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));

	}
}

void URpgOverlay::SetSouls(int32 Souls)
{
	if (SoulCountText)
	{
		SoulCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
	}
}
