#pragma once
#include "CoreMinimal.h"
#include "SlotItemData.generated.h"

/** * \brief ������ ���� ����ü */
USTRUCT()
struct FsSlotItemData
{
    GENERATED_BODY()

public:
    FString Name        = "";
    FString Category    = "";
    FString Description = "";
    int     ImageIndex  = 0;
    int     Capacity    = 0;
    int     Count       = 0;

    static const FsSlotItemData EmptyData;

public:
    FsSlotItemData() = default;

    /**
      * \brief �⺻ ������ ������ ��Ī / �̹��� �ε��� / ���� (UI�Ŵ���)
      * \param _name ������ ��Ī 
      * \param _image_index UI �̹��� �ε��� 
      * \param _count ����
     */
    FsSlotItemData(FString Name, FString Category, FString Description, int ImageIndex, int Capacity = 0,  int Count = 1) : Name(Name), Category(Category), Description(Description), ImageIndex(ImageIndex), Capacity(Capacity),  Count(Count) { }

    bool operator==(FsSlotItemData& _Other)
    {
        return this->Name        == _Other.Name        &&
               this->Category    == _Other.Category    &&
               this->Description == _Other.Description &&
               this->ImageIndex  == _Other.ImageIndex  &&
               this->Capacity    == _Other.Capacity    &&
               this->Count       == _Other.Count;
    }

    bool operator!=(FsSlotItemData& Other) { return !(*this == Other); }

public:
    void Reset()
    {
        *this = FsSlotItemData::EmptyData;
    }

    bool IsEmpty()
    {
        FsSlotItemData emptyData = FsSlotItemData::EmptyData;
        return *this == emptyData;
    }

    static FsSlotItemData GetDataFrom(class ABaseInteraction* pObj);
};