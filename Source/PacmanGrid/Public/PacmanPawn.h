// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridPawn.h"
#include "GridGenerator.h"
#include "GridPlayerController.h"
#include "GridBaseNode.h"
#include "MyGameInstance.h"
#include "PhantomPawn.h"
#include "PacmanPawn.generated.h"

/**
 * 
 */

class USoundCue;

UCLASS()
class PACMANGRID_API APacmanPawn : public AGridPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APacmanPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetVerticalInput(float AxisValue) override;
	virtual void SetHorizontalInput(float AxisValue) override;
	// called on left mouse click (binding)
	virtual	void OnClick() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PowerModeOn();
	void PowerModeOff();
	void ClearTimer();

	FTimerHandle PowerModeTimer;
	FTimerHandle FlashGhostTimer;
	FTimerHandle PacmanDeadTimerHandle;
	FTimerHandle GhostRespawnTimer1;
	FTimerHandle GhostRespawnTimer2;
	FTimerHandle GhostRespawnTimer3;
	FTimerHandle GhostRespawnTimer4;
	// game instance reference
	UMyGameInstance* GameInstance;

	int GetNumberOfGhostsKilled();
	void SetNumberOfGhostsKilled(int n);

	void RespawnStartingPosition();

	UPROPERTY(VisibleAnywhere)
	APhantomPawn* CurrentPreferredGhost;

	void SetNextPreferredGhost();
	void ResetPreferredGhost();

	//pacman dead animation
	void OnPacmanDead();

protected:
	virtual void BeginPlay() override;
	virtual void HandleMovement() override;
	virtual void SetTargetNode(AGridBaseNode* Node) override;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnNodeReached() override;
private: 
	UPROPERTY(EditAnywhere, Category="Audio")
		USoundCue* ConsumptionSound;
	UPROPERTY(EditAnywhere, Category = "Audio")
		USoundCue* EatingPowerSound;
	UPROPERTY(EditAnywhere, Category = "Audio")
		USoundCue* PacmanDeadSound;

	UPROPERTY(VisibleAnywhere)
		class APhantomPawn* Blinky;
	UPROPERTY(VisibleAnywhere)
		class APhantomPawn* Inky;
	UPROPERTY(VisibleAnywhere)
		class APhantomPawn* Pinky;
	UPROPERTY(VisibleAnywhere)
		class APhantomPawn* Clyde;

	int NumberOfGhostsKilled = 0;
};
