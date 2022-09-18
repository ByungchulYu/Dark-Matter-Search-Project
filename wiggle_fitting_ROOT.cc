#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TMath.h" //Root에 built인 되어 있는 pi를 사용하기 위해서는 TMath라는 모듈을 include해줘야 한다

const double two_pi = 2*TMath::Pi(); 
const double omega_a = two_pi*0.23; // rad/us
//글로벌 variable로서 2개를 const double type으로 지정을 해주는데 하나는 2pi를 TMath::pi를 이용하여 지정해주고 다른 하나는 omega_a를 radian/microsecond의 유닛으로 지정을 해주었다. w_a가 원래 230kHz임을 상기하자.

double fit_func(double *x, double *par)
{
		double t = x[0];
		double N0 = par[0];
		double tau = par[1];
		double A = par[2];
		double r = par[3];
		double phi = par[4];
		return N0*exp(-t/tau)*(1+A*cos(omega_a*(1+r)*t+phi));
}
//c++ 문법을 이용하여 fit_func을 정의해주고 이 function은 다시 myTF1이라는 이름을 가진 ROOT의 TF1의 함수에서 불려져서 역할을 수행할 것이다. fit_func는 2개의 
void wiggle_fitting_ROOT()
{

TFile *f = new TFile("run2d-yolo.root", "r");

TH1F *wiggle_full = (TH1F *) gDirectory->Get("MuonCoincidence/TIME_FULL");
TH1F *wiggle_d =  (TH1F *) gDirectory->Get("MuonCoincidence/TIME_D");

TH1F *wiggle_fitting = (TH1F *) wiggle_full->Clone();
wiggle_fitting->Add(wiggle_d, -1);

TCanvas *c = new TCanvas("c", "Wiggle_Fitting", 1000, 800);
c->SetLogy();

wiggle_fitting->Draw();

TF1 *func = new TF1("myTF1", fit_func, 30, 300, 5);
func->SetParameters(1e7, 64.4, 0.4, 0, 0);

wiggle_fitting->Fit("myTF1", "R");


}
