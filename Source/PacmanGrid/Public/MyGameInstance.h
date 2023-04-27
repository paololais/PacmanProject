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
		int Score = 0;

	UFUNCTION(BlueprintCallable)
		void setScore(int newvalue);

	UFUNCTION(BlueprintCallable)
		int getScore();



	//vite pacman
	UPROPERTY(EditAnywhere)
		int Lives = 3;

	UFUNCTION(BlueprintCallable)
		int GetLives();

	//decrement lives -1
	void DecrementLives();

};
