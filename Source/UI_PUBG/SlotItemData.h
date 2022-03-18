#pragma once
#include "CoreMinimal.h"
#include "SlotItemData.generated.h"

/** * \brief 아이템 슬롯 구조체 */
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
      * \brief 기본 생성자 아이템 명칭 / 이미지 인덱스 / 개수 (UI매니저)
      * \param _name 아이템 명칭 
      * \param _image_index UI 이미지 인덱스 
      * \param _count 개수
     */
    FsSlotItemData(FString _Name, FString _Category, FString _Description, int _ImageIndex, int _Capacity = 0,  int _Count = 1) : Name(_Name), Category(_Category), Description(_Description), ImageIndex(_ImageIndex), Capacity(_Capacity),  Count(_Count) { }

    bool operator==(FsSlotItemData& _Other)
    {
        return this->Name        == _Other.Name        &&
               this->Category    == _Other.Category    &&
               this->Description == _Other.Description &&
               this->ImageIndex  == _Other.ImageIndex  &&
               this->Capacity    == _Other.Capacity    &&
               this->Count       == _Other.Count;
    }

    bool operator!=(FsSlotItemData& _Other) { return !(*this == _Other); }

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

    static FsSlotItemData GetDataFrom(class ABaseInteraction* _pObj);
};