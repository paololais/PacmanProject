// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/Actor.h"
//#include "GridPawn.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "MyTeleportBox.generated.h"

/**
 * 
 */

//forward declaration to avoid circular dependencies
class GridPawn;

UCLASS()
class PACMANGRID_API AMyTeleportBox : public ATriggerBox
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;


public:

	AMyTeleportBox();

	UFUNCTION()
		void EnterTeleporter(class AActor* overlappedActor, class AActor* otherActor);

	UFUNCTION()
		void ExitTeleporter(class AActor* overlappedActor, class AActor* otherActor);

	//we are gonna define 2 teleporters, so when we go through one we spawn in the other one
	UPROPERTY(EditAnywhere, Category = "Teleporter")
		AMyTeleportBox* otherTele;

	//this boolean will tell us if we've just teleported from the other teleporter 
	//so that we won't teleport back again, avoiding loops
	UPROPERTY()
		bool teleporting;



};
