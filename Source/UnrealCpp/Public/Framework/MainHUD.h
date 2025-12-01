// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/MainHudWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPP_API AMainHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	inline UMainHudWidget* GetMainWidget() const { return MainWidgetInstance; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> MainWidgetClass = nullptr;

	//인벤토리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UMainHudWidget> MainWidgetInstance = nullptr;

	
};
