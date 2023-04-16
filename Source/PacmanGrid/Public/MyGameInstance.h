// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PACMANGRID_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	//score
	UPROPERTY(EditAnywhere)
		int32 Score = 0;

	//increment score
	void IncrementScore();

	//get the score
	UFUNCTION(BlueprintCallable)
		int32 GetScore();


};
