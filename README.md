# PUBG_UE4

## 특징 )

#### 1. 언리얼 기반 PUBG 카피겜 TPS - FPS 전환 기능
#### 2. 차량 - 플레이어간 변경
#### 3. 에임 시 스나이퍼 / 기본 모드 전환
</br>

## 정보 ) &#x2B1B; 제목 및 폴더 위치 / &#x25FE; 상세 내용 / &#x25AA; 사이트 경로
#### &#x2B1B; CSV 파일 구글시트) 프로젝트 폴더 내 0_CSV 통상적으로 데이터임
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; WEAPON_DATA(총기), OTHER_WEAPON_DATA(근접/투척무기), ATTACHMENT_DATA(부착물), VEHICLE_DATA(차량), SOUND_DATA(미정)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; ITEM_DATA(파밍 아이템)
##### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &#x25AA; https://docs.google.com/spreadsheets/d/1RMEf637caAJwg0p0abtzzqoj2tuGjVTUY7crYpbKW7M/edit#gid=0 
#### &#x2B1B; UML 파일 draw.io) 프로젝트 폴더 내 0_Diagrams
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Class(클래스), SEQUENCE(시퀀스), MODULES(모듈 맵), 필요시 클라 서버 분리
##### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &#x25AA; https://app.diagrams.net/
#### &#x2B1B; 구현 예정 트렐로)
##### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &#x25AA; https://trello.com/b/34tauMdQ/ue4pubg
</br>

## 쓰여진 에셋 ) &#x2B1B; 에셋 사이트 경로 / &#x25FE; 에셋 명칭 / $ 표시있을 시 유료
#### &#x2B1B; 언리얼 마켓 플레이스)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Vehicle Variety Pack
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Modular House $
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; FPS Weapon Bundle
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; FX Variety Pack
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; MCO Mocap Basics
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Animation Starter Pack
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Defect Ultimate Props Bundle Vol.1 $
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; M5 VFX Vol2. Fire and Frames $
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Infinity Blade: Effects
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Modular Interface Pack $
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Car Sound Effects Pack. Vol. 1 (100+ SFX) $
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Icon Creator $
#### &#x2B1B; 유니티 에셋 스토어)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; SFX bundle
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; ProgressBars #4
#### &#x2B1B; CUBEBRUSH)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Advance Weapon Pack
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Military Style UI
#### &#x2B1B; 기타 경로)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; crosshairpack_kenney
</br>

## 에셋 경로 ) &#x2B1B; 최상위 폴더 / &#x25FE; 하위 폴더 / &#x25AA; 최하위 폴더
#### &#x2B1B; Animations) 캐릭터 에니메이션 및 에님 블프
#### &#x2B1B; Blueprints) 블루프린트 폴더
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Animations (애님 블프)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Items(각종 아이템)
##### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &#x25AA; Equipment (장비)
##### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &#x25AA; Consumables (섭취가능한 아이템)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Managers (각종 매니저)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; UI (각종 UI)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Vehicles (차량)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Weapons (무기)
##### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &#x25AA; Bullets (탄알)
#### &#x2B1B; Characters) 캐릭터 및 차량 메쉬, 셰이더 등
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; UE4_Mannequin (플레이어)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Vehicles (차량)
#### &#x2B1B; Data) 데이터 테이블
#### &#x2B1B; Items) 총기, 부속품과 같은 각종 아이템들은 상위 폴더로 둘 예정
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Attachment > 부속품
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Recovery > 섭취 가능한 아이템
##### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &#x25AA; HP) 체력 회복 아이템
##### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &#x25AA; Energy) 에너지 회복 아이템
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Equipment    > 장비
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Weapons      > 무기
##### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &#x25AA; Explosives) 투척류 모음
##### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &#x25AA; Melee) 근접 무기
#### &#x2B1B; Meshes) 여기는 주로 인벤토리/플레이어 창에 즉 1회성인 아이템 메쉬들이 들어갈 폴더
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Parachute   > 낙하산
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Pubg_crate > 보급상자
#### &#x2B1B; SFX) 사운드
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Gun_sounds > 총기 소리
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Item_sounds > 아이템 소리
#### &#x2B1B; UI) 이미지 UI 
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Car (자동차 계기판)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Military (인벤토리 슬롯)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Weapon_icon (메인 UI 무기 아이콘)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Weapon_inventory_icon (인벤토리 무기 아이콘)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Progress_bar (바 UI)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Reticle (조준선)
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Weapon_icon (인벤토리 무기 UI)
#### &#x2B1B; VFX) 이펙트 
##### &nbsp;&nbsp;&nbsp;&nbsp; &#x25FC; Items (아이템)
##### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &#x25AA; Crate_box (보급상자)
