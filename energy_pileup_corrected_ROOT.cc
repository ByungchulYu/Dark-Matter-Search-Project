#include "TFile.h" //TFile class를 사용하기 위해서 해당 헤더파일을 include해야 한다
#include "TH1.h" //TH1 Class를 사용하기 위해서 해당 헤더파일을 include해야 한다
#include "TCanvas.h" //TCanvas Class를 사용하기 위해서 해당 파일을 include 해야 한다"

void energy_pileup_corrected_ROOT() 
//root는 파일이름과 같은 이름의 function이 가장 먼저 앞에 있어야 하고 void type을 한 이유는 아무것도 return을 하지 않을 것이기에 그냥 void type을 한 것이다. 이 function을 통해 run2d-yolo.root파일을 읽고, 그 파일에서 ENERGY_FULL파일을 빼는 pileup correction 작업을 직접해보고  TCanvas를 이용해서 결과물(히스토그램)을 볼 것이다
{
	TFile *f = new TFile("run2d-yolo.root", "r"); 
//TFile은 루트파일을 새로 만들거나("create"), 기존의 파일을 업데이트하거나("update"), 덧씌우거나("recreate) 혹은 기존의 파일을 읽을 때("read") 사용하는 root의 built in constructor이다. 현재 이 루트파일의 directory에 run2d-yolo.root라는 기존의 루트파일이 있고 우리는 그것을 읽을 것이기에 TFile의 argument에는 파일이름과 read의 약자인 "r"이 string type 으로 들어가 있다. TFile을 f라는 이름의 pointer로 선언을 하였고 거기에 dynamic memory(another type of pointer) TFile을 할당해주었다.  
	TH1F *energy_full = (TH1F *) gDirectory->Get("MuonCoincidence/ENERGY_FULL");
//gDirectory는 현재 root file의 directory에서 global pointer 역할을 하는 녀석으로, 현재 roof file의 directory에서 새로운 directory를 만들 수 있고 (->mkdir(subdirectory/subsubdirectory)), 하위디렉토리 경로를 이동할 수 있고 (->cd(subdirectory)) 혹은 현재 혹은 하위 루트 디렉토리의 다른 파일을 가져올 수 도 있다 (->Get(file경로/file name)). 현재 root file의 directory에서sub-directory안에 MuonCoincidence에 있는 ENERGY_FULL파일을 가져올것이고 이 파일의 데이터 타입이 float타입을 갖고 있는 1차원 히스토그램이므로 TH1F 의 constructor 를 사용할것인데gDirectory->Get을 사용하여 파일을 가져오면 이녀석은 TObject type으로 파일을 가져오므로, 타입을 TH1F로 바꿔줘야 하기때문에 (TH1F *)을 넣어준 것이다. 마지막으로 line5처럼 할당되는것과 할당을 받는것 모두 pointer로 사용하였다.
	TH1F *energy_d = (TH1F *) gDirectory->Get("MuonCoincidence/ENERGY_D"); //상동
	TH1F *energy_s1 = (TH1F *) gDirectory->Get("MuonCoincidence/ENERGY_S1"); //상동
	TH1F *energy_s2 = (TH1F *) gDirectory->Get("MuonCoincidence/ENERGY_S2"); //상동


	TH1F *energy_pileup_corrected = (TH1F *) energy_full->Clone(); 
//새로운 TH1F Type의 히스토그램을 만드는 과정으로, pileup correction을 한 히스토그램을 만들어볼 것이다. 우선 energy_pileup_corrected 메모리 안에 energy_full을 복사를 했는데, 이유는 간단하다 energy_full에서 energy_d를 빼고 energy_s1과 energy_s2를 더하는과정을 할 것인데 원본파일인energy_full을 손상시키지 않으면서 결과를 도출하고 싶기에 원본파일을 새로운 TH1F type의 energy_pileup_corrected에 복사를 하고 거기에서 더하고 빼고를 하려는 것이 목적이다.
	energy_pileup_corrected->Add(energy_d, -1); // energy_d를 빼는 과정인데, 이때 add 기능에 -1을 넣어줌으로써 -1배 한것을 더하는 것이니 빼는 것과 같은 일을 한 것이다
	energy_pileup_corrected->Add(energy_s1, 1); // energy_s1을 더하는 과정으로 이때 add기능에 1을 넣어줌으로써 1배 한것을 더하는 것이 된다.
	energy_pileup_corrected->Add(energy_s2, 1); // energy_s2를 더하는 과정으로 이때 add기능에 1을 넣어줌으로써 1배 한 것을 더하는 것이 된다.



	TCanvas *c = new TCanvas("c", "Energy_Pileup_Corrected", 800, 600);
	c->SetLogy();
//TCanvas는 plot이 그려질 figure와axis를 customize를 해주는 constructor로서, argument에 파일이름, plot타이틀, 픽셀의 가로 세로가 입력되야 한다. Line 7과 마찬가지로, TCanvas type pointer를 c로 선언하였고, dynamic memoery (pointer) 로 선언된 TCanvas를 할당해주었다. customizing의 한 부분으로 우리는 y축을 log scale로 변환을 하였는데 그 이유는?

	energy_full->Draw();
	energy_d->Draw("same");
	energy_d->SetLineColor(2);
	energy_s1->Draw("same");
	energy_s1->SetLineColor(6);
	energy_s2->Draw("same");
	energy_s2->SetLineColor(6);
	

	energy_pileup_corrected->Draw("same");
	energy_pileup_corrected->SetLineColor(3);

// Line 25에서 customized된 캔버스안에 line10,12,13,14,과 17-21의 결과를 draw하는 과정이다. 총5개의 히스토그램이 그려질 텐데 모두 하나의 캔버스에 그릴것이기에 제일처음 그리는 energy_full에는 draw();라고 되어있지만, 그 이후에 그려지는것들은 draw("same")이라고 써야 하며, 라인색을 각각 다르게 지정할 수 있다. 기본색은 검정 (2)는 빨간, (3)은 파랑 (6)은 핑크색이다.

// TCanvas 를 이용해서 위의 과정을 하나의 히스토그램에 다 그려볼 것이다. 우선 line 16에서 TCanvas를 c의 메모리 안에 할당하고, 파일이름은 Energy_Pileup_Corrected라 하고 800*600 픽셀크기로 지정을 한다. 이어서 y축을 log scale로 변경을 한 뒤 기본색(검정)으로 energy_full을 그리고, 같은 히스토그램에 energy_d를 빨간색(2번)으로 energy_pileup_corrected를 초록색(3번)으로 그려보았다. 

//숙제로는 energy_s1과 energy_s2를 불러와서 energy_pileup_corrected에 두개를 더한 히스토그램을 그리는것이다
}

