// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "UI/MainHudWidget.h"
#include "ActionPlayerController.generated.h"


class UInputAction; //전방선언
class UInputMappingContext; //UInput~ 라는 클래스가 있다고 알려주는 역할
/**
*
* 
 */
UCLASS()
class UNREALCPP_API AActionPlayerController : public APlayerController
{
	GENERATED_BODY()

	//UInputMappingContext a; //전방 선언한 것은 헤더가 없으면 인스턴스를 만들 수 없다.
	//UInputMappingContext* b; //전방 선언한 것의 주소는 저장 할 수 있다.

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

public:
	virtual void SetupInputComponent() override;
	
	//인벤토리창 이용 함수
	void OpenInventoryWidget();
	UFUNCTION()
	void CloseInventoryWidget();

	void InitializeMainHudWidget(UMainHudWidget* InWidget);

	void TestChangeInventoryTarget(UInventoryComponent* NewTarget);

private:
	void OnLookInput(const FInputActionValue& InValue);
	void OnInventoryOnOff();

protected: 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction>IA_Look = nullptr;

	//인벤토리 온오프
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction>IA_InventoryOnOff = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Pitch")
	float VewPitchMin = -40;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Pitch")
	float VewPitchMax = 30;
private:
	int32 GameInputPriority = 1;
	TWeakObjectPtr<UMainHudWidget> MainHudWidget = nullptr;
	TWeakObjectPtr<UInventoryWidget> InventoryWidget = nullptr;
	TWeakObjectPtr<class UInventoryComponent> InventoryComponent = nullptr;

};
