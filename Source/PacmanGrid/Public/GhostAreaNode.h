// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridBaseNode.h"
#include "PhantomPawn.h"
#include "GhostAreaNode.generated.h"

/**
 * 
 */
UCLASS()
class PACMANGRID_API AGhostAreaNode : public AGridBaseNode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGhostAreaNode();

	bool CanEnter();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APhantomPawn> GhostClass;
	UPROPERTY(VisibleAnywhere)
		APhantomPawn* Ghost;
};
