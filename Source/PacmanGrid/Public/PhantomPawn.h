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
enum class EEnemyState{
	Chase, //ghost chases pacman
	Scatter, //ghost walks a default path
	Frightened, //ghost can be eat by pacman
	Idle //ghost in his house
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

private:
	UPROPERTY(VisibleAnywhere)
		class APacmanPawn* Player;

public:
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		APacmanPawn* GetPlayer() const;
	UFUNCTION()
		void SetSpeed(float Speed);
	UFUNCTION()
		virtual AGridBaseNode* GetPlayerRelativeTarget();
	void SetGhostTarget();

	// game instance reference
	UMyGameInstance* GameInstance;

	UPROPERTY(EditAnywhere)
		USoundCue* PacmanDeadSound;
};
