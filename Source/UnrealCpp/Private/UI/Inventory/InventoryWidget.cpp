// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CloseButton)
	{
		//CloseButton
		CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnCloseClicked);

	}
}

void UInventoryWidget::OnCloseClicked()
{
	OnInventoryCloseRequested.Broadcast();	//닫힘버튼이 눌려졌음을 알리기만 함
}