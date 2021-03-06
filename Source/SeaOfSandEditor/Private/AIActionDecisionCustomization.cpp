
#include "AIActionDecisionCustomization.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Input/SSearchBox.h"

#define LOCTEXT_NAMESPACE "FAIActionDecisionCustomization"

TSharedPtr<FString> FAIActionDecisionCustomization::InitWidgetContent()
{
	TSharedPtr<FString> InitialValue = MakeShareable(new FString(LOCTEXT("DataTable_None", "None").ToString()));;

	FName RowName;
	const FPropertyAccess::Result RowResult = RowNamePropertyHandle->GetValue(RowName);
	RowNames.Empty();

	/** Get the properties we wish to work with */
	UDataTable* DataTable = NULL;
	DataTablePropertyHandle->GetValue((UObject*&)DataTable);

	if (DataTable != NULL)
	{
		/** Extract all the row names from the RowMap */
		for (TMap<FName, uint8*>::TConstIterator Iterator(DataTable->RowMap); Iterator; ++Iterator)
		{
			/** Create a simple array of the row names */
			TSharedRef<FString> RowNameItem = MakeShareable(new FString(Iterator.Key().ToString()));
			RowNames.Add(RowNameItem);

			/** Set the initial value to the currently selected item */
			if (Iterator.Key() == RowName)
			{
				InitialValue = RowNameItem;
			}
		}
	}

	/** Reset the initial value to ensure a valid entry is set */
	if (RowResult != FPropertyAccess::MultipleValues)
	{
		FName NewValue = FName(**InitialValue);
		RowNamePropertyHandle->SetValue(NewValue);
	}

	return InitialValue;
}

void FAIActionDecisionCustomization::CustomizeHeader(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	this->StructPropertyHandle = InStructPropertyHandle;

	if (StructPropertyHandle->HasMetaData(TEXT("RowType")))
	{
		const FString& RowType = StructPropertyHandle->GetMetaData(TEXT("RowType"));
		RowTypeFilter = FName(*RowType);
	}

	HeaderRow
		.NameContent()
		[
			InStructPropertyHandle->CreatePropertyNameWidget(FText::GetEmpty(), FText::GetEmpty(), false)
		];
}

void FAIActionDecisionCustomization::CustomizeChildren(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	/** Get all the existing property handles */
	DataTablePropertyHandle = InStructPropertyHandle->GetChildHandle("DataTable");
	RowNamePropertyHandle = InStructPropertyHandle->GetChildHandle("RowName");

	if (DataTablePropertyHandle->IsValidHandle() && RowNamePropertyHandle->IsValidHandle())
	{
		/** Queue up a refresh of the selected item, not safe to do from here */
		StructCustomizationUtils.GetPropertyUtilities()->EnqueueDeferredAction(FSimpleDelegate::CreateSP(this, &FAIActionDecisionCustomization::OnDataTableChanged));

		/** Setup Change callback */
		FSimpleDelegate OnDataTableChangedDelegate = FSimpleDelegate::CreateSP(this, &FAIActionDecisionCustomization::OnDataTableChanged);
		DataTablePropertyHandle->SetOnPropertyValueChanged(OnDataTableChangedDelegate);

		/** Construct a combo box widget to select from a list of valid options */
		StructBuilder.AddCustomRow(LOCTEXT("DataTable_RowName", "Preset"))
			.NameContent()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("DataTable_RowName", "Preset"))
			//.Font(StructCustomizationUtils.GetRegularFont())
			]
		.ValueContent()
			.MaxDesiredWidth(0.0f) // don't constrain the combo button width
			[
				SAssignNew(RowNameComboButton, SComboButton)
				.ToolTipText(this, &FAIActionDecisionCustomization::GetRowNameComboBoxContentText)
			.OnGetMenuContent(this, &FAIActionDecisionCustomization::GetListContent)
			.OnComboBoxOpened(this, &FAIActionDecisionCustomization::HandleMenuOpen)
			.ContentPadding(FMargin(2.0f, 2.0f))
			.ButtonContent()
			[
				SNew(STextBlock)
				.Text(this, &FAIActionDecisionCustomization::GetRowNameComboBoxContentText)
			]
			];
	}
}

void FAIActionDecisionCustomization::HandleMenuOpen()
{
	FSlateApplication::Get().SetKeyboardFocus(SearchBox);
}

TSharedRef<SWidget> FAIActionDecisionCustomization::GetListContent()
{
	SAssignNew(RowNameComboListView, SListView<TSharedPtr<FString> >)
		.ListItemsSource(&RowNames)
		.OnSelectionChanged(this, &FAIActionDecisionCustomization::OnSelectionChanged)
		.OnGenerateRow(this, &FAIActionDecisionCustomization::HandleRowNameComboBoxGenarateWidget)
		.SelectionMode(ESelectionMode::Single);

	// Ensure no filter is applied at the time the menu opens
	OnFilterTextChanged(FText::GetEmpty());

	if (CurrentSelectedItem.IsValid())
	{
		RowNameComboListView->SetSelection(CurrentSelectedItem);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(SearchBox, SSearchBox)
			.OnTextChanged(this, &FAIActionDecisionCustomization::OnFilterTextChanged)
		]

	+ SVerticalBox::Slot()
		.FillHeight(1.f)
		[
			SNew(SBox)
			.MaxDesiredHeight(600)
		[
			RowNameComboListView.ToSharedRef()
		]
		];
}

void FAIActionDecisionCustomization::OnDataTableChanged()
{
	CurrentSelectedItem = InitWidgetContent();
	if (RowNameComboListView.IsValid())
	{
		RowNameComboListView->SetSelection(CurrentSelectedItem);
		RowNameComboListView->RequestListRefresh();
	}
}

TSharedRef<ITableRow> FAIActionDecisionCustomization::HandleRowNameComboBoxGenarateWidget(TSharedPtr<FString> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return
		SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
		[
			SNew(STextBlock).Text(FText::FromString(*InItem))
		];
}

FText FAIActionDecisionCustomization::GetRowNameComboBoxContentText() const
{
	FString RowNameValue;
	const FPropertyAccess::Result RowResult = RowNamePropertyHandle->GetValue(RowNameValue);
	if (RowResult != FPropertyAccess::MultipleValues)
	{
		TSharedPtr<FString> SelectedRowName = CurrentSelectedItem;
		if (SelectedRowName.IsValid())
		{
			return FText::FromString(*SelectedRowName);
		}
		else
		{
			return LOCTEXT("DataTable_None", "None");
		}
	}

	return LOCTEXT("MultipleValues", "Multiple Values");
}

void FAIActionDecisionCustomization::OnSelectionChanged(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo)
{
	if (SelectedItem.IsValid())
	{
		CurrentSelectedItem = SelectedItem;
		FName NewValue = FName(**SelectedItem);
		RowNamePropertyHandle->SetValue(NewValue);

		// Close the combo
		RowNameComboButton->SetIsOpen(false);
	}
}

void FAIActionDecisionCustomization::OnFilterTextChanged(const FText& InFilterText)
{
	FString CurrentFilterText = InFilterText.ToString();

	FName RowName;
	const FPropertyAccess::Result RowResult = RowNamePropertyHandle->GetValue(RowName);
	RowNames.Empty();

	/** Get the properties we wish to work with */
	UDataTable* DataTable = nullptr;
	DataTablePropertyHandle->GetValue((UObject*&)DataTable);

	TArray<FString> AllRowNames;
	if (DataTable != nullptr)
	{
		for (TMap<FName, uint8*>::TConstIterator Iterator(DataTable->RowMap); Iterator; ++Iterator)
		{
			FString RowString = Iterator.Key().ToString();
			AllRowNames.Add(RowString);
		}

		// Sort the names alphabetically.
		AllRowNames.Sort();
	}

	for (const FString& RowString : AllRowNames)
	{
		if (CurrentFilterText == TEXT("") || RowString.Contains(CurrentFilterText))
		{
			TSharedRef<FString> RowNameItem = MakeShareable(new FString(RowString));
			RowNames.Add(RowNameItem);
		}
	}

	RowNameComboListView->RequestListRefresh();
}

bool FAIActionDecisionCustomization::ShouldFilterAsset(const struct FAssetData& AssetData)
{
	if (!RowTypeFilter.IsNone())
	{
		const UDataTable* DataTable = Cast<UDataTable>(AssetData.GetAsset());
		if (DataTable->RowStruct && DataTable->RowStruct->GetFName() == RowTypeFilter)
		{
			return false;
		}
		return true;
	}
	return false;
}

#undef LOCTEXT_NAMESPACE