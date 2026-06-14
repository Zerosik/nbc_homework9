# nbc_homework9
네트워크 기초 및 숫자 야구 게임 구현

프로젝트명은 숫자야구의 원제를 따 Bulls and cows로 하였음. 이후 프로젝트 코드명은 BNC로 시작하도록 작성함.

주어진 강의를 기반으로 프로젝트 작성 후
과제 요구사항을 추가로 덧붙이는 식으로 진행함.


ABNCGameModeBase
숫자야구 게임의 핵심 로직 규칙을 담당.

ABNCGameState
플레이어의 입력 제한시간을 부분적으로 담당.

ABNCPlayerController
플레이어의 입력과 화면에 출력될 위젯을 제어.

ABNCPlayerState
각 플레이어의 닉네임, 입력횟수, 입력 최대횟수 등을 저장.

주요 기능 구현 과정

필수 구현 내용의 경우, 강의와 비슷함.

**턴 제어 기능**

ABNCGameModeBase내에 현재 접속한 플레이어컨트롤러를 저장한 TArray와 현재 어떤 플레이어의 턴인지 결정하는 int32 Index값을 추가.
   
매번 게임이 시작할 때 TArray를 Algo::RandomShuffle 매서드를 이용해 순서를 뒤섞고 Index를 0으로 초기화.
   
플레이어의 채팅 입력이 숫자야구 정답 제출인지 확인할때, 현재 플레이어인지 체크하는 내용을 추가.
   
   <img width="527" height="152" alt="image" src="https://github.com/user-attachments/assets/879df9ce-0c9d-4bf7-8d33-61cd694aafab" />


 **실시간 타이머 위젯:** 화면에 남은 시간이 줄어드는 UI를 띄웁니다.
 
**시간 동기화**

PlayerState에서 시간 담당후 매 게임, 매 턴이 변경될때 GameMode에서 남은 시간을 초기화하는 함수를 호출하는 방식으로 구현함.

화면 상단에 Textbox를 띄우는 위젯을 추가하고 PlayerController에 적용.

PlayerState에는 남은 시간을 알 수 있는 float TimeLimit, float TimeElapsed를 추가하고 Replicated 설정.

GameMode에서 시간을 초기화 할 때, 동시에 같은 시간으로 Timer를 설정하여 입력 제한시간이 만료될 때 플레이어의 턴을 종료시키고 입력 기회를 차감시키는 동시에 다음 플레이어의 턴으로 변경함.
  
   
**입력 차단**

채팅 기능이 존재하여 완전한 입력 차단은 하지않았음.
다만 입력 제한시간과 각 플레이어의 입력 기회(턴제방식)을 추가하면서 정답 입력을 체크하는 로직에서 아무때나 입력한 답안이 처리되지 않도록 하였음. 

**턴 내에 숫자 야구(채팅을 입력)하지 않을 시 기회 소진 기능**

PlayerController 리스트와 int32 index값 두개로 플레이어의 턴을 구현함.

매 플레이어가 입력 등 기회 차감시 index값을 1씩 늘리며, 해당 값을 통해 현재 입력 가능한 플레이어를 결정.
또 해당 값이 변경될 때 UI에 어떤 플레이어의 턴인지 알 수 있도록 값을 갱신함.
   
   
