﻿// Copyright Chronicler.


#include "GameMode/ModularWorldSettings.h"

#include "EngineUtils.h"
#include "ModularGameplayExperiencesLogs.h"
#include "Engine/AssetManager.h"
#include "GameFramework/PlayerStart.h"
#include "Misc/UObjectToken.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModularWorldSettings)

AModularWorldSettings::AModularWorldSettings(const FObjectInitializer& ObjectInitializer)
{
}

FPrimaryAssetId AModularWorldSettings::GetDefaultGameplayExperience() const
{
	FPrimaryAssetId Result;
	if (DefaultGameplayExperience.IsNull())
	{
		return Result;
	}

	Result = UAssetManager::Get().GetPrimaryAssetIdForPath(DefaultGameplayExperience.ToSoftObjectPath());
	if (!Result.IsValid())
	{
		UE_LOG(LogModularGameplayExperiences, Error,
			TEXT("%s.DefaultGameplayExperience is %s but that failed to resolve into an asset ID (you might need to add a path to the Asset Rules in your game feature plugin or project settings"),
			*GetPathNameSafe(this), *DefaultGameplayExperience.ToString());
	}
	return Result;
}


#if WITH_EDITOR
void AModularWorldSettings::CheckForErrors()
{
	Super::CheckForErrors();

	FMessageLog MapCheck("MapCheck");

	for (TActorIterator<APlayerStart> PlayerStartIt(GetWorld()); PlayerStartIt; ++PlayerStartIt)
	{
		APlayerStart* PlayerStart = *PlayerStartIt;
		if (IsValid(PlayerStart) && PlayerStart->GetClass() == APlayerStart::StaticClass())
		{
			MapCheck.Warning()
				->AddToken(FUObjectToken::Create(PlayerStart))
				->AddToken(FTextToken::Create(FText::FromString("is a normal APlayerStart, replace with AModularPlayerStart.")));
		}
	}

	//@TODO: Make sure the soft object path is something that can actually be turned into a primary asset ID (e.g., is not pointing to an experience in an unscanned directory)
}
#endif
