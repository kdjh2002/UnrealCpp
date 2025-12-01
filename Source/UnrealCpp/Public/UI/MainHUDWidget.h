// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHudWidget.generated.h"

UENUM(BlueprintType)
enum class EOpenState : uint8
{
	Open	UMETA(DisplayName = "Open"),
	Close	UMETA(DisplayName = "Close"),
};
/**
 * 
 */
UCLASS()
class UNREALCPP_API UMainHudWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void OpenInventory();

	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void CloseInventory();

	inline EOpenState GetOpenState() const { return OpenState; }

protected:
	// meta = (BindWidget)
	// ���� �������Ʈ�� ������ �� Ŭ������ ������ ���ε��ϰڴٶ�� �ǹ�(=���� ���� �Ŷ�� ����)
	// ���� �������Ʈ�� ������� �� Ŭ������ �������� �ݵ�� ���ƾ� �Ѵ�.(��ҹ��ڵ� ���ƾ� �Ѵ�)	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UResourceBarWidget> HealthBar = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UResourceBarWidget> StaminaBar = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<class UInventoryWidget> Inventory = nullptr;

private:
	EOpenState OpenState = EOpenState::Close;
};