// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopWidget.h"
#include "UI/Shop/ShopItemSellWidget.h"
#include "UI/Shop/ShopItemListWidget.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RestShopItemListWidget();
}

void UShopWidget::AddToItemSellDelegate(const FScriptDelegate& Delegate)
{
	ItemSellWidget->OnItemSell.Add(Delegate);
}

void UShopWidget::UpdateAllBuyButtonState(int32 _)
{
	UE_LOG(LogTemp, Log, TEXT("UpdateAllBuyButtonState"));
	if (GetVisibility() == ESlateVisibility::Visible
		|| GetVisibility() == ESlateVisibility::SelfHitTestInvisible)	// 열려있으면
	{
		//UE_LOG(LogTemp, Log, TEXT("UpdateAllBuyButtonState:Visible"));
		ItemListWidget->UpdateAllBuyButton();	//아이템리스트 위젯에게 구매 버튼 업데이트 하라고 지시
	}
}


void UShopWidget::RestShopItemListWidget()
{
	if (ShopItemList.IsValid())
	{
		ItemListWidget->ResetItemList(ShopItemList.Get());
	}
}
