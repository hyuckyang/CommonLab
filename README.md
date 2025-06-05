# CommonLab 플러그인 모음

## 간단 개요

본 문서는 Unreal Engine 프로젝트에서 사용될 수 있는 CommonLab 관련 플러그인 세트에 대한 설명을 제공합니다. 각 플러그인은 게임 개발의 특정 측면을 모듈화하고 관리하는 데 도움을 주기 위해 설계되었습니다. `CommonLab`은 핵심 플레이어별 기능 관리를, `CommonLabLoadingScreen`은 주 로딩 화면을, `CommonLabPreLoadingScreen`은 초기 스플래시 화면 기능을 담당합니다.

## 목차

1.  [CommonLab 플러그인](#1-commonlab-플러그인)
    *   [개요 (CommonLab)](#개요-commonlab)
    *   [주요 기능 (CommonLab)](#주요-기능-commonlab)
    *   [주요 구성 요소 (CommonLab)](#주요-구성-요소-commonlab)
        *   [`UCommonLabSubsystem`](#ucommonlabsubsystem)
        *   [`UCommonLabLocalPlayer`](#ucommonlablocalplayer)
        *   [`ICommonLabSubClassInterface`](#icommonlabsubclassinterface)
        *   [세부 서브 시스템](#세부-서브-시스템)
            *   [`UCommonLabDelegateSubClass`](#ucommonlabdelegatesubclass)
            *   [`UCommonLabActivatableSubClass`](#ucommonlabactivatablesubclass)
            *   [`UCommonLabPointTagSubClass`](#ucommonlabpointtagsubclass)
        *   [Extension](#extension)
            *   [`CommonLabInputActionDecorator`](#commonlabinputactiondecorator)
    *   [사용 방법 (CommonLab)](#사용-방법-commonlab)
2.  [CommonLabLoadingScreen 플러그인](#2-commonlabloadingscreen-플러그인)
    *   [개요 (CommonLabLoadingScreen)](#개요-commonlabloadingscreen)
    *   [주요 기능 (CommonLabLoadingScreen)](#주요-기능-commonlabloadingscreen)
    *   [주요 구성 요소 (CommonLabLoadingScreen)](#주요-구성-요소-commonlabloadingscreen)
    *   [사용 방법 (CommonLabLoadingScreen)](#사용-방법-commonlabloadingscreen)
3.  [CommonLabPreLoadingScreen 플러그인](#3-commonlabpreloadingscreen-플러그인)
    *   [개요 (CommonLabPreLoadingScreen)](#개요-commonlabpreloadingscreen)
    *   [주요 기능 (CommonLabPreLoadingScreen)](#주요-기능-commonlabpreloadingscreen)
    *   [주요 구성 요소 (CommonLabPreLoadingScreen)](#주요-구성-요소-commonlabpreloadingscreen)
    *   [사용 방법 (CommonLabPreLoadingScreen)](#사용-방법-commonlabpreloadingscreen)

---

## 1. CommonLab 플러그인

### 개요 (CommonLab)

`CommonLab` 플러그인은 Unreal Engine 프로젝트 내에서 로컬 플레이어별 서브 시스템 관리를 용이하게 하기 위해 설계되었습니다. 각 로컬 플레이어에 대해 특정 기능들을 모듈화하여 관리할 수 있는 구조를 제공하며, 플레이어 생명주기에 맞춰 이러한 모듈들을 초기화하고 해제하는 기능을 담당합니다.

### 주요 기능 (CommonLab)

*   **로컬 플레이어별 서브 클래스 관리**: `UCommonLabLocalPlayer` 단위로 다양한 서브 클래스를 생성하고 관리합니다.
*   **생명주기 관리**:
    *   플레이어가 추가될 때 (`NotifyPlayerAdded`): 연관된 서브 클래스들을 생성하고 초기화합니다.
    *   플레이어 컨트롤러가 설정될 때: 서브 클래스들을 해제 후 다시 초기화합니다.
    *   플레이어가 제거될 때 (`NotifyPlayerRemoved`): 서브 클래스들을 해제합니다.
    *   플레이어가 파괴될 때 (`NotifyPlayerDestroyed`): 서브 클래스들을 파괴하고 관련 데이터를 정리합니다.
*   **인터페이스 기반 확장**: 서브 클래스들은 `ICommonLabSubClassInterface`를 구현하여 일관된 방식으로 초기화(`Initialize`), 해제(`Released`), 파괴(`Destroyed`) 로직을 처리합니다.
*   **조건부 서브시스템 생성**: `ShouldCreateSubsystem` 함수를 통해 특정 조건(예: 데디케이티드 서버가 아니고, 해당 서브시스템을 상속하는 클래스가 없을 경우)에서만 서브시스템이 생성되도록 제어합니다.
*   **자동 초기화 및 해제**: `UCommonLabSubsystem`의 `Initialize` 및 `Deinitialize` 함수를 통해 플러그인 전체의 서브 클래스들을 관리합니다.

### 주요 구성 요소 (CommonLab)

#### `UCommonLabSubsystem`

플러그인의 핵심 클래스로, 로컬 플레이어와 연관된 서브 클래스들의 생성, 관리, 생명주기를 담당합니다. `UGameInstanceSubsystem`을 상속받아 게임 인스턴스 레벨에서 동작합니다.

#### `UCommonLabLocalPlayer`

이 플러그인이 대상으로 하는 커스텀 로컬 플레이어 클래스입니다. `OnPlayerControllerSet` 델리게이트를 통해 플레이어 컨트롤러 변경 시점을 감지하고, 이에 따라 서브 클래스들의 상태를 갱신합니다.

#### `ICommonLabSubClassInterface`

모든 로컬 플레이어별 서브 클래스가 구현해야 하는 인터페이스입니다. `Initialize`, `Released`, `Destroyed` 함수를 정의하여 각 서브 클래스가 일관된 생명주기 관리 로직을 따르도록 합니다.

#### 세부 서브 시스템

`UCommonLabSubsystem`은 `CreateSubClass` 함수를 통해 다음과 같은 서브 클래스들을 각 `UCommonLabLocalPlayer`에 대해 생성하고 관리합니다.

*   ##### `UCommonLabDelegateSubClass`
    이 서브 클래스는 다양한 델리게이트를 보다 간편하게 사용할 수 있도록 지원합니다. 이름(`FString`)으로 관리되며, 별도의 델리게이트 등록 절차 없이 이름과 함수의 인자를 통해 등록하여 바로 사용할 수 있습니다.

*   ##### `UCommonLabActivatableSubClass`
    이 서브 클래스는 Lyra 샘플 게임에서 사용되는 UI 시스템을 GAS(Gameplay Ability System) 기능 없이도 간단히 사용할 수 있도록 하는 UI 시스템입니다. `GameTag`를 등록하여 UI Layer를 설정할 수 있으며, HUD 및 메뉴 UI 사용에 유용합니다. 특히 등록된 `OnverOverlap` (또는 유사한 이벤트)을 통해 `UCommonLabActivatableSubClass` 아래에서 관리되는 위젯 간 통신의 용이성을 목표로 합니다.

*   ##### `UCommonLabPointTagSubClass`
    이 서브 클래스는 `GameTag`를 활용하여 특정 `GameTag`를 보유한 위젯을 관리하고 간편하게 찾는 기능을 제공합니다. 현재는 등록된 위젯의 Visible 여부를 확인하고 실제 활성화/비활성화를 실행합니다.

#### Extension

*   ##### `CommonLabInputActionDecorator`
    ###### 개요
    CommonUI의 `CommonInputActionDataTable`을 기반으로 플랫폼 아이콘 출력을 지원하는 RichText Decorator입니다.

    <img src="https://github.com/user-attachments/assets/c36f0ac7-4c83-469d-ab5a-3f2924a15a2a" width="55%"/>

    `img id="id"` 규격으로 `CommonInputActionData`를 기반으로 Rich Text에 키 아이콘을 출력이 가능합니다.

    <img src="https://github.com/user-attachments/assets/e2400d44-cfb3-41d6-bff6-c95578503595" width="55%"/>

    런타임(게임 플레이) 도중, 입력 값(키보드/게임패드 등)을 감지하여 실시간으로 키 아이콘이 변경됩니다.

### 사용 방법 (CommonLab)

1.  `CommonLabGameInstance`를 `GameInstance`에 등록합니다. 혹은 기존 사용 중인 `GameInstance`에 `CommonLabGameInstance`와 동일하게 구성합니다.
2.  `CommonLabPlayerController`를 사용합니다. 혹은 기존 사용 중인 `PlayerController`에 `CommonLabPlayerController`와 동일하게 구성합니다.
3.  `CommonLabLocalPlayer`를 등록 (프로젝트 세팅 -> 일반 세팅 -> 로컬 플레이어 클래스) 합니다. 혹은 기존 사용 중인 `LocalPlayer`에 `CommonLabLocalPlayer`와 동일하게 구성합니다.
4.  `CommonGameViewportClient`를 설정 (프로젝트 세팅 -> 일반 세팅 -> 게임 뷰포트 클라이언트 클래스) 합니다. **이것은 반드시 설정해야 합니다.**
5.  프로젝트 세팅 -> `CommonLabActivatableSetting`에서 기본적인 `Activatable Stack Tags`를 설정합니다. (중요)
6.  `UCommonLabBPFunctionLibrary`를 참조 및 사용하면 됩니다.

---

## 2. CommonLabLoadingScreen 플러그인

### 개요 (CommonLabLoadingScreen)

`CommonLabLoadingScreen` 플러그인은 게임의 메인 로딩 화면 기능을 제공하기 위해 설계되었습니다. 레벨 전환이나 초기 게임 로딩 시 최초 페이드 인/아웃 및 로딩 위젯을 출력합니다.
기본적으로 `CommonLoadingScreen`과 유사하나, 지속적으로 상태를 감지(`ILoadingProcessInterface` 등)하는 방식과 달리, 특정 시점에 페이드 연출과 로딩 화면 출력을 명확하게 실행합니다.

### 주요 기능 (CommonLabLoadingScreen)

*   로딩 함수를 정의하여 특정 지점에서 로딩 화면이 시작되도록 명시화합니다.
*   기본적인 페이드 인/아웃 연출을 정의합니다.
*   `UCommonLabLoadingScreenSetting` 또는 `OnTravelBySubClass` 함수 등을 통해 다양한 로딩 위젯을 출력할 수 있도록 지원합니다.
*   `ICommonLabLoadingShouldInterface`를 통해 로딩 스크린이 닫힐지 여부를 체크합니다.

### 주요 구성 요소 (CommonLabLoadingScreen)

*   `FadeProcess` (페이드 연출)와 `LoadingProcess` (로딩 위젯)를 이원화하고 기능별로 분리합니다.
*   `ICommonLabLoadingShouldInterface`, `ICommonLabLoadingWidgetInterface`를 통해 로딩 스크린의 실행 여부, 로딩 스크린에 입력값 전달 등을 유연하게 적용합니다.

### 사용 방법 (CommonLabLoadingScreen)

1.  프로젝트 세팅 -> `CommonLabLoadingScreenSetting`에서 `ZOrder`와 기본 로딩 위젯을 설정합니다.
2.  `CommonLabLoadingBPFunctionLibrary`를 참조하여 사용합니다.

---

## 3. CommonLabPreLoadingScreen 플러그인

### 개요 (CommonLabPreLoadingScreen)

`CommonLabPreLoadingScreen` 플러그인은 애플리케이션 시작 직후, 엔진의 주요 시스템이 완전히 초기화되기 전에 매우 간단한 로딩 화면 또는 스플래시 스크린을 표시하기 위해 설계되었습니다. 주 로딩 화면(`CommonLabLoadingScreen`)이 표시되기 전 사용자에게 즉각적인 시각적 피드백을 제공하는 역할을 합니다.

### 주요 기능 (CommonLabPreLoadingScreen)

*   게임 실행 초기에 간단한 로고 혹은 이미지를 표시합니다.
*   최소한의 리소스(`Image`, `Material` 등)를 이용하여 적용합니다.

### 주요 구성 요소 (CommonLabPreLoadingScreen)

*   **PreLoadingScreen 모듈**: 엔진 초기화 단계에 최대한 일찍 로드되어 스크린을 표시하는 로직을 포함하는 게임 모듈 또는 플러그인 모듈입니다.
*   **간단한 이미지/텍스트 표시 기능**: 복잡한 UI 시스템(`UMG` 등)에 의존하지 않고, `Slate` 또는 플랫폼별 API를 사용하여 기본적인 시각 요소를 표시합니다.
*   **설정**: 표시할 이미지 파일 경로 등을 간단한 설정 파일이나 코드를 통해 지정합니다.

### 사용 방법 (CommonLabPreLoadingScreen)

1.  `CommonLab.uplugin` 파일 내 `Modules` 섹션에서 `CommonLabPreLoadingScreen` 모듈을 찾아 `EnabledByDefault` 값을 `true`로 설정합니다. (참고: 이 값은 기본적으로 `false`로 설정되어 있으므로, 플러그인 활성화를 위해 변경이 필요할 수 있습니다.)
    ```json
    // CommonLab.uplugin 예시
    // ...
    "Modules": [
      // ... 다른 모듈들 ...
      {
        "Name": "CommonLabPreLoadingScreen",
        "Type": "ClientOnly",
        "LoadingPhase": "PreLoadingScreen",
        "EnabledByDefault": true // 이 값을 true로 변경
      }
    ],
    // ...
    ```
2.  프로젝트 세팅 -> `CommonLabPreLoadingScreenSetting`에서 출력할 이미지를 설정합니다.