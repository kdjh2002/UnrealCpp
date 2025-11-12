// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourceWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPP_API UResourceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateWidget(float InCurrent, float InMax);

protected:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Resource", meta = (BlueprintBaseOnly)
	
};
