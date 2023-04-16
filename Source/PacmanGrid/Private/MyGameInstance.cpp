// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::IncrementScore()
{
	Score += 10;
}

int32 UMyGameInstance::GetScore()
{
	return Score;
}

int32 UMyGameInstance::GetLives()
{
	return Lives;
}

void UMyGameInstance::DecrementLives()
{
	Lives--;
}
