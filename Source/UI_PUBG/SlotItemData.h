#pragma once
#include "CoreMinimal.h"
#include "SlotItemData.generated.h"

/** * \brief ������ ���� ����ü */
USTRUCT()
struct FsSlotItemData
{
    GENERATED_BODY()

public:
    static const FsSlotItemData EmptyData;

    FString Name        = "";
    FString Category    = "";
    FString Description = "";
    int     ImageIndex  = 0;
    int     Capacity    = 0;
    int     Count       = 0;

public:
    FsSlotItemData() = default;

    /**
      * \brief �⺻ ������ ������ ��Ī / �̹��� �ε��� / ���� (UI�Ŵ���)
      * \param _name ������ ��Ī 
      * \param _image_index UI �̹��� �ε��� 
      * \param _count ����
     */
    FsSlotItemData(FString Name, FString Category, FString Description, int ImageIndex, int Capacity = 0,  int Count = 1) : Name(Name), Category(Category), Description(Description), ImageIndex(ImageIndex), Capacity(Capacity),  Count(Count) { }

    bool operator==(const FsSlotItemData& _Other)
    {
        return this->Name        == _Other.Name        &&
               this->Category    == _Other.Category    &&
               this->Description == _Other.Description &&
               this->ImageIndex  == _Other.ImageIndex  &&
               this->Capacity    == _Other.Capacity    &&
               this->Count       == _Other.Count;
    }

    bool operator!=(const FsSlotItemData& Other) { return !(*this == Other); }

public:
    void Reset() { *this = FsSlotItemData::EmptyData; }

    bool IsEmpty() { return *this == FsSlotItemData::EmptyData; }

    static FsSlotItemData GetDataFrom(class ABaseInteraction* pObj);
};