// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridBaseNode.h"
#include "GridGenerator.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "GridPawn.generated.h"

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

	FVector GetLastValidDirection() const;

	void PowerModeOn();
	void PowerModeOff();

	FTimerHandle PowerModeTimer;

	UFUNCTION(BlueprintCallable)
	AGridBaseNode* GetLastNode() const;
	UFUNCTION(BlueprintCallable)
	AGridBaseNode* GetTargetNode() const;
	UFUNCTION(BlueprintCallable)
	FVector2D GetLastNodeCoords() const;
	UFUNCTION(BlueprintCallable)
	FVector2D GetTargetNodeCoords() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector LastInputDirection;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector LastValidInputDirection;

	void SetLastValidDirection(FVector Dir);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float CurrentMovementSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float NormalMovementSpeed = 200.0f;
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
