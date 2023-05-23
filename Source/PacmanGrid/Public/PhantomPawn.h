// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridPawn.h"
#include "MyGameInstance.h"
#include "PhantomPawn.generated.h"

/**
 * 
 */
class SoundCue;

UENUM()
enum EEnemyState{
	Chase, //ghost chases pacman
	Scatter, //ghost walks a default path
	Frightened, //ghost can be eat by pacman
	Idle, //ghost in his house
	Dead, //ghost eaten by player when in frightened state
};

UCLASS()
class PACMANGRID_API APhantomPawn : public AGridPawn
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	APhantomPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnNodeReached() override;

	UPROPERTY(VisibleAnywhere)
		class APacmanPawn* Player;


private:
	//Indexes overridden: 0 - Blinky, 1 - Pinky, 2 - Inky, 3 - Clyde
	int GhostIndex = 0;

	//tracks sequence point of AlternateScatterChase() 
	int sequencePoints[4] = { 1,1,1,1 };

	//Starting positions
	FVector2D HomePosition[4] = {FVector2D(14,14), FVector2D(14,13), FVector2D(14,11), FVector2D(14,16)};
	//called when spawn or respawn
	FVector2D StartPosition[4] = { FVector2D(17,10), FVector2D(14,13), FVector2D(14,11), FVector2D(14,16) };

public:
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		APacmanPawn* GetPlayer() const;
	UFUNCTION()
		void SetSpeed(float Speed);
	UFUNCTION()
		virtual AGridBaseNode* GetPlayerRelativeTarget();
	void SetGhostTarget();

	FTimerHandle DeadStateTimer;

	// game instance reference
	UMyGameInstance* GameInstance;

	UPROPERTY(EditAnywhere)
		USoundCue* PacmanDeadSound;
	UPROPERTY(EditAnywhere)
		USoundCue* GhostDeadSound;

	//skins
	UPROPERTY(EditAnywhere)
		UMaterialInterface* DefaultSkin;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* VulnerableSkin;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* DeadSkin;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* FlashingSkin;

	//respawn starting position
	virtual void RespawnGhostStartingPosition();

	//go back in assigned position (when eaten by player)
	virtual void GoHome();

	//exit ghost area
	void ExitGhostArea();

	//called when ghost changes state
	void ReverseDirection();
	//FTimerHandle DelayReverse;

	//function called when ghost is in scattered mode
	//every ghost as an assigned corner to go
	virtual void GoToHisCorner();
	
	FTimerHandle TimerHandle;
	
	void ClearTimer();

	void AlternateScatterChase(int Index);

	//StateManager
	UPROPERTY(EditAnywhere, Category = "Ghost State", meta = (DisplayName = "Ghost State"))
		TEnumAsByte<EEnemyState> EEnemyState = Idle;

	void FlashSkin();

	void SetChaseState();
	void SetScatterState();
	void SetFrightenedState();
	void SetIdleState();
	void SetDeadState();

	bool IsChaseState();
	bool IsScatterState();
	bool IsFrightenedState();
	bool IsIdleState();
	bool IsDeadState();

	void UpAndDown();
	
	virtual int MyIndex() const { return GhostIndex; }

	//keeps tracks if ghost is exiting the ghost area (necessary for ghost exit logic)
	UPROPERTY(VisibleAnywhere, Category = "Ghost State")
		bool bIsLeaving = false;

	//counter
	int PointCounter = 0;
	int PointLimit;
	virtual int PointGhostCounter() const { return PointCounter; }
	virtual int PointGhostLimit() const { return PointLimit; }
	virtual void IncrementPointCounter() { PointCounter++; }
	virtual void ResetPointCounter() { PointCounter = 0; }

	//global counter used after pacman loses a life
	int GlobalCounter;
	int GlobalLimits[4] = { 0,7,17,32 };

	void CanExitHouse();
};
