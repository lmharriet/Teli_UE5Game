// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TLUserWidget.h"

void UTLUserWidget::Init()
{

}

void UTLUserWidget::ShutDown()
{

}

const void UTLUserWidget::SetWidgetName(const FName& InWidgetName)
{
	WidgetName = InWidgetName;
}

const FName& UTLUserWidget::GetWidgetName()
{
	return WidgetName;
}