// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopItemBuyWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"
#include "Components/Overlay.h"
#include "Components/Button.h"
#include "Data/ItemDataAsset.h"
#include "Player/InventoryOwner.h"

void UShopItemBuyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemCount)
	{
		ItemCount->SetHintText(FText::AsNumber(MinimumBuyCount));
		ItemCount->OnTextChanged.AddDynamic(this, &UShopItemBuyWidget::OnItemCountTextChanged);		// 변경이 있을 때
		ItemCount->OnTextCommitted.AddDynamic(this, &UShopItemBuyWidget::OnItemCountTextCommitted);	// 변경을 확정했을 때(엔터친 후, 포커스를 잃은 후)
	}

	if (ItemBuy)
	{
		ItemBuy->OnClicked.AddDynamic(this, &UShopItemBuyWidget::OnBuyButtonClicked);
	}
}

void UShopItemBuyWidget::SetItemData(const UItemDataAsset* InItemData, int32 InStockCount)
{
	ItemIcon->SetBrushFromTexture(InItemData->ItemIcon);
	ItemName->SetText(InItemData->ItemName);
	ItemPrice->SetText(FText::AsNumber(InItemData->ItemPrice));
	ItemStockCount->SetText(FText::AsNumber(InStockCount));
	ItemDescription->SetText(InItemData->ItemDescription);;

	StockCount = InStockCount;
	BuyCount = MinimumBuyCount;
	ItemData = InItemData;

	UpdateBuyButton();
}

void UShopItemBuyWidget::OnItemCountTextChanged(const FText& Text)
{
	UE_LOG(LogTemp, Log, TEXT("Changed : %s"), *Text.ToString());

	FString number =Text.ToString();
	if (number.IsNumeric())
	{
		BuyCount = FMath::Clamp(FCString::Atoi(*number),MinimumBuyCount,StockCount); //1~StockCount //갯수는 1~StockCount 사이
		ItemCount->SetText(FText::AsNumber(BuyCount));

		//BuyCount* ItemData->ItemPrice;
		UpdateBuyButton();
	}
}

void UShopItemBuyWidget::OnItemCountTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	UE_LOG(LogTemp, Log, TEXT("Changed : %s"), *Text.ToString());

	FString number = Text.ToString();
	if (number.IsNumeric())
	{ 
	int32 count = FCString::Atoi(*number);
	ItemCount->SetText(FText::AsNumber(count));
	}
	else
	{
		ItemCount->SetText(FText::AsNumber(MinimumBuyCount));
	}

}

void UShopItemBuyWidget::OnBuyButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("구매 버튼 클릭"));

}

void UShopItemBuyWidget::UpdateBuyButton() const
{
	//BuyCount* ItemData->ItemPrice;
	APawn* Player = GetOwningPlayerPawn();
	if (Player->Implements<UInventoryOwner>())
	{
		//플레이어의 돈 상태 확인해서 버튼 활성화/비활성화
		bool hasEnoughMoney = IInventoryOwner::Execute_HasEnoughMoney(Player, BuyCount * ItemData->ItemPrice);
		ItemBuy->SetIsEnabled(hasEnoughMoney);
	}
}
