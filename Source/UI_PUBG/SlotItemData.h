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
    FsSlotItemData(FString _Name, FString _Category, FString _Description, int _ImageIndex, int _Capacity = 0,  int _Count = 1) : Name(_Name), Category(_Category), Description(_Description), ImageIndex(_ImageIndex), Capacity(_Capacity),  Count(_Count) { }

    bool operator==(FsSlotItemData& _Other)
    {
        return this->Name == _Other.Name &&
               this->ImageIndex == _Other.ImageIndex &&
               this->Count == _Other.Count;
    }

    bool operator!=(FsSlotItemData& _Other) { return !(*this == _Other); }

public:
    void Reset()
    {
        this->Name        = "";
        this->Category    = "";
        this->Description = "";
        this->ImageIndex  = 0;
        this->Count       = 0;
    }

    bool IsEmpty()
    {
        return this->Name == "" &&
            this->ImageIndex == 0 &&
            this->Count == 0;
    }

    static FsSlotItemData GetDataFrom(class ABaseInteraction* _pObj);
};