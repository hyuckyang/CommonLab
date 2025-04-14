# CommonLab
Unreal CommonUI (플러그인) 기반으로 제작 된 실험용 플러그인 입니다 </br>
Unreal Lyra 샘플에 존재하는 CommonGame (플러그인) 을 참조하여 새롭게 작성된 코드 입니다. </br>
</br>
CommonLab 은 CommonGame 와 같이 LocalPlayer 이 생성, 변경, 삭제를 통한 생명주기를 기본으로 합니다. </br>
CommonLab 은 언급된 생명주기를 기본으로 ICommonLabSubClassInterface 를 상속받은 객체들을 생성, 각 객체들이 특정 기능을 하겠금 유도하였습니다. </br>
</br>
</br>
[각 요소 의 설정법]
</br>
</br>
CommonLabGameInstance  </br>
> 위의 Instance 를 사용하지 못한다면 (원하는) GameInstance 클래스 중 CommonLabGameInstance 와 동일하게 구성합니다. (함수) </br>
</br>

CommonLabPlayerController </br>
> 위의 Controller 를 사용하지 못한다면 (원하는) PlayerController 에 CommonLabPlayerController 와 동일하게 구성합니다. (함수) </br>
</br>

CommonLabLocalPlayer </br>
> 프로젝트 셋팅 -> 일반 셋팅 -> 로컬 플레이어 클래스 에 설정합니다. CommonLabLocalPlayer 는 반드시 필요하여, (사용중인) LocalPlayer 에 상속받은 형식으로라도 사용되어야 합니다.
 </br>

CommonGameViewportClient </br>
> 프로젝트 셋팅 -> 일반 셋팅 -> 게임 뷰포트 클라이언트 클래스 에 설정합니다. CommonLab 은 CommonUI 기반이기에 CommonGameViewportClient 셋팅은 반드시 필요하며 (사용중인) GameViewportClient 에 상속받은 형식으로라도 사용되어야 합니다.
</br>
</br>


</br>
</br>

[CommonLab Subsystem 를 통해 관리되는 객체(클래스)]
</br>
</br>
CommonLabDelegate </br>
> 개요</br>
>> C++ 전용으로 가변인자 Delegate 를 RunTime 에서 생성 및 활용하여 통신하는 클래스 (자세한 설명 및, 활용법 작성 중)
</br>
</br>


</br>
</br>

[Extention -> CommonLab Subsystem 이 관리하지 않는 객체(클래스)]
</br>
</br>
CommonLabInputActionDecorator
> 개요</br>
>> CommonUI 의 CommonInputActionDataTable 를 기반으로 플랫폼 아이콘이 출력을 지원하는 RichText Decorator  (자세한 설명 및, 활용법 작성 중) </br>
>> ![](img src="https://github.com/user-attachments/assets/d6096971-a735-4413-8f71-fed61067bb05" witdh="100%")
</br>













