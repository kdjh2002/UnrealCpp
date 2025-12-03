// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Player/InventoryComponent.h"
#include "UI/Inventory/InventoryDragDropOperation.h"
#include "UI/Inventory/TemporarySlotWidget.h"

void UInventorySlotWidget::InitializeSlot(UInventoryComponent* InInventoryComponent, int32 InIndex)
{
	if (InInventoryComponent)
	{
		TargetInventory = InInventoryComponent;
		Index = InIndex;
		SlotData = TargetInventory->GetSlotData(InIndex);
		OnSlotRightClick.BindUFunction(TargetInventory.Get(), "UseItem");	// 인벤토리 컴포넌트에 있는 UseItem과 바인딩
		RefreshSlot();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InInventoryComponent가 nullptr입니다!!!"));
	}
}


void UInventorySlotWidget::RefreshSlot() const
{
	if (SlotData && !SlotData->IsEmpty())
	{
		//슬롯에 아이템이 들어있으면 들어있는 아이템 보여주기 
		ItemIconImage->SetBrushFromTexture(SlotData->ItemData->ItemIcon);
		ItemIconImage->SetBrushTintColor(FLinearColor(1.0f,1.0f,1.0f,1.0f));

		CountText->SetText(FText::AsNumber(SlotData->GetCount()));
		MaxCountText->SetText(FText::AsNumber(SlotData->ItemData->ItemMaxStackCount));

		CountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SeparateText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MaxCountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		//슬롯에 아이템이 없으면 빈 슬롯 보여주기
		ClearSlotWidget();
	}
}

void UInventorySlotWidget::ClearSlotWidget() const
{
	ItemIconImage->SetBrushFromTexture(nullptr);
	ItemIconImage->SetBrushTintColor(FLinearColor::Transparent);
	CountText->SetVisibility(ESlateVisibility::Hidden);
	SeparateText->SetVisibility(ESlateVisibility::Hidden);
	MaxCountText->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UE_LOG(LogTemp, Log, TEXT("DragDetected : %d Slot"), this->Index);

	UInventoryDragDropOperation* DragOp = NewObject<UInventoryDragDropOperation>();

	//기본 데이터 세팅
	DragOp->Index = Index;
	DragOp->ItemData = SlotData->ItemData;
	DragOp->Count = SlotData->GetCount();

	// 비주얼 위젯 만들기
	UTemporarySlotWidget* DragTemporaryWidget = CreateWidget<UTemporarySlotWidget>(
		this, TargetInventory->GetTemporarySlotWidgetClass());
	DragTemporaryWidget->SetItemIconImage(SlotData->ItemData->ItemIcon);
	DragTemporaryWidget->SetCountText(SlotData->GetCount());

	DragOp->DefaultDragVisual = DragTemporaryWidget;


	OutOperation = DragOp;	// NativeOnDrop나 NativeOnDragCancelled를 발동시키기 위해 반드시 필요

	TargetInventory->ClearSlotAtIndex(Index);
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//드레그 앤 드롭이 끝났다.
	UInventoryDragDropOperation* invenOp = Cast<UInventoryDragDropOperation>(InOperation);
	if (invenOp)
	{
		UE_LOG(LogTemp, Log, TEXT("Drop : %d Slot에 %s를 옮기기"),
			Index,
			*(invenOp->ItemData->ItemName.ToString()));

		TargetInventory->SetItemAtIndex(Index, invenOp->ItemData.Get(), invenOp->Count);

		return true;	// 성공적으로 끝났음을 알림
	}
	return false;	// 실패로 끝났음을 알림 -> NativeOnDragCancelled 실행
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// 드래그 앤 드롭이 실패로 끝났다.
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	UInventoryDragDropOperation* invenOp = Cast<UInventoryDragDropOperation>(InOperation);
	if (invenOp)
	{
		UE_LOG(LogTemp, Log,
			TEXT("DragCancelled : 바닥에다가 (%s)아이템을 버려야 한다."),
			*(invenOp->ItemData->ItemName.ToString()));
	}
}


FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))	// 마우스 오른쪽 버튼 눌렸는지 확인
		{
			if (SlotData->ItemData)
			{
				UE_LOG(LogTemp, Log, TEXT("Widget %d Slot : Right click(%s)"), Index, *SlotData->ItemData->ItemName.ToString());
				OnSlotRightClick.ExecuteIfBound(Index);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Widget %d Slot : Right click(empty)"), Index);
			}
			return FReply::Handled();	// 이 마우스 클릭은 완료되었다라고 전달
		}
		else if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))	// 마우스 왼쪽 버튼 눌렸는지 확인
		{
			if (!SlotData->IsEmpty())	// 슬롯에 아이템이 들어있었는지 확인
			{
				return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
			}
		}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);	// 나는 처리안했다. 부모 or 다른 위젯이 처리할거다.
}
