// Copyright Chronicler.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "GameplayTagContainer.h"

#include "ModularPlayerStart.generated.h"

enum class EModularPlayerStartLocationOccupancy
{
	Empty,
	Partial,
	Full
};

/**
 * AModularPlayerStart
 * 
 * Base player starts that can be used by a lot of modes.
 */
UCLASS(Config = Game)
class MODULARGAMEPLAYEXPERIENCES_API AModularPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	AModularPlayerStart(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const FGameplayTagContainer& GetGameplayTags() { return StartPointTags; }

	EModularPlayerStartLocationOccupancy GetLocationOccupancy(AController* const ControllerPawnToFit) const;

	/** Did this player start get claimed by a controller already? */
	bool IsClaimed() const;

	/** If this PlayerStart was not claimed, claim it for ClaimingController */
	bool TryClaim(AController* OccupyingController);

protected:
	/** Check if this PlayerStart is still claimed */
	void CheckUnclaimed();

	/** The controller that claimed this PlayerStart */
	UPROPERTY(Transient)
	TObjectPtr<AController> ClaimingController = nullptr;

	/** Interval in which we'll check if this player start is not colliding with anyone anymore */
	UPROPERTY(EditDefaultsOnly, Category = "Player Start Claiming")
	float ExpirationCheckInterval = 1.f;

	/** Tags to identify this player start */
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer StartPointTags;

	/** Handle to track expiration recurring timer */
	FTimerHandle ExpirationTimerHandle;
};
