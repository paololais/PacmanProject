// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridBaseNode.h"
#include "GridGenerator.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "GridPawn.generated.h"

class USoundCue;
UCLASS()
class PACMANGRID_API AGridPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGridPawn();
	virtual void SetVerticalInput(float AxisValue);
	virtual void SetHorizontalInput(float AxisValue);

	UFUNCTION(BlueprintCallable)
		void SetNextNodeByDir(FVector InputDir, bool ForceLast = false);


	UFUNCTION(BlueprintCallable)
		AGridBaseNode* GetLastNode() const;
	UFUNCTION(BlueprintCallable)
		AGridBaseNode* GetTargetNode() const;
	UFUNCTION(BlueprintCallable)
		FVector2D GetLastNodeCoords() const;
	UFUNCTION(BlueprintCallable)
		FVector2D GetTargetNodeCoords() const;
	UFUNCTION(BlueprintCallable)
		FVector2D GetNextNodeCoords() const;

	UPROPERTY(EditAnywhere)
		USoundCue* TeleportSound;

	FVector GetLastValidDirection() const;
	FVector GetPreviousDirection() const;
	void SetLastValidDirection(FVector Dir);
	void SetPreviousDirection(FVector Dir);
	void SetLastInputDirection(FVector Dir);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Movement")
		FVector LastInputDirection;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector LastValidInputDirection;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector PreviousDirection;
	
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		bool bCanTeleport;
	//speed
	UPROPERTY(EditAnywhere, Category = "Movement")
		float CurrentMovementSpeed = 600.0f;
	//speed value = 750.0f
	//normal speed - pacman = 80%
	float NormalMovementSpeed = 600.0f;
	//power mode on - 90%
	float PowerSpeed = 700.0f;
	//normal speed - ghosts = 75%
	float NormalGhostSpeed = 562.5f;
	//Tunnel Speed - ghosts - 40%
	float TunnelSpeed = 300.0f;
	//frightened speed - ghosts - 50%
	float FrightenedSpeed = 375.0f;
	//dead speed - ghosts
	float DeadSpeed = 900.0f;;

	UPROPERTY(EditAnywhere)
		float AcceptedDistance = 4.f;

	UPROPERTY(VisibleAnywhere)
		FVector2D CurrentGridCoords;

	UPROPERTY(VisibleAnywhere)
		class ATestGridGameMode* GameMode;

	UPROPERTY(VisibleAnywhere)
		TMap<FVector2D, AGridBaseNode*> GridGenTMap;

	UPROPERTY(VisibleAnywhere)
		AGridGenerator* TheGridGen;

	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		AGridBaseNode* NextNode;
	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		AGridBaseNode* TargetNode;
	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		AGridBaseNode* LastNode;

	virtual void HandleMovement();
	virtual void OnNodeReached();
	void MoveToCurrentTargetNode();
	virtual void SetTargetNode(AGridBaseNode* Node);
	void SetNextNode(AGridBaseNode* Node);
	void SetNodeGeneric(const FVector Dir);

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
		UBoxComponent* Collider;

	UPROPERTY(VisibleAnywhere)
		bool CanMove;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// called on left mouse click (binding)
	virtual void OnClick();

};
