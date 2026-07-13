void analysis_peak(){
    TFile * file = TFile::Open("./test6_output.root");
    TCanvas* c1 = new TCanvas("c1", "c1", 800, 800);
    c1->SetMargin(0.10,0.05,0.10,0.05);
    c1->Draw();
    c1->SetLogy();
    c1->SetGridx();
    c1->SetGridy();
    TH1D* h2 = (TH1D*) file->Get("h2");
    h2->Draw("HIST E");
    TF1 * f1 = new TF1("f1", "exp([1]*x+[2])*[0]", 2, 34);
    f1->SetLineColor(kRed);
    h2->Fit(f1, "R", "goff");
    h2->GetXaxis()->SetTitleSize(0.03);
    h2->GetYaxis()->SetTitleSize(0.03);
    h2->GetXaxis()->SetTitleOffset(1.2);
    h2->GetXaxis()->SetLabelSize(0.03);
    h2->GetYaxis()->SetLabelSize(0.03);
    f1->Draw("SAME");


    TCanvas* c2 = new TCanvas("c2", "c2", 800, 800);
    c2->SetGridx();
    c2->SetGridy();
    c2->SetMargin(0.10,0.05,0.10,0.05);
    TH1D* h2c = new TH1D(*h2);
    for(int i=1; i<=h2c->GetNbinsX(); i++){
        h2c->SetBinContent( i, h2c->GetBinContent(i)/(f1->Eval(h2c->GetBinCenter(i))) );
        h2c->SetBinError( i, h2c->GetBinError(i)/(f1->Eval(h2c->GetBinCenter(i))) );
    }
    h2c->GetXaxis()->SetTitleSize(0.03);
    h2c->GetYaxis()->SetTitleSize(0.03);
    h2c->GetXaxis()->SetTitleOffset(1.2);
    h2c->GetXaxis()->SetLabelSize(0.03);
    h2c->GetYaxis()->SetLabelSize(0.03);
    h2c->GetYaxis()->SetTitle("Ratio");
    // h2c->GetYaxis()->Set(0.03);
    h2c->Draw();

    c1->SaveAs("Qupid-Ratio1.pdf");
    c2->SaveAs("Qupid-Ratio2.pdf");

}