// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPP_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	void NativeConstruct();

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TWeakObjectPtr<class UResourceBarWidget> HealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TWeakObjectPtr<class UResourceBarWidget> StaminaBar;
};