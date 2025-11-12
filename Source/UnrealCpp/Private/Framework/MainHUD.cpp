// Fill out your copyright notice in the Description page of Project Settings.


#include "FrameWork/MainHUD.h"
#include "Blueprint/UserWidget.h"


void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

		if (MainWidgetClass)
		{
			UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), MainWidgetClass);
			if (widget)
				{
					widget->AddToViewport();
				}
		}
}

