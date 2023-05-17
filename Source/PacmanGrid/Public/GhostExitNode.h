// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridBaseNode.h"
#include "Kismet/GameplayStatics.h"
#include "GhostExitNode.generated.h"

/**
 * 
 */
UCLASS()
class PACMANGRID_API AGhostExitNode : public AGridBaseNode
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
